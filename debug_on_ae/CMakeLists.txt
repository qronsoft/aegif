get_filename_component(AE2018_INST_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Adobe\\After Effects\\15.0;InstallPath]" ABSOLUTE PATH)
get_filename_component(AE2019_INST_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Adobe\\After Effects\\16.0;InstallPath]" ABSOLUTE PATH)
get_filename_component(AE2020_INST_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Adobe\\After Effects\\17.0;InstallPath]" ABSOLUTE PATH)
get_filename_component(AEBETA_INST_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Adobe\\After Effects (Beta)\\17.0;InstallPath]" ABSOLUTE PATH)

set(AE2018_EXE_PATH "${AE2018_INST_PATH}/AfterFX.exe")
set(AE2019_EXE_PATH "${AE2019_INST_PATH}/AfterFX.exe")
set(AE2020_EXE_PATH "${AE2020_INST_PATH}/AfterFX.exe")
set(AEBETA_EXE_PATH "${AEBETA_INST_PATH}/AfterFX (Beta).exe")

macro(debug_on name)
    add_custom_target(debug_on_${name})
    set_target_properties(debug_on_${name}
    PROPERTIES
        VS_DEBUGGER_COMMAND "${${name}_EXE_PATH}"
        VS_DEBUGGER_COMMAND_ARGUMENTS "-m"
    )
    add_dependencies(debug_on_${name} aegif)
endmacro()

debug_on(AE2018)
debug_on(AE2019)
debug_on(AE2020)
debug_on(AEBETA)
