#ifndef ASYNC_PRINTING
#define ASYNC_PRINTING

#include <queue>
#include <mutex>
#include <condition_variable>

extern std::queue<std::string> msg_queue;
extern std::mutex output_mutex;
extern std::condition_variable output_cv;
extern bool run_async_printing;

void async_print(std::string msg);
void print_worker();

#endif