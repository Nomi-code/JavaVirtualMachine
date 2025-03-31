#include "java_base.hpp"
#include <atomic>

class SpinLock {
  public:
    SpinLock() = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock) = delete;

    void lock() {
        while (flag.test_and_set()) {
        }
    }
    
    void unlock() {
        flag.clear();
    }

  private:
    std::atomic_flag flag;
};