include(GoogleTest)
find_package(GTest REQUIRED)

set(DIR_TEST_SRC_PATH ${DIR_TEST_PATH}/src)
set(DIR_TEST_BIN_PATH ${DIR_TEST_PATH}/bin)
set(DIR_TEST_INCLUDE_PATH ${DIR_TEST_PATH}/include)
add_definitions(-DDEBUG)

include_directories(${DIR_TEST_INCLUDE_PATH})
aux_source_directory(${DIR_TEST_SRC_PATH} SRC)

add_executable(JavaVirtualMachineTest ${SRC})
target_link_libraries(JavaVirtualMachineTest GTest::GTest GTest::Main)
set(EXECUTABLE_OUTPUT_PATH ${DIR_TEST_BIN_PATH})

add_test(NAME CLASS_FILE_TEST COMMAND JavaVirtualMachineTest)