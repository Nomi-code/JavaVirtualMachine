#include <cstddef>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>
#include <spdlog/spdlog.h>
#include <mutex>
#include <array>
#include <shared_mutex>

#include "java_base.hpp"

class StringPool {
  private:
    constexpr static int pool_size = 17;
    static std::array<std::unordered_set<std::string>, pool_size> pool;
    static std::array<std::shared_mutex, pool_size> pool_locks;

    StringPool(const StringPool&) = delete;
    StringPool& operator=(const StringPool&) = delete;
    StringPool(StringPool&&) = delete;

    static size_t get_pool_index(const std::string& str) {
        return std::hash<std::string>{}(str) % pool_size;
    }

  public:
    static const std::string& intern(const std::string& str) {
        size_t index = get_pool_index(str);
        auto& mtx = pool_locks[index];
        auto& container = pool[index];
        {
            std::shared_lock<std::shared_mutex> read_lock(mtx);
            auto it = container.find(str);
            if (it != container.end()) {
                return *it;
            }
        }
        {
            std::unique_lock<std::shared_mutex> write_lock(mtx);
            auto [it, inserted] = container.emplace(str);
            return *it;
        }
    }
};

inline std::array<std::unordered_set<std::string>, StringPool::pool_size> StringPool::pool;
inline std::array<std::shared_mutex, StringPool::pool_size> StringPool::pool_locks;