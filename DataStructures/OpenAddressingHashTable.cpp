#include <limits>
#include <algorithm>
#include <memory>
#include <cmath>

template <typename T, size_t estSize = 512u, typename Hasher = std::hash<T>>
class OA_HashTable
{
    static T is_Removed;
    static T is_Empty;
    std::unique_ptr<T[]> values;
    size_t hashFunc(T t);
    size_t probeSeq;
    Hasher hash;
public:
    OA_HashTable(size_t pSeq =(64/sizeof(T))) : probeSeq(pSeq)
    {
        const size_t N = estSize*pSeq;
        values = std::unique_ptr<T[]>(new T[N]);
        for(size_t i = 0; i < N; ++i)
            values[i] = is_Empty;
    }
    void addValue(T t);
    void removeValue(T t);
    bool findValue(T t);
};

template <typename T, size_t estSize, typename Hasher>
T OA_HashTable<T, estSize, Hasher>::is_Removed = std::numeric_limits<T>::max();
template <typename T, size_t estSize, typename Hasher>
T OA_HashTable<T, estSize, Hasher>::is_Empty = std::numeric_limits<T>::min();

template <typename T, size_t estSize, typename Hasher>
void OA_HashTable<T, estSize, Hasher>::addValue(T t)
{
    if ((t >= is_Removed) || (t <= is_Empty)) return;
    size_t index = hashFunc(t);
    for (size_t i = 1; i < probeSeq; ++i){
        if ((values[index] == is_Empty) || (values[index] == is_Removed)){
            values[index] = t;
            break;
        }
        index += i*i; // quadratic probing
    }
    /*resize the table*/
}

template <typename T, size_t estSize, typename Hasher>
void OA_HashTable<T, estSize, Hasher>::removeValue(T t)
{
    size_t index = hashFunc(t);
    for (size_t i = 1; i < probeSeq; ++i){
        if (values[index] == is_Empty) break;
        if(values[index] == t){
            values[index] = is_Removed;
            break;
        }
        index += i*i; // quadratic probing
    }
}

template <typename T, size_t estSize, typename Hasher>
bool OA_HashTable<T, estSize, Hasher>::findValue(T t)
{
    size_t index = hashFunc(t);
    for (size_t i = 1; i < probeSeq; ++i){
        if (values[index] == is_Empty) return false;
        if (values[index] == t) return true;
        index += i*i; // quadratic probing
    }
    return false;
}


template <typename T, size_t estSize, typename Hasher>
size_t OA_HashTable<T, estSize, Hasher>::hashFunc(T t)
{
    return hash(t) % (estSize / probeSeq);
}

/*
    is_Empty flag is used when you add new value. Cell was not used before.
    is_Removed  flag is used when you want to find a value. Cell had been used in a
    probe sequance, but was removed (so the search is not stoped prematurely).
*/


