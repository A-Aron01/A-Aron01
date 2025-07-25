CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -mtune=native -flto -DNDEBUG
CXXFLAGS += -Wall -Wextra -Wpedantic
CXXFLAGS += -ffast-math -funroll-loops -finline-functions
CXXFLAGS += -fomit-frame-pointer -fno-exceptions
LDFLAGS = -flto -s

TARGET = optimized_program
SOURCE = first.c++

# Release build with maximum optimizations
release: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

# Debug build for development
debug: CXXFLAGS = -std=c++17 -O0 -g -Wall -Wextra -Wpedantic
debug: LDFLAGS = 
debug: $(TARGET)

# Profile-guided optimization build
pgo: CXXFLAGS += -fprofile-generate
pgo: $(TARGET)
	./$(TARGET)
	$(CXX) $(CXXFLAGS:-fprofile-generate=-fprofile-use) -o $(TARGET) $(SOURCE) $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.gcda *.gcno

# Analyze performance with perf
profile: $(TARGET)
	perf record -g ./$(TARGET)
	perf report

.PHONY: release debug pgo clean profile