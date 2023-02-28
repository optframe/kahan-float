
#==========
#  TESTS
#----------
# add_subdirectory(tests/thirdparty/catch2/)
FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.3.1)
FetchContent_MakeAvailable(Catch2)
#
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
#
add_executable(kahan-float-tests kahan-float_tests/kahan.test.cpp)
target_link_libraries(kahan-float-tests PRIVATE kahan-float Catch2::Catch2WithMain)
#
#add_compile_definitions(CYCLES_TEST)  # just for testing ?
catch_discover_tests(kahan-float-tests)


# MANUAL:
# target_link_libraries(my_test PUBLIC Catch2)
# enable_testing()
# add_test(NAME MeusTestes COMMAND my_test)


# ==========
# benchmarks
# ----------

# disable internal testing for google benchmark
set(BENCHMARK_ENABLE_TESTING off)
# get compatible versions: gbench v1.6.0 with gtest 1.11.0
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG release-1.11.0)     
FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git GIT_TAG v1.6.0)
FetchContent_MakeAvailable(googletest googlebenchmark)

add_executable(all_bench all.bench.cpp)
target_link_libraries(all_bench PRIVATE kahan-float benchmark::benchmark)

