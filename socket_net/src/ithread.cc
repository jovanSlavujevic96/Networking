#include "ithread.h"

IThread::IThread()
{

}

IThread::~IThread()
{
    if (mThread)
    {
        delete mThread;
    }
}

void IThread::start()
{
    if (!mThread)
    {
        mThread = new std::thread(&IThread::internalThreadCall, this);
    }
}

void IThread::join()
{
    if (mThread)
    {
        mThread->join();
    }
}

void IThread::detach()
{
    if (mThread)
    {
        mThread->detach();
    }
}

void* IThread::internalThreadCall(void* _this)
{
    reinterpret_cast<IThread*>(_this)->threadEntry();
    return NULL;
}