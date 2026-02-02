#ifndef POOL_H
#define POOL_H

#include <vector>
#include <memory>
#include <functional>
#include <iostream>

/**
 * @class Pool
 * @brief Template class for object pooling pattern
 *
 * Reduces memory allocations by reusing objects.
 * Useful for frequently created/destroyed objects like projectiles.
 *
 * @tparam T The type of objects to pool
 */
template<typename T>
class Pool {
public:
    /**
     * @brief Construct a pool with initial capacity
     * @param initialSize Number of objects to pre-allocate
     * @param factory Function to create new objects
     */
    explicit Pool(size_t initialSize = 10,
                  std::function<T()> factory = []() { return T(); })
        : mFactory(factory), mTotalCreated(initialSize)
    {
        // Pre-allocate objects
        for (size_t i = 0; i < initialSize; ++i) {
            mAvailable.push_back(mFactory());
        }
        std::cout << "Pool initialized with " << initialSize << " objects\n";
    }

    /**
     * @brief Get an object from the pool
     * @return An object (either recycled or newly created)
     */
    T acquire() {
        if (mAvailable.empty()) {
            // Pool exhausted - create new object
            mTotalCreated++;
            return mFactory();
        }

        // Reuse from pool
        T obj = std::move(mAvailable.back());
        mAvailable.pop_back();
        mActiveCount++;
        return obj;
    }

    /**
     * @brief Return an object to the pool for reuse
     * @param obj The object to return
     */
    void release(T&& obj) {
        mAvailable.push_back(std::move(obj));
        if (mActiveCount > 0) mActiveCount--;
    }

    /**
     * @brief Get statistics
     */
    size_t getAvailableCount() const { return mAvailable.size(); }
    size_t getActiveCount() const { return mActiveCount; }
    size_t getTotalCreated() const { return mTotalCreated; }

    /**
     * @brief Clear all pooled objects
     */
    void clear() {
        mAvailable.clear();
        mActiveCount = 0;
    }

private:
    std::vector<T> mAvailable;              ///< Pool of available objects
    std::function<T()> mFactory;            ///< Factory function to create new objects
    size_t mActiveCount = 0;                ///< Number of active objects
    size_t mTotalCreated = 0;               ///< Total objects created
};

#endif // POOL_H
