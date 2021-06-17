cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

##############################################################################################################

project(fty-pack VERSION 1.0.0)

########################################################################################################################

find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR})

########################################################################################################################

option(WITH_PROTOBUF "Using protobuf provider" ON)
option(WITH_ZCONFIG "Using zconfig provider" ON)

########################################################################################################################

set(sources)
set(defs)
set(libs)

if (WITH_PROTOBUF)
    find_package(Protobuf QUIET)
    if (NOT Protobuf_FOUND OR NOT Protobuf_PROTOC_EXECUTABLE)
        message(FATAL_ERROR "You tried to compile with protobuf support, but protobuf was not found, please install \
            libprotobuf-dev \
            libprotoc-dev \
            protobuf-compiler ")
    endif()

    list(APPEND sources src/providers/protobuf.cpp)
    list(APPEND defs    -DWITH_PROTOBUF)
    list(APPEND libs    protobuf::libprotobuf)
endif()

if (WITH_ZCONFIG)
    resolve(czmq)
    list(APPEND sources src/providers/zconfig.cpp)
    list(APPEND defs    -DWITH_ZCONFIG)
    list(APPEND libs    czmq)
endif()

########################################################################################################################

etn_target(shared ${PROJECT_NAME}
    PUBLIC
        pack/pack.h
        pack/attribute.h
        pack/list.h
        pack/map.h
        pack/value.h
        pack/enum.h
        pack/node.h
        pack/magic_enum.h
        pack/types.h
        pack/serialization.h
        pack/proto-map.h
        pack/visitor.h
        pack/variant.h

    SOURCES
        src/node.cpp
        src/attribute.cpp
        src/providers/yaml.cpp
        ${sources}
    PREPROCESSOR ${defs}
    USES_PUBLIC
        fty-utils
    USES
        yaml-cpp
        ${libs}
)


##############################################################################################################

if (WITH_PROTOBUF)
    add_subdirectory(protoc)
    include(protoc/cmake/protogen.cmake)
endif()

##############################################################################################################

#if (WITH_PROTOBUF)
#    etn_test_target(${PROJECT_NAME}
#        SOURCES
#            tests/main.cpp
#            tests/simple.cpp
#            tests/list.cpp
#            tests/nested.cpp
#            tests/enums.cpp
#            tests/map.cpp
#            tests/include.cpp
#            tests/child.cpp
#            tests/variant.cpp
#            tests/json.cpp
#            tests/options.cpp
#        PREPROCESSOR
#            -DCATCH_CONFIG_FAST_COMPILE
#        SUBDIR
#            tests
#    )

#    if (TARGET ${PROJECT_NAME}-test)
#        fty_protogen(
#            TARGET ${PROJECT_NAME}-test
#            WORKDIR tests
#            PROTO
#                tests/examples/example1.proto
#                tests/examples/example2.proto
#                tests/examples/example3.proto
#                tests/examples/example4.proto
#                tests/examples/example5.proto
#                tests/examples/example6.proto
#                tests/examples/example7.proto
#                tests/examples/example8.proto
#        )
#    endif()

#    if (TARGET ${PROJECT_NAME}-coverage)
#        fty_protogen(
#            TARGET ${PROJECT_NAME}-coverage
#            WORKDIR tests
#            PROTO
#                tests/examples/example1.proto
#                tests/examples/example2.proto
#                tests/examples/example3.proto
#                tests/examples/example4.proto
#                tests/examples/example5.proto
#                tests/examples/example6.proto
#                tests/examples/example7.proto
#                tests/examples/example8.proto
#        )
#    endif()
#endif()

if (BUILD_TESTING AND WITH_PROTOBUF)
    etn_test(${PROJECT_NAME}-test
        SOURCES
            tests/main.cpp
            tests/simple.cpp
            tests/list.cpp
            tests/nested.cpp
            tests/enums.cpp
            tests/map.cpp
            tests/include.cpp
            tests/child.cpp
            tests/variant.cpp
            tests/json.cpp
            tests/options.cpp
        PREPROCESSOR -DCATCH_CONFIG_FAST_COMPILE
        USES
            ${PROJECT_NAME}
            Catch2::Catch2
    )

    fty_protogen(
        TARGET ${PROJECT_NAME}-test
        WORKDIR tests
        PROTO
            tests/examples/example1.proto
            tests/examples/example2.proto
            tests/examples/example3.proto
            tests/examples/example4.proto
            tests/examples/example5.proto
            tests/examples/example6.proto
            tests/examples/example7.proto
            tests/examples/example8.proto
    )

    etn_coverage(${PROJECT_NAME}-test SUBDIR tests)
endif()


##############################################################################################################

