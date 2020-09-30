find_package(Protobuf COMPONENTS protoc)
macro(fty_protogen)
    cmake_parse_arguments(arg
        ""
        "TARGET;WORKDIR"
        "PROTO"
        ${ARGN}
    )

    if (NOT Protobuf_PROTOC_EXECUTABLE)
        message(FATAL_ERROR "Protobuf compiler was not found, please install `protobuf-compiler`")
    endif()
    add_dependencies(${arg_TARGET} fty-protoc)

    set(protoc ${Protobuf_PROTOC_EXECUTABLE})
    set(plugin ${FTY_PACK_BIN_DIR}/protoc-gen-fty)
    if (FTY_PACK_BIN_DIR)
        set(plugin ${FTY_PACK_BIN_DIR}/protoc-gen-fty)
    else()
        if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/protoc/protoc-gen-fty)
            set(plugin ${CMAKE_CURRENT_BINARY_DIR}/protoc/protoc-gen-fty)
        elseif (EXISTS ${CMAKE_BINARY_DIR}/fty-pack/protoc/protoc-gen-fty)
            set(plugin ${CMAKE_BINARY_DIR}/fty-pack/protoc/protoc-gen-fty)
        endif()
    endif()

    foreach(proto ${arg_PROTO})
        get_filename_component(outDir ${proto} DIRECTORY)
        get_filename_component(abs ${proto} ABSOLUTE)
        if (NOT arg_WORKDIR)
            get_filename_component(inc ${abs} DIRECTORY)
        else()
            get_filename_component(inc ${arg_WORKDIR} ABSOLUTE)
            set(outDir ${arg_WORKDIR})
            target_include_directories(${arg_TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${arg_WORKDIR})
        endif()

        get_filename_component(genName ${proto} NAME_WE)
        get_filename_component(getPath ${proto} DIRECTORY)

        set(result ${CMAKE_CURRENT_BINARY_DIR}/${getPath}/${genName}.h)

        add_custom_command(
            OUTPUT  ${result}
            COMMAND ${protoc} --plugin=${plugin} -I ${inc} --fty_out=${CMAKE_CURRENT_BINARY_DIR}/${outDir} ${abs}
            DEPENDS ${plugin} ${proto}
        )
        target_sources(${arg_TARGET} PRIVATE ${result})
        target_sources(${arg_TARGET} PRIVATE ${proto})
    endforeach()

    message(STATUS "Generate protos for target ${arg_TARGET}")
endmacro()
