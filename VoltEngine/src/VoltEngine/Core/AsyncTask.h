#pragma once

#include <mutex>
#include <future>
#include <functional>
#include <sstream>

#include "Log.h"

namespace Volt
{
    class CAsyncTask
    {
    public:
        template <typename T>
        void RunTask(T& taskData, std::function<void(std::mutex&, T&)> executeTaskFn, std::function<void()> onCompletedFn)
        {
            m_future = std::async(std::launch::async, [this, executeTaskFn, onCompletedFn](T& taskData)
            {
                VOLT_LOG(Info, "Started thread.");
                if (executeTaskFn)
                    executeTaskFn(m_mutex, taskData);
                else
                    VOLT_LOG(Warning, "Invalid async task in thread.");
                if (onCompletedFn)
                    onCompletedFn();
                else
                    VOLT_LOG(Warning, "Invalid OnCompletedFn param in thread.");
                VOLT_LOG(Info, "Finished thread.");
            }, taskData);
        }

        void Wait();
        void LockMutex();
        void UnlockMutex();

    private:
        std::mutex m_mutex;
        std::future<void> m_future;
    };
}
