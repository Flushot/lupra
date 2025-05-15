cmake_minimum_required(VERSION 3.31)
project(lupra C)

include(FetchContent)
include(CTest)

set(lupra_VERSION_MAJOR 0)
set(lupra_VERSION_MINOR 1)

# Enable for debugging
# set(CMAKE_VERBOSE_MAKEFILE ON)

# C compiler flags
set(CMAKE_C_STANDARD 23) # Minimum C20 required for __VA_OPT__ macro
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused-command-line-argument")

# Source files
set(
    SOURCES
    src/library.c
    src/algos/murmur3.c
    src/structs/array_list.c
    src/structs/bit_array.c
    src/structs/bloom_filter.c
    src/structs/hash_table.c
    src/structs/linked_list.c
    src/structs/heap.c
    src/utils/value.c
    src/utils/net_utils.c
)

# Main program
add_library(lupra STATIC ${SOURCES})
install(TARGETS lupra DESTINATION lib)
# configure_file(zlog.conf zlog.conf COPYONLY)

# math
# Library doesn't seem to be included on all platforms
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(lupra PUBLIC ${MATH_LIBRARY})
endif()

# Unit tests
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
    file(COPY ci DESTINATION .)

    add_executable(
        test_runner
        src/test.c
        src/tests/algos/array_test.c
        src/tests/structs/array_list_test.c
        src/tests/structs/bit_array_test.c
        src/tests/structs/bloom_filter_test.c
        src/tests/structs/hash_table_test.c
        src/tests/structs/linked_list_test.c
        src/tests/structs/heap_test.c
        src/tests/algos/murmur3_test.c
        src/tests/utils/net_utils_test.c
    )
    target_link_libraries(test_runner PRIVATE lupra)
    # https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html
    add_test(NAME unit_test COMMAND test_runner)

    # zlog test configuration
    # configure_file(zlog_test.conf zlog_test.conf COPYONLY)
    # set_property(TEST unit_test PROPERTY ENVIRONMENT ZLOG_CONF_PATH=zlog_test.conf)

    # cunit
    # https://cunit.sourceforge.net/doc/index.html
    # https://cunity.gitlab.io/cunit/
    FetchContent_Declare(
        cunit
        GIT_REPOSITORY https://gitlab.com/cunity/cunit.git
        GIT_TAG 3.5.2
    )
    FetchContent_MakeAvailable(cunit)
    target_include_directories(test_runner PRIVATE ${cunit_SOURCE_DIR}/CUnit)
    target_link_libraries(test_runner PRIVATE cunit)

    # target_include_directories(test_runner PRIVATE ${zlog_SOURCE_DIR}/src)
    # target_link_libraries(test_runner PRIVATE zlog)
endif()

# zlog
# https://hardysimpson.github.io/zlog/UsersGuide-EN.html
# FetchContent_Declare(
#     zlog
#     GIT_REPOSITORY https://github.com/HardySimpson/zlog.git
#     GIT_TAG 1.2.18
# )
# FetchContent_MakeAvailable(zlog)
# if(APPLE)
#     set_property(TARGET zlog PROPERTY MACOSX_RPATH ON)
# endif()
# target_include_directories(lupra PRIVATE ${zlog_SOURCE_DIR}/src)
# target_link_libraries(lupra PRIVATE zlog)

# Documentation
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # Theme
        # https://github.com/jothepro/doxygen-awesome-css?tab=readme-ov-file
        FetchContent_Declare(
            doxygen-awesome-css
            URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
        )
        FetchContent_MakeAvailable(doxygen-awesome-css)
        FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

        set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
        set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)

        configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
        message("Doxygen build started")

        # Create output directory
        file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

        add_custom_target(
            doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
            MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
            DEPENDS ${SOURCES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )

        install(
            DIRECTORY ${DOXYGEN_OUPUT_DIR}
            DESTINATION share/doc/${PROJECT_NAME}
            OPTIONAL
        )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()
