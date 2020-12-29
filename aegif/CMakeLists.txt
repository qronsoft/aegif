include(${PROJECT_SOURCE_DIR}/cmake/add_pipl_rc.cmake)

add_pipl_rc(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/aegif.rc
    SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/aegif.r
)

add_library(aegif
SHARED
    ${CMAKE_CURRENT_BINARY_DIR}/aegif.rc
    src/aegif.cpp
    src/log.hpp
    src/log.cpp
    src/charenc.hpp
    src/charenc.cpp
    src/scopeguard.hpp
    src/error_guard.hpp
    src/catch_exception.hpp
    src/global_setup.hpp
    src/global_setup.cpp
    src/global_setdown.hpp
    src/global_setdown.cpp
    src/aeio.hpp
    src/aeio.cpp
)

target_include_directories(aegif
PRIVATE
    $<TARGET_PROPERTY:spdlog,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(aegif
PRIVATE
    $<$<CONFIG:Debug>:spdlog>
    $<$<CONFIG:RelWithDebInfo>:spdlog>
    libgifski
    AfterEffectsSDK
)

target_compile_definitions(aegif
PRIVATE
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
    $<$<CONFIG:Debug>:_AEGIF_ENABLE_LOG>
    $<$<CONFIG:RelWithDebInfo>:_AEGIF_ENABLE_LOG>
    $<$<CONFIG:Debug>:_AEGIF_USE_OS_CONSOLE_LOG>
    $<$<CONFIG:RelWithDebInfo>:_AEGIF_USE_OS_CONSOLE_LOG>
)

set_target_properties(aegif
PROPERTIES
    SUFFIX ".aex"
)

# note: create shortcut to ${PROJECT_BINARY_DIR}/Plug-ins/ in your plugin directory
add_custom_command(TARGET aegif
POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/Plug-ins/"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:aegif>" "${PROJECT_BINARY_DIR}/Plug-ins/"
)
