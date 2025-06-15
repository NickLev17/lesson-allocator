#pragma once
#include <iostream>
#include <memory>
using namespace std;
template <typename T>
class MyAllocator {
   public:
    using value_type = T;
    size_t MAX_SIZE;
    struct Buffer {
        size_t max_size;
        T *pool;
        size_t current;
        Buffer(size_t max_objects) : max_size(max_objects), current{0} {
            pool = static_cast<T *>(std::malloc(max_objects * sizeof(T)));

            if (!pool) throw std::bad_alloc();
        }

        ~Buffer() { std::free(pool); }
    };

    std::shared_ptr<Buffer> pool_ptr;

    MyAllocator() {};

    explicit MyAllocator(size_t max_objects)
        : pool_ptr{std::make_shared<Buffer>(max_objects)},
          MAX_SIZE{max_objects} {};

    ~MyAllocator() {}

    T *allocate(std::size_t n) {
        // std::cout << "elements of type " << typeid(T).name() << "\n";

        if constexpr (std::is_fundamental<T>::value) {
            pool_ptr->current == 0;
            return pool_ptr->pool;
        } else {
            T *temp = &pool_ptr->pool[pool_ptr->current];

            if (pool_ptr->current >= pool_ptr->max_size) {
                throw std::bad_alloc();
            }

            pool_ptr->current++;

            return temp;
        }
    }

    template <typename U>
    struct rebind {
        using other = MyAllocator<U>;
    };

    template <typename U>
    MyAllocator(const MyAllocator<U> &other) noexcept {
        MAX_SIZE = other.MAX_SIZE;
        pool_ptr = std::make_shared<Buffer>(MAX_SIZE);
    }

    void construct(T *p, const T &value) {
        T *temp = &pool_ptr->pool[pool_ptr->current];
        if (temp == p) {
            if (pool_ptr->current >= pool_ptr->max_size) {
                throw std::bad_alloc();
            }

            new (temp) T(value);

            pool_ptr->current++;

        } else {
        }
    }
    void expansion(size_t new_size) {
        if (pool_ptr->current < new_size) {
            T *temp = static_cast<T *>(std::malloc(new_size * sizeof(T)));

            if (!temp) throw std::bad_alloc();
            pool_ptr->current = 0;

            for (size_t i = 0; i < pool_ptr->max_size; i++) {
                new (temp + i) T(pool_ptr->pool[pool_ptr->current]);
                pool_ptr->current++;
            }

            delete[] pool_ptr->pool;
            pool_ptr->pool = temp;
            temp = nullptr;
            pool_ptr->max_size = new_size;
        }
    };

    void deallocate(T *p, std::size_t n) {}
    // void destroy(T *p);
    void destroy(T *p) { p->~T(); }

   private:
    template <typename U>
    friend class MyAllocator;
};

template <class T, class U>
constexpr bool operator==(const MyAllocator<T> &,
                          const MyAllocator<U> &) noexcept {
    return true;
}

template <class T, class U>
constexpr bool operator!=(const MyAllocator<T> &,
                          const MyAllocator<U> &) noexcept {
    return false;
}
