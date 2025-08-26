#include "java_base.hpp"

template <typename T> class Singleton {
  public:
    template <typename... Args> static T& instance(Args&&... args) {
        std::call_once(init_flag,
                       [&] { instance_ptr = std::make_unique<T>(std::forward<Args>(args)...); });
        return *instance_ptr;
    }

  protected:
    Singleton() = default;
    ~Singleton() = default;

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

  private:
    inline static std::unique_ptr<T> instance_ptr;
    inline static std::once_flag init_flag;
};
