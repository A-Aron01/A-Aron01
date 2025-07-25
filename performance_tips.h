#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace PerformanceOptimizations {

// 1. Memory Management Optimizations
class MemoryOptimized {
private:
    std::vector<int> data_;
    
public:
    // Pre-allocate memory to avoid reallocations
    explicit MemoryOptimized(size_t expected_size) {
        data_.reserve(expected_size);
    }
    
    // Use move semantics to avoid copying
    void add_data(std::vector<int>&& new_data) {
        data_.insert(data_.end(), 
                    std::make_move_iterator(new_data.begin()),
                    std::make_move_iterator(new_data.end()));
    }
    
    // Return by const reference to avoid copying
    const std::vector<int>& get_data() const noexcept {
        return data_;
    }
};

// 2. Cache-Friendly Data Structures
struct CacheFriendlyData {
    // Pack data to improve cache locality
    struct alignas(64) CacheLineData {
        int values[16];  // Fits in one cache line (64 bytes)
    };
    
    std::vector<CacheLineData> cache_lines;
    
    // Process data in cache-friendly patterns
    void process_sequential() {
        for (auto& line : cache_lines) {
            for (int& value : line.values) {
                value *= 2;  // Simple operation
            }
        }
    }
};

// 3. String Optimizations
class StringOptimized {
private:
    std::unordered_map<std::string, int> string_cache_;
    
public:
    // Use string_view for read-only operations
    int find_string(std::string_view str) const {
        auto it = string_cache_.find(std::string(str));
        return (it != string_cache_.end()) ? it->second : -1;
    }
    
    // Reserve capacity for string operations
    std::string build_string(const std::vector<std::string>& parts) {
        size_t total_size = 0;
        for (const auto& part : parts) {
            total_size += part.size();
        }
        
        std::string result;
        result.reserve(total_size);
        
        for (const auto& part : parts) {
            result += part;
        }
        
        return result;
    }
};

// 4. Loop Optimizations
class LoopOptimized {
public:
    // Unroll small loops manually for better performance
    static void unrolled_copy(const int* src, int* dst, size_t count) {
        size_t i = 0;
        
        // Process 4 elements at a time
        for (; i + 3 < count; i += 4) {
            dst[i] = src[i];
            dst[i + 1] = src[i + 1];
            dst[i + 2] = src[i + 2];
            dst[i + 3] = src[i + 3];
        }
        
        // Handle remaining elements
        for (; i < count; ++i) {
            dst[i] = src[i];
        }
    }
    
    // Minimize branching in loops
    static int sum_positive(const std::vector<int>& data) {
        int sum = 0;
        for (int value : data) {
            // Branchless: add value if positive, 0 otherwise
            sum += value & ~(value >> 31);
        }
        return sum;
    }
};

// 5. RAII and Smart Pointer Optimizations
template<typename T>
class ObjectPool {
private:
    std::vector<std::unique_ptr<T>> available_;
    std::vector<std::unique_ptr<T>> in_use_;
    
public:
    // Reuse objects to avoid allocation overhead
    std::unique_ptr<T> acquire() {
        if (available_.empty()) {
            return std::make_unique<T>();
        }
        
        auto obj = std::move(available_.back());
        available_.pop_back();
        return obj;
    }
    
    void release(std::unique_ptr<T> obj) {
        if (obj) {
            available_.push_back(std::move(obj));
        }
    }
};

// 6. Compiler Hints for Optimization
class CompilerHints {
public:
    // Use [[likely]] and [[unlikely]] for branch prediction
    static int process_value(int value) {
        if (value > 0) [[likely]] {
            return value * 2;
        } else [[unlikely]] {
            return 0;
        }
    }
    
    // Use [[nodiscard]] to prevent ignoring return values
    [[nodiscard]] static bool is_valid(int value) noexcept {
        return value >= 0 && value <= 100;
    }
    
    // Force inlining for small, frequently called functions
    __attribute__((always_inline)) static inline int fast_multiply(int a, int b) {
        return a * b;
    }
};

} // namespace PerformanceOptimizations