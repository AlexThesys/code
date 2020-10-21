//RAII join thread

class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(std::thread& _t) : t(_t) {}
    ~thread_guard()
    {
        if (t.joinable())   t.join();
    }
    thread_guard(const& thread_guard) = delete;
    void operator=(const & thread_guard) =  delete;
};

//-----------------------------

class join_threads
{
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std:thread>& t) : threads(t) {}
    ~join_threads()
    {
        for (auto& i : threads)
            if (i.joinable)
                i.join();
    }
};
