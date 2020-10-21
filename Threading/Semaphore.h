#ifndef SEMAPHORE_H_INCLUDED
#define SEMAPHORE_H_INCLUDED

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

using namespace std;

class Semaphore
{
    mutex mu;
    condition_variable cv;
    atomic<unsigned int> counter{0};

public:
    void init(unsigned int items) {counter.store(items);}

    void notify()
    {
        counter.fetch_add(1u);
        cv.notify_one();
    }

    void release(unsigned int t)
    {
        counter.fetch_add(t);
        cv.notify_all();
    }

    void wait()
    {
        unique_lock<mutex> locker(mu);
        unsigned int t = counter.load();
        if(!t)
            cv.wait(locker, [&](){return this->counter.load();});
        counter.fetch_sub(1u);
    }

    bool try_wait()
    {
        unsigned int  t = counter.load();
        if(t) {
            counter.fetch_sub(1u);
            return true;
        }
        return false;
    }
};

#endif // SEMAPHORE_H_INCLUDED
