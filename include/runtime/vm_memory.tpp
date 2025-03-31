#include "vm_memory.hpp"
#include <mutex>

using namespace rt_jvm_memory;

template <std::size_t N> void StringPoolMemory<N>::add_record(const std::string_view& sv) {
    std::lock_guard<SpinLock> guard(this->lock);
    this->strings_rec.emplace(sv);
}

template <std::size_t N> bool StringPoolMemory<N>::contains(const std::string_view& sv) const {
    return this->strings_rec.contains(sv);
}