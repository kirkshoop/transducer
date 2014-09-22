cmake_minimum_required(VERSION 2.8)

project(ducer)

MESSAGE( STATUS "CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID} )

FIND_PACKAGE(Threads)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list( APPEND CMAKE_CXX_FLAGS " -std=c++1y -ftemplate-depth=1024 ${CMAKE_CXX_FLAGS}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    list( APPEND CMAKE_CXX_FLAGS " -std=c++1y ${CMAKE_CXX_FLAGS}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    list( APPEND CMAKE_CXX_FLAGS " /DUNICODE /D_UNICODE /bigobj ${CMAKE_CXX_FLAGS}")
endif()


# define some folders
get_filename_component(DUCER_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(DUCER_DIR "${DUCER_DIR}" PATH)

MESSAGE( STATUS "DUCER_DIR: " ${DUCER_DIR} )


include_directories(${DUCER_DIR}/ext/catch/include ${DUCER_DIR}/src)


set(TEST_DIR ${DUCER_DIR}/test)

# define the sources of the self test
set(TEST_SOURCES
    ${TEST_DIR}/test.cpp
)
add_executable(ducer_test ${TEST_SOURCES})
TARGET_LINK_LIBRARIES(ducer_test ${CMAKE_THREAD_LIBS_INIT})

add_executable(ducer_gist ${TEST_DIR}/gist.cpp)
TARGET_LINK_LIBRARIES(ducer_gust ${CMAKE_THREAD_LIBS_INIT})

# configure unit tests via CTest
enable_testing()

add_test(NAME RunTests COMMAND ducer_test)

add_test(NAME ListTests COMMAND ducer_test --list-tests)
set_tests_properties(ListTests PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]+ test case")

add_test(NAME ListTags COMMAND ducer_test --list-tags)
set_tests_properties(ListTags PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]+ tags")
