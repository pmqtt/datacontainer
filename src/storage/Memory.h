#ifndef MEMORY_H
#define MEMORY_H

template<class T> T * AllocateInitializedMemoryArray(int size) {
    T* mem = new T[size];
    for (int i = 0; i < size; ++i) {
        mem[i] = T{};
    }
    return mem;
}

template<class T> void DeallocateMemoryArray(T * mem) {
    if (mem != nullptr) {
        delete[] mem;
        mem = nullptr;
    }
}

template<class T> void deallocateMemory(T * memory) {
    if (memory != nullptr) {
        delete memory;
        memory = nullptr;
    }
}


#endif
