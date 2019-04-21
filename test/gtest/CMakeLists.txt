cmake_minimum_required (VERSION 3.10)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(gdb_test gdb_client.cpp)
target_link_libraries(gdb_test GTest::GTest)

gtest_add_tests(gdb_test "" AUTO)

add_test(NAME gdb_test COMMAND gdb_test)

#gtest_discover_tests(gdb_test "" AUTO)

#add_test(NAME monolithic COMMAND gdb_test)

#add_test(monolithic gdb_test)
