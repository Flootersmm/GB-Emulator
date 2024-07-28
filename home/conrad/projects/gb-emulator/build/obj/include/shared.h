#ifndef SHARED_H
#define SHARED_H

#include <atomic>
#include <mutex>
#include <thread>

extern std::atomic<bool> running;
extern std::mutex emu_mutex;

#endif
