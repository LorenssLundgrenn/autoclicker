#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "print.hpp"

std::queue<std::string> msg_queue;
std::mutex output_mutex;
std::condition_variable output_cv;
bool run_async_printing = true;

void async_print(std::string msg) {
    std::unique_lock<std::mutex> lock(output_mutex);
    msg_queue.push(msg);
    output_cv.notify_one();
}

void print_worker() {
    while (true) {
        std::unique_lock<std::mutex> lock(output_mutex);
        output_cv.wait(lock, [](){ return !msg_queue.empty() || !run_async_printing; });
        if (!run_async_printing) break;
        std::string msg = msg_queue.front();
        msg_queue.pop();
        lock.unlock();
        std::cout << msg << std::flush;
    }
}