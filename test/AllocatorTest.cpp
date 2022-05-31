//
// Created by cefour on 27.05.22.
//
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE ALLOCATOR_TEST_MODULE
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/datastructure/table.h"
#include "../src/api/prime_stream.h"
#include "../src/api/time_measure.h"
#include "../src/storage/data_types.h"
#include <algorithm>

/*
 * NEEDED INCLUDES
 *
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>





int fd;
template<class T>
T * createMemory(int sz){
    //fd = open("test.map", O_RDWR | O_APPEND | O_CREAT, 0777);
    fd = open("test.map", O_RDWR );
    T * ptr = static_cast<T*>(mmap ( NULL, sz*sizeof(T),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ));
    if(ptr == MAP_FAILED){
        std::cout<<std::strerror(errno)<<std::endl;
        throw std::runtime_error("MAP FAILED");
    }
    return ptr;
}

template<class T>
void  destroyMemory(T * ptr,int sz){
    munmap(ptr,sz * sizeof(T));
    close(fd);
}


BOOST_AUTO_TEST_SUITE(AllocatorTestSuite)

    BOOST_AUTO_TEST_CASE(Test1){
        int * ptr = createMemory<int>(10000);

        for(int i = 0; i < 10000; i++){
            ptr[i] = i;
            std::cout<<ptr[i]<<"\n";
        }

        destroyMemory<int>(ptr,10000);
    }

BOOST_AUTO_TEST_SUITE_END()
