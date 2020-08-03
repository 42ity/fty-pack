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

    set(protoc ${Protobuf_PROTOC_EXECUTABLE})
    set(plugin ${FTY_PACK_BIN_DIR}/protoc-gen-fty)
    if (FTY_PACK_BIN_DIR)
        set(plugin ${FTY_PACK_BIN_DIR}/protoc-gen-fty)
    else()
        set(plugin ${CMAKE_CURRENT_BINARY_DIR}/protoc/protoc-gen-fty)
    endif()

    if (arg_WORKDIR)
        get_filename_component(arg_WORKDIR ${arg_WORKDIR} ABSOLUTE)
    endif()

    foreach(proto ${arg_PROTO})
        get_filename_component(abs ${proto} ABSOLUTE)
        if (NOT arg_WORKDIR)
            get_filename_component(inc ${abs} DIRECTORY)
        else()
            set(inc ${arg_WORKDIR})
        endif()

        file(RELATIVE_PATH relPath ${inc} ${abs})
        get_filename_component(outDir ${relPath} DIRECTORY)
        get_filename_component(genName ${proto} NAME_WE)

        set(result ${CMAKE_CURRENT_BINARY_DIR}/${outDir}/${genName}.h)

        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${outDir}/${genName}.h
            COMMAND ${protoc} --plugin=${plugin} -I ${inc} --fty_out=${CMAKE_CURRENT_BINARY_DIR} ${abs}
            DEPENDS ${plugin} ${proto}
        )
        target_sources(${arg_TARGET} PRIVATE ${result})
        target_sources(${arg_TARGET} PRIVATE ${proto})
    endforeach()

    message(STATUS "Generate protos for target ${arg_TARGET}")
endmacro()
