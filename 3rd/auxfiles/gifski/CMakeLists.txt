set(GIFSKI_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd/gifski/gifski-api)
set(GIFSKI_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/target)
set(GIFSKI_RELEASE ${GIFSKI_BUILD_DIR}/release/gifski.lib)

add_custom_command(
    OUTPUT ${GIFSKI_RELEASE}
    COMMAND cargo rustc --lib --release --target-dir ${GIFSKI_BUILD_DIR} -- -C target-feature=+crt-static

    WORKING_DIRECTORY ${GIFSKI_ROOT_DIR}
)

add_custom_target(gifski
DEPENDS
    ${GIFSKI_RELEASE}
)

add_library(libgifski STATIC IMPORTED GLOBAL)
set_target_properties(libgifski
PROPERTIES
    IMPORTED_LOCATION ${GIFSKI_RELEASE}
)
target_include_directories(libgifski INTERFACE ${GIFSKI_ROOT_DIR})

target_link_libraries(libgifski
INTERFACE
    Userenv
    Ws2_32
)

add_dependencies(libgifski gifski)
