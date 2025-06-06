cmake_minimum_required(VERSION 3.20.0)
project(JavaVirtualMachine VERSION 0.1.0 LANGUAGES C CXX)

# cmake_policy(SET CMP0167 NEW)

include(FetchContent)

set(JavaVirtualMachine PROJECT_NAME)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(DIR_TEST_PATH ${PROJECT_SOURCE_DIR}/test)
set(DIR_SRC_PATH ${PROJECT_SOURCE_DIR}/src)
set(DIR_LIB_PATH ${PROJECT_SOURCE_DIR}/lib)
set(DIR_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
set(DIR_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

SET(CMAKE_C_COMPILER /usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD_REQUIRED 20)
set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
 
add_compile_options(-std=c++20 -gdwarf-4 -O)
add_link_options(-lstdc++fs)

add_subdirectory(${DIR_TEST_PATH})
add_subdirectory(${DIR_SRC_PATH})

include_directories(${DIR_INCLUDE_PATH})

enable_testing()

add_custom_target(clean-all
    COMMAND rm -rf bin/*
    COMMAND rm -rf build/*
    COMMAND rm -rf test/bin/*
)