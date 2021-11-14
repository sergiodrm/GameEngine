#pragma once
#include "Log.h"

namespace Volt
{
    template <typename T>
    class ISingleton
    {
    private:
    public:
        ISingleton() = default;
        ISingleton(const ISingleton&) = delete;
        ISingleton(const ISingleton&&) = delete;
        void operator=(const ISingleton&) = delete;
        void operator=(const ISingleton&&) = delete;


        virtual ~ISingleton() = default;

        template <typename ... Args>
        static void Create(Args&& ... args)
        {
            VOLT_ASSERT(s_instance == nullptr, "Singleton was already created!");
            s_instance = new T(std::forward<Args>(args)...);
            s_instance->OnCreate();
        }

        static void Destroy()
        {
            s_instance->OnDestroy();
            delete s_instance;
            s_instance = nullptr;
        }

        static T& Get() { return *s_instance; }
        static T* GetPtr() { return s_instance; }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}

    private:
        static T* s_instance;
    };

    template <typename T>
    T* ISingleton<T>::s_instance = nullptr;
}
