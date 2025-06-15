#pragma once
#include <iostream>
using namespace std;

template <typename T, typename Alloc = std::allocator<T>>
class MyVector {
   public:
    using iterator = T *;
    using const_iterator = const T *;
    using allocator_type = Alloc;
    allocator_type allocator;

    size_t size_ = 0;
    size_t capacity_ = 0;
    T *data = nullptr;

    MyVector(const allocator_type &alloc = allocator_type())
        : allocator(alloc), data(nullptr), size_(0), capacity_(0) {};

    MyVector(const MyVector &other) {
        size_ = other.size_;
        capacity_ = other.capacity_;

        data = allocator.allocate(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            allocator.construct(data + i, other.data_[i]);
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) return;

        T *new_data = allocator.allocate(new_capacity);

        for (size_t i = 0; i < size_; ++i) {
            allocator.construct(new_data + i, std::move(data[i]));
            allocator.destroy(data + i);
        }

        if (data) {
            allocator.deallocate(data, capacity_);
        }

        data = new_data;
        capacity_ = new_capacity;
    }

    void push_back(const T &value) {
        if (size_ == capacity_) {
            size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            T *new_data = allocator.allocate(new_capacity);

            for (size_t i = 0; i < size_; ++i) {
                allocator.construct(new_data + i,
                                    std::move_if_noexcept(data[i]));
                allocator.destroy(data + i);
            }

            allocator.deallocate(data, capacity_);

            data = new_data;
            capacity_ = new_capacity;
        }

        allocator.construct(data + size_, value);
        ++size_;
    }

    ~MyVector() {
        // for (size_t i = 0; i < size; ++i) {
        //      allocator.destroy(data + i);
        //  }
        if (data) {
            allocator.deallocate(data, capacity_);
        }
    }

    T &operator[](size_t index) { return data[index]; }

    iterator begin() { return data; }
    const_iterator begin() const { return data; }
    iterator end() { return data + size_; }
    const_iterator end() const { return data + size_; }

    size_t size() { return size_; }

    bool empty() const noexcept { return (size_ == 0); }
};