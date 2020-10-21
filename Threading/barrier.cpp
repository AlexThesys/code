#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

class Barrier
{
    std::mutex mu;
    std::condition_variable cv;
    size_t num_threads;
    std::atomic<unsigned int> counter{0};

public:
    Barrier(size_t num_t) : num_threads(num_t) {}

    void block() {
        const unsigned int  t = counter.fetch_add(1u);
        if (++t == num_threads) {
            conter.store{0u};
            cv.notify_all();
        } else {
            std::unique_lock<mutex> locker(mu);
            cv.wait(locker, [&t](){return counter.load() == num_threads;});
        }
    }
};
