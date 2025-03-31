#pragma once

#include <utility>

#define SINGLETON_USED(SINGLETON_TYPE) friend class Singleton<SINGLETON_TYPE>

template <class T, class... Args> class Singleton {
  public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    template <typename... Params> static T& instance(Params&&... params) {
        static T instance(std::forward<Params>(params)...);
        return instance;
    }

  protected:
    Singleton() = default;
    ~Singleton() = default;
};