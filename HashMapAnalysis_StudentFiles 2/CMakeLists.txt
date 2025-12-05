cmake_minimum_required(VERSION 3.17)
project(HashTablesOpenAddressing)

set(CMAKE_CXX_STANDARD 17)

add_executable(HashTablesOpenAddressing main.cpp HashTableDictionary.cpp HashTableDictionary.hpp InvertedListDictionary.hpp InvertedListDictionary.cpp SmallIntMixedOperations.cpp SmallIntMixedOperations.hpp
        Operations.hpp
        TraceConfig.cpp
)

add_executable(LRU_Trace
        src/trace-generators/lru-generator/main.cpp
)

add_executable(LRU_Harness
        src/harness/main.cpp
        HashTableDictionary.cpp
)

target_include_directories(LRU_Harness PRIVATE
        ${CMAKE_SOURCE_DIR}
)