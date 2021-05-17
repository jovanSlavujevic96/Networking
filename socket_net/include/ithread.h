#pragma once

#include <thread>
#include <mutex>

class IThread
{
public:
    explicit IThread();
    virtual ~IThread();

    void start();
    void join();
    void detach();

protected:
    std::mutex mMutex;
    virtual void threadEntry() = 0;

private:
    std::thread* mThread = NULL;
    static void* internalThreadCall(void* _this);
};