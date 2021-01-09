include(${PROJECT_SOURCE_DIR}/cmake/add_pipl_rc.cmake)

add_library(aegif
SHARED
    src/entry_point.cpp
    src/aegp_mem.cpp
    src/aegp_mem.hpp
    src/aegp_mem_lock_guard.cpp
    src/aegp_mem_lock_guard.hpp
    src/aeio.cpp
    src/aeio.hpp
    src/catch_exception.hpp
    src/charenc.hpp
    src/error_guard.hpp
    src/gif_encoder.cpp
    src/gif_encoder.hpp
    src/global_setdown.cpp
    src/global_setdown.hpp
    src/global_setup.cpp
    src/global_setup.hpp
    src/locale.cpp
    src/locale.hpp
    src/log.cpp
    src/log.hpp
    src/output_options.hpp
    src/output_options.cpp
    src/options_dialog.cpp
    src/options_dialog.hpp
    src/scope_guard.hpp
    src/version.hpp

    # ui
    src/ui/ui_options_dialog_base.cpp
    src/ui/ui_options_dialog_base.hpp
)

if(WIN32)
add_pipl_rc(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/aegif_pipl.rc
    SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/aegif_pipl.r
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp
)

target_sources(aegif
PRIVATE
    src/charenc_win.cpp
    src/dll_hinstance.cpp
    src/dll_hinstance.hpp
    src/dll_main.cpp

    # ui
    src/ui/ui_options_dialog_win.cpp
    src/ui/ui_options_dialog_win.hpp

    # resources
    resource/options_dialog/resource.h
    resource/options_dialog/options_dialog.rc
    ${CMAKE_CURRENT_BINARY_DIR}/aegif_pipl.rc
)
endif()

target_include_directories(aegif
PRIVATE
    $<TARGET_PROPERTY:spdlog,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(aegif
PRIVATE
    $<$<CONFIG:Debug>:spdlog>
    $<$<CONFIG:RelWithDebInfo>:spdlog>
    libgifski
    mpack
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
    INTERPROCEDURAL_OPTIMIZATION_RELEASE True
)

# note: create shortcut to ${PROJECT_BINARY_DIR}/Plug-ins/ in your plugin directory
add_custom_command(TARGET aegif
POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/Plug-ins/"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:aegif>" "${PROJECT_BINARY_DIR}/Plug-ins/"
)

add_subdirectory(test)
