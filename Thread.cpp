#include "Thread.h"
#include <iostream>
#include <fstream>


Thread::Thread(int size) : stop(false)
{
    for (int i = 0; i < size; i++)
    {
        threads.emplace_back(std::thread(
            [this](){
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(task_mutex);
                        cv.wait(lock, [this](){ return stop || !tasks.empty();});
                        if (stop && tasks.empty())
                        {
                            return ;
                        }
                        task = tasks.front();
                        tasks.pop();
                    }
                    std::cout<< "Thread::Thread::lambda, thread_id:" << std::this_thread::get_id() << std::endl;
                    std::ofstream ofs("main_raactor.txt", std::ofstream::app);
                    ofs << "Thread::Thread::lambda, thread_id:" << std::this_thread::get_id() << std::endl;
                    ofs.close();
                    task();
                }
                
            }

        ));
    }
}


Thread::~Thread()
{
    {
        std::unique_lock<std::mutex> lock(task_mutex);
        stop = true;
    }
    cv.notify_all();
    for (std::thread& th : threads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }
}


