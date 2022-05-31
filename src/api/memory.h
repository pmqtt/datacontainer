#ifndef MEMORY_H
#define MEMORY_H

template<class T> T * allocate_initialized_memory_array(int size) {
    T* mem = new T[size];
    for (int i = 0; i < size; ++i) {
        mem[i] = T{};
    }
    return mem;
}

template<class T> void deallocate_memory_array(T * mem) {
    if (mem != nullptr) {
        delete[] mem;
        mem = nullptr;
    }
}

template<class T> void deallocate_memory(T * memory) {
    if (memory != nullptr) {
        delete memory;
        memory = nullptr;
    }
}


#endif
