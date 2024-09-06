# Dynamic Multilevel Caching System

## Overview

This project implements a dynamic multilevel caching system that efficiently manages data across multiple cache levels with support for dynamic addition and removal of cache levels. The system supports both LRU (Least Recently Used) and LFU (Least Frequently Used) eviction policies and handles data retrieval and promotion across different cache levels.

## Features

- **Multiple Cache Levels**: Supports multiple cache levels (L1, L2, ..., Ln) with configurable sizes and eviction policies.
- **Dynamic Management**: Allows dynamic addition and removal of cache levels at runtime.
- **Eviction Policies**: Implements LRU (Least Recently Used) and LFU (Least Frequently Used) eviction policies.
- **Data Promotion**: Moves data up to higher cache levels upon retrieval from lower levels.
- **Thread Safety (Bonus)**: Concurrent access support (optional for future implementation).
- **Efficient Data Handling**: Optimizes data retrieval and minimizes cache misses.

## Structure

- **CacheLevel**: Manages data storage and eviction policies for a specific cache level.
- **EvictionPolicy**: Interface defining the behavior for eviction policies (LRU, LFU).
- **LRUEvictionPolicy**: Implementation of the LRU eviction policy.
- **LFUEvictionPolicy**: (To be implemented) Implementation of the LFU eviction policy.
- **MultilevelCacheSystem**: Manages multiple cache levels, handles data retrieval, insertion, and dynamic cache level management.

## Key Classes

- **CacheLevel**: Manages data and applies the defined eviction policy.
- **EvictionPolicy**: Interface for eviction policies.
- **LRUEvictionPolicy**: Concrete class for LRU eviction.
- **LFUEvictionPolicy**: Concrete class for LFU eviction.
- **MultilevelCacheSystem**: Manages the overall caching system and supports dynamic operations.

## Usage

### Setup

1. Clone the repository:
    ```bash
    git clone <repository_url>
    cd <repository_directory>
    ```

2. Compile the code (if using a C++ compiler):
    ```bash
    g++ -o cache_system main.cpp
    ```

3. Run the application:
    ```bash
    ./cache_system
    ```

### Example Operations

1. **Add Cache Levels**:
    ```cpp
    cacheSystem.addCacheLevel(3, "LRU");  // Adds L1 cache with LRU policy
    cacheSystem.addCacheLevel(2, "LFU");  // Adds L2 cache with LFU policy
    ```

2. **Insert Data**:
    ```cpp
    cacheSystem.put("A", "1");
    cacheSystem.put("B", "2");
    cacheSystem.put("C", "3");
    ```

3. **Retrieve Data**:
    ```cpp
    std::string value = cacheSystem.get("A");  // Retrieves data from L1
    ```

4. **Display Cache State**:
    ```cpp
    cacheSystem.displayCache();  // Shows current state of all cache levels
    ```

5. **Remove Cache Level**:
    ```cpp
    cacheSystem.removeCacheLevel(1);  // Removes L1 cache
    ```

## Key Decisions

- **Eviction Policies**: Chose LRU and LFU to cover common caching scenarios. LRU is implemented using a `LinkedHashMap` for easy access order management, and LFU can be implemented similarly.
- **Dynamic Management**: Allows flexibility in adjusting cache sizes and policies during runtime.

## Future Work

- Implement the LFU eviction policy.
- Add thread-safety to support concurrent operations.
- Enhance error handling and optimize performance further.

## Contributing

Feel free to contribute by submitting issues, pull requests, or suggestions. For significant changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

---

For any questions or issues, please contact [your_email@example.com].
