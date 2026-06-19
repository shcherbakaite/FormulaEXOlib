cmake_minimum_required(VERSION 3.5)
project(FormulaEXOlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
    FetchContent_Declare(
            glm
            URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    )
    FetchContent_MakeAvailable(glm)
    if(TARGET glm AND NOT TARGET glm::glm)
        add_library(glm::glm ALIAS glm)
    endif()
endif()

add_library(FormulaEXOlib STATIC library.cpp
        VehicleController.cpp
        VehicleController.h)
target_include_directories(FormulaEXOlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(FormulaEXOlib PUBLIC glm::glm)

include(CTest)

if(BUILD_TESTING)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # Keep GoogleTest from overriding parent project compiler/linker settings.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(FormulaEXOlib_tests tests/library_test.cpp)
    target_link_libraries(FormulaEXOlib_tests
            PRIVATE
            FormulaEXOlib
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(FormulaEXOlib_tests)
endif()
