set(GIFSKI_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd/gifski)
set(GIFSKI_RELEASE ${GIFSKI_ROOT_DIR}/target/release/gifski.lib)
set(GIFSKI_DEBUG ${GIFSKI_ROOT_DIR}/target/debug/gifski.lib)

add_custom_command(
    OUTPUT ${GIFSKI_RELEASE} ${GIFSKI_DEBUG}
    COMMAND cargo build --release -q
    COMMAND cargo build -q
    WORKING_DIRECTORY ${GIFSKI_ROOT_DIR}
)

add_custom_target(build_gifski
DEPENDS
    ${GIFSKI_RELEASE}
    ${GIFSKI_DEBUG}
)

add_library(gifski STATIC IMPORTED GLOBAL)
set_target_properties(gifski
PROPERTIES
    IMPORTED_LOCATION ${GIFSKI_RELEASE}
    IMPORTED_LOCATION_DEBUG ${GIFSKI_DEBUG}
)
target_include_directories(gifski INTERFACE ${GIFSKI_ROOT_DIR})

target_link_libraries(gifski
INTERFACE
    Userenv
    Ws2_32
)

add_dependencies(gifski build_gifski)
