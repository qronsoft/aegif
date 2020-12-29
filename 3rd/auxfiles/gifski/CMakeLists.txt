set(GIFSKI_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd/gifski)
set(GIFSKI_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/target)
set(GIFSKI_RELEASE ${GIFSKI_BUILD_DIR}/release/gifski.lib)
set(GIFSKI_DEBUG ${GIFSKI_BUILD_DIR}/debug/gifski.lib)

add_custom_command(
    OUTPUT ${GIFSKI_RELEASE} ${GIFSKI_DEBUG}
    COMMAND cargo build --release -q --target-dir ${GIFSKI_BUILD_DIR}
    COMMAND cargo build -q --target-dir ${GIFSKI_BUILD_DIR}
    WORKING_DIRECTORY ${GIFSKI_ROOT_DIR}
)

add_custom_target(gifski
DEPENDS
    ${GIFSKI_RELEASE}
    ${GIFSKI_DEBUG}
)

add_library(libgifski STATIC IMPORTED GLOBAL)
set_target_properties(libgifski
PROPERTIES
    IMPORTED_LOCATION ${GIFSKI_RELEASE}
    IMPORTED_LOCATION_DEBUG ${GIFSKI_DEBUG}
)
target_include_directories(libgifski INTERFACE ${GIFSKI_ROOT_DIR})

target_link_libraries(libgifski
INTERFACE
    Userenv
    Ws2_32
)

add_dependencies(libgifski gifski)
