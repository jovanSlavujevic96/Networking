#include "ithread.h"

IThread::IThread()
{

}

IThread::~IThread()
{
    if(mThread != NULL)
    {
        delete mThread;
    }
}

void IThread::start()
{
    mThread = new std::thread(&IThread::internalThreadCall, this);
}

void IThread::join()
{
    mThread->join();
}

void IThread::detach()
{
    mThread->detach();
}

void* IThread::internalThreadCall(void* _this)
{
    reinterpret_cast<IThread*>(_this)->threadEntry();
    return NULL;
}