cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

##############################################################################################################

project(fty-pack VERSION 1.0.0)

##############################################################################################################

find_package(fty-cmake PATHS ${CMAKE_BINARY_DIR})

##############################################################################################################

option(WITH_PROTOBUF "Using protobuf provider" OFF)
option(WITH_ZCONFIG "Using zconfig provider" OFF)

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

    SOURCES
        src/node.cpp
        src/attribute.cpp
        src/providers/visitor.h
        src/providers/yaml.cpp
        src/providers/serialization.cpp
    USES_PUBLIC
        fty-utils
    USES
        yaml-cpp
)

if (WITH_PROTOBUF)
    target_sources(${PROJECT_NAME} src/providers/protobuf.cpp)
    target_compile_definitions(${PROJECT_NAME} -DWITH_PROTOBUF)
    resolve_lib(protobuf::libprotobuf)
    target_link_libraries(${PROJECT_NAME} PRIVATE protobuf::libprotobuf)
endif()

if (WITH_ZCONFIG)
    target_sources(${PROJECT_NAME} src/providers/zconfig.cpp)
    target_compile_definitions(${PROJECT_NAME} -DWITH_ZCONFIG)
    resolve_lib(libczmq)
    target_link_libraries(${PROJECT_NAME} PRIVATE libczmq)
endif()

##############################################################################################################

if (BUILD_TESTING AND WITH_PROTOBUF)
    find_package(fty-protoc PATHS ${CMAKE_BINARY_DIR})

    etn_target(exe ${PROJECT_NAME}-test
        SOURCES
            tests/main.cpp
            tests/simple.cpp
            tests/list.cpp
            tests/nested.cpp
            tests/enums.cpp
            tests/map.cpp
            tests/include.cpp
            tests/oneof.cpp
        PREPROCESSOR -DCATCH_CONFIG_FAST_COMPILE
        USES
            pack
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
endif()

##############################################################################################################

