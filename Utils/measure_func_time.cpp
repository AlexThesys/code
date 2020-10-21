#include <chrono>

using func = template <typename T, typename... Args> T(*)(Args... args);

template <typename... Args>
auto benchmark(size_t num_iter, func function, Args&&... args)
{
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    function(std::forward<Args...>(args);
    chrono::steady_clock::time_point stop = chrono::steady_clock::now();
    chrono::steady_clock::duration d = stop - start;
    auto accumulator = chrono::duration_cast<chrono::microseconds>(d).count();
    if (num_iter > 1) {
        for (size_t i = 0; i < num_iter - 1; i++)
        {
            start = chrono::steady_clock::now();
            function(std::forward<Args...>(args);
            stop = chrono::steady_clock::now();
            d = stop - start;
            accumulator += chrono::duration_cast<chrono::microseconds>(d).count();
        }
    }
    return accumulator / static_cast<double>(num_iter);
}
