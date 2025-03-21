include(GoogleTest)
find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)

set(DIR_TEST_SRC_PATH ${DIR_TEST_PATH}/src)
set(DIR_TEST_BIN_PATH ${DIR_TEST_PATH}/bin)
set(DIR_TEST_INCLUDE_PATH ${DIR_TEST_PATH}/include)
set(EXECUTABLE_OUTPUT_PATH ${DIR_TEST_BIN_PATH})
add_definitions(-DDEBUG)

aux_source_directory(${DIR_TEST_SRC_PATH} TEST_SRC)

file(GLOB_RECURSE CLASS_FILES ${DIR_SRC_PATH}/classFile/*.cpp)
file(GLOB_RECURSE RUNTIME_FILES ${DIR_SRC_PATH}/runtime/*.cpp)
set(SOURCES ${TEST_SRC} ${CLASS_FILES} ${RUNTIME_FILES})

# debug 
# foreach(FILE ${CLASS_FILES})
#     message(STATUS "${FILE}")
# endforeach()

add_executable(JavaVirtualMachineTest)
target_sources(JavaVirtualMachineTest PRIVATE ${SOURCES})
target_link_libraries(JavaVirtualMachineTest PRIVATE GTest::GTest GTest::Main minizip)
target_include_directories(JavaVirtualMachineTest PRIVATE ${DIR_INCLUDE_PATH})

add_test(NAME CLASS_FILE_TEST COMMAND JavaVirtualMachineTest)

pkg_check_modules(MINIZIP REQUIRED minizip)
if (MINIZIP_FOUND)
    include_directories(${MINIZIP_INCLUDE_DIRS})
    target_link_libraries(JavaVirtualMachineTest PRIVATE ${MINIZIP_LIBRARIES})
else()
    message(FATAL_ERROR "minizip not found!")
endif()
