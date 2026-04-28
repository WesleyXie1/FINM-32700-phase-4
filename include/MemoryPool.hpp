#pragma once

#include <cstddef>
#include <memory>
#include <new>
#include <vector>

template <typename T, std::size_t BlockSize = 4096>
class MemoryPool {
private:
    struct Slot {
        alignas(T) unsigned char storage[sizeof(T)];
        Slot* next;
    };

public:
    MemoryPool() : free_list_(nullptr) {}

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    template <typename... Args>
    T* create(Args&&... args) {
        if (free_list_ == nullptr) {
            allocateBlock();
        }

        Slot* slot = free_list_;
        free_list_ = free_list_->next;

        return new (slot->storage) T(std::forward<Args>(args)...);
    }

    void destroy(T* object) {
        if (object == nullptr) {
            return;
        }

        object->~T();

        Slot* slot = reinterpret_cast<Slot*>(object);
        slot->next = free_list_;
        free_list_ = slot;
    }

private:
    void allocateBlock() {
        auto block = std::make_unique<Slot[]>(BlockSize);

        for (std::size_t i = 0; i < BlockSize; ++i) {
            block[i].next = free_list_;
            free_list_ = &block[i];
        }

        blocks_.push_back(std::move(block));
    }

private:
    std::vector<std::unique_ptr<Slot[]>> blocks_;
    Slot* free_list_;
};

template <typename T>
class PoolDeleter {
public:
    explicit PoolDeleter(MemoryPool<T>* pool = nullptr)
        : pool_(pool) {}

    void operator()(T* ptr) const {
        if (ptr != nullptr && pool_ != nullptr) {
            pool_->destroy(ptr);
        }
    }

private:
    MemoryPool<T>* pool_;
};