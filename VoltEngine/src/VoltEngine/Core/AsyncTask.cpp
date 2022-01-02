#include "AsyncTask.h"

namespace Volt
{
    void CAsyncTask::Wait()
    {
        if (m_future.valid())
            m_future.wait();
    }

    void CAsyncTask::LockMutex()
    {
        m_mutex.lock();
    }

    void CAsyncTask::UnlockMutex()
    {
        m_mutex.unlock();
    }
}
