#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <memory>

// Use more specific namespace instead of 'using namespace std;' for better performance
using std::cout;
using std::endl;
using std::vector;

// Example of performance-optimized C++ code
class OptimizedExample {
private:
    vector<int> data;
    
public:
    // Use move semantics for better performance
    OptimizedExample(vector<int>&& input) : data(std::move(input)) {}
    
    // Reserve memory to avoid reallocations
    void reserve_space(size_t size) {
        data.reserve(size);
    }
    
    // Use const reference to avoid copying
    void process_data(const vector<int>& additional_data) {
        // Efficient insertion
        data.insert(data.end(), additional_data.begin(), additional_data.end());
        
        // Use STL algorithms for optimized operations
        std::sort(data.begin(), data.end());
    }
    
    // Use nodiscard attribute for performance-critical functions
    [[nodiscard]] size_t get_size() const noexcept {
        return data.size();
    }
};

int main() {
    // Example of performance measurement
    auto start = std::chrono::high_resolution_clock::now();
    
    // Create optimized object with move semantics
    vector<int> initial_data = {5, 2, 8, 1, 9};
    OptimizedExample example(std::move(initial_data));
    
    // Reserve space to avoid reallocations
    example.reserve_space(1000);
    
    vector<int> more_data = {3, 7, 4, 6};
    example.process_data(more_data);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    cout << "Data size: " << example.get_size() << endl;
    cout << "Execution time: " << duration.count() << " microseconds" << endl;
    
    return 0;
}

