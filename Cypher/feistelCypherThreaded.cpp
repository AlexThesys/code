#include <memory>
#include <cassert>
#include <cstdio>
#include <cmath>
#include <random>
#include <limits>
#include <functional>
#include <thread>
#include <vector>
#include <iostream>

using namespace std;

class Random
{
public:
    Random(int mx = numeric_limits<int>::max(), int mn = 0) : _max(mx), _min(mn), A(13), B(7), M(11), x(51){}
    constexpr int random(int mx = numeric_limits<int>::max(), int mn = 0) noexcept
    {
        _max = mx;
        _min = mn;

        x = (A * x + B) % M;

        return static_cast<int>(_min + ((float)x / (float)M) * (_max - _min));
    }

    constexpr int operator()(int mx = numeric_limits<int>::max(), int mn = 0) noexcept
    {
        _max = mx;
        _min = mn;

        x = (A * x + B) % M;

        return static_cast<int>(_min + ((float)x / (float)M) * (_max - _min));
    }

    void seed(int _x) noexcept {x = _x;}
private:
    int x;
    int _max, _min;
    const int A, B, M;
};

template <typename T>
class FuncObject
{
public:
    //typedef void(Reciever::*action)();    // so action is the name of the function pointer
    using Func = void(T::*)(int*, int);     // so action is the name of the function pointer
    FuncObject(T* rec, Func a) :
        reciever(rec), act(a) {}
    void execute(int* p, int i)
    {
        (reciever->*act)(p, i); // don't foget the brackets
    }
    void operator()(int* p, int i)
    {
        (reciever->*act)(p, i); // don't foget the brackets
    }
private:
    T* reciever;
    Func act;
};

class FCypherThreaded
{
public:
    FCypherThreaded(void* arr, int cycles, int n);
    void cypherT();
    void decypherT();
private:
    Random rnd;
    unique_ptr<int[]> keys;
    int *m_arr, m_size, m_cycles, m_blockSize, m_numThreads;
    int func(int r, int k);
    void calcBlockSize(int length);
    void cypher(int* start, int len);
    void decypher(int* start, int len);
    void process(FuncObject<FCypherThreaded>&& f);
};

FCypherThreaded::FCypherThreaded(void* arr, int cycles, int n) : m_arr(reinterpret_cast<int*>(arr)),
    m_size(n),
    m_cycles(cycles)
{
    assert((m_size & 0x1) == 0);

    rnd.seed(static_cast<int>(m_cycles<<1));
    keys = unique_ptr<int[]>(new int[static_cast<unsigned>(m_cycles)]);
    for (int i = 0 ; i < m_cycles; ++i)
        keys[i] = rnd.random(m_cycles);

    calcBlockSize(m_size);
    --m_numThreads; // the last one is this thread
}

void FCypherThreaded::process(FuncObject<FCypherThreaded>&& f)
{
    std::vector<std::thread> threads(m_numThreads);
    for (int i = 0; i < m_numThreads; ++i)
        threads[i] = std::thread([this, i, &f]{f((m_arr+m_blockSize*i), m_blockSize);});
    int chuncks = m_blockSize * m_numThreads;
    int lastChunk = m_size - chuncks;
    f(m_arr+chuncks, lastChunk);
    for (auto& t : threads)
        t.join();
}

void FCypherThreaded::cypherT()
{
    FuncObject<FCypherThreaded> f(this, &FCypherThreaded::cypher);
    process(std::move(f));
}

void FCypherThreaded::decypherT()
{
    FuncObject<FCypherThreaded> f(this, &FCypherThreaded::decypher);
    process(std::move(f));
}

void FCypherThreaded::cypher(int* start, int len)
{
    unique_ptr<int[]> tempArr(new int[len >> 1]);
    for (int i = 0; i < m_cycles; ++i){
        for (int j = 0; j < len; j+=2){
            tempArr[j >> 1] = start[j];
            start[j] = start[j+1];
            start[j+1] = tempArr[j >> 1] ^ func(start[j+1], keys[i]);
        }
    }
}

void FCypherThreaded::decypher(int* start, int len)
{
    unique_ptr<int[]> tempArr(new int[len >> 1]);
    for (int i = m_cycles - 1; i >= 0; --i){
        for (int j = len - 1; j >=0; j-=2){
            tempArr[(j-1) >> 1] = start[j];
            start[j] = start[j-1];
            start[j-1] = tempArr[(j-1) >> 1] ^ func(start[j-1], keys[i]);
        }
    }
}

inline int FCypherThreaded::func(int r, int k)
{
    return ((r*r) % (k+r));
}

void FCypherThreaded::calcBlockSize(int length)
{
    constexpr int min_per_thread = 16;
    const int max_threads =
            (length+min_per_thread)/min_per_thread;
    const int hardware_threads =
            std::thread::hardware_concurrency();
    m_numThreads =
            std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    m_blockSize = length/m_numThreads;
    m_blockSize &= ~0x1;
}

int main()
{
    Random rnd;
    constexpr size_t size = 2048;
    int arr[size];
    for (size_t i = 0; i < size; ++i)
        arr[i] = rnd.random(4096);

    FCypherThreaded fct(arr, 16, size);

    fct.cypherT();

    fct.decypherT();

    return 0;
}


