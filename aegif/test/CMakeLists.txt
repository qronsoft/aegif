get_target_property(aegif_SOURCE_DIR aegif SOURCE_DIR)

add_executable(aegif_test
    ${aegif_SOURCE_DIR}/src/output_options.hpp
    ${aegif_SOURCE_DIR}/src/output_options.cpp

    src/output_options_test.cpp
    src/frozen/output_options_v1.hpp
    src/frozen/output_options_v1.cpp
    src/frozen/output_options_v255.hpp
    src/frozen/output_options_v255.cpp
)

target_include_directories(aegif_test
PRIVATE
    ${aegif_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(aegif_test
PRIVATE
    spdlog
    gtest_main
    mpack
    AfterEffectsSDK
)

include(GoogleTest)
gtest_add_tests(TARGET aegif_test)
