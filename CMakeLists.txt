cmake_minimum_required(VERSION 3.17)
project(aegif)

# global settings
enable_testing()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# enable LTO on Release build
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# msvc global settings
if (MSVC)
    add_compile_definitions(
        # use Unicode charset
        UNICODE
        _UNICODE
        # disable min-max macros
        NOMINMAX
        # suppress security function warning
        _CRT_SECURE_NO_WARNINGS
        # reduce the size of the win32 header file
        WIN32_LEAN_AND_MEAN
        VC_EXTRALEAN
        # enable win32 strict
        STRICT
    )

    add_compile_options(
        # charcode warning
        /wd4566
        /wd4819
        # deprecated register keyword warning
        /wd5033
        # multi processor compile
        $<$<CXX_COMPILER_ID:MSVC>:/MP>
    )
endif()

# 3rd party libs
add_subdirectory(3rd/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(3rd/AuxCMakeLists/AfterEffectsSDK EXCLUDE_FROM_ALL)

# targets
add_subdirectory(aegif)
add_subdirectory(debug_on_ae)
