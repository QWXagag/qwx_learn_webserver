#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <utility>

class Thread
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex task_mutex;
    std::condition_variable cv;
    bool stop;
public:
    Thread(int size = 10);
    ~Thread();
    template<class F, class... Args>
    auto add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

template<class F, class... Args>
auto Thread::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(task_mutex);
        if (stop)
        {
            throw std::runtime_error("ThreadPoll already stop, cant add task any more");
        }
        tasks.emplace([task](){(*task)();});
    }
    cv.notify_one();
    return res;
}


