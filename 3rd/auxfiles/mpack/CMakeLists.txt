set(MPACK_ROOT ${PROJECT_SOURCE_DIR}/3rd/mpack)

add_library(mpack STATIC
    ${MPACK_ROOT}/src/mpack/mpack.h
    ${MPACK_ROOT}/src/mpack/mpack-common.h
    ${MPACK_ROOT}/src/mpack/mpack-expect.c
    ${MPACK_ROOT}/src/mpack/mpack-node.c
    ${MPACK_ROOT}/src/mpack/mpack-platform.c
    ${MPACK_ROOT}/src/mpack/mpack-reader.c
    ${MPACK_ROOT}/src/mpack/mpack-writer.c
    ${MPACK_ROOT}/src/mpack/mpack-common.c
    ${MPACK_ROOT}/src/mpack/mpack-defaults.h
    ${MPACK_ROOT}/src/mpack/mpack-expect.h
    ${MPACK_ROOT}/src/mpack/mpack-node.h
    ${MPACK_ROOT}/src/mpack/mpack-platform.h
    ${MPACK_ROOT}/src/mpack/mpack-reader.h
    ${MPACK_ROOT}/src/mpack/mpack-writer.h
)

target_include_directories(mpack PUBLIC ${MPACK_ROOT}/src/mpack)
set_target_properties(mpack PROPERTIES LINKER_LANGUAGE C)
