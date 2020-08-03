cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

etn_target(exe fty-protoc
    SOURCES
        src/main.cpp
        src/generator.h
        src/generator.cpp
        src/filegenerator.h
        src/filegenerator.cpp
        src/classgenerator.h
        src/classgenerator.cpp
        src/formatter.h
        src/utils.h
    CMAKE
        cmake/protogen.cmake
    PREPROCESSOR
        -DPROTOBUF_USE_DLLS
    USES
        fty-utils
        protobuf::libprotobuf
        protobuf::libprotoc
)

set_target_properties(fty-protoc PROPERTIES OUTPUT_NAME protoc-gen-fty)

