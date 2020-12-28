set(AE_SDK_ROOT ${PROJECT_SOURCE_DIR}/3rd/AfterEffectsSDK/Examples)

add_library(AfterEffectsSDK STATIC
    ${AE_SDK_ROOT}/Util/AEGP_SuiteHandler.cpp
    ${AE_SDK_ROOT}/Util/AEGP_Utils.cpp
    ${AE_SDK_ROOT}/Util/MissingSuiteError.cpp
)

target_include_directories(AfterEffectsSDK PUBLIC
    ${AE_SDK_ROOT}/Headers
    ${AE_SDK_ROOT}/Headers/SP
    ${AE_SDK_ROOT}/Util
)
