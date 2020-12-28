# OUTPUT : output (*.rc) path
# SOURCE : input (*.r) path
# INCLUDE_DIRS : additional include directories (option)
# DEPENDS : addtional dependent files (option)
function(add_pipl_rc)
    cmake_parse_arguments(ADD_PIPL_RC "" "OUTPUT;SOURCE" "INCLUDE_DIRS;DEPENDS" ${ARGN})
    set(PIPL_R ${ADD_PIPL_RC_SOURCE})
    set(PIPL_RC ${ADD_PIPL_RC_OUTPUT})
    get_filename_component(PIPL ${PIPL_R} NAME_WE)
    get_filename_component(OUT_DIR ${PIPL_RC} DIRECTORY)
    set(AE_SDK_DIR ${PROJECT_SOURCE_DIR}/3rd/AfterEffectsSDK/Examples)
    set(PIPL_TOOL ${AE_SDK_DIR}/Resources/PiPLTool.exe)
    set(PIPL_RR ${OUT_DIR}/${PIPL}.rr)
    set(PIPL_RRC ${OUT_DIR}/${PIPL}.rrc)
    set(PIPL_R_INCLUDE_DIRS ${AE_SDK_DIR}/Headers ${ADD_PIPL_RC_INCLUDE_DIRS})

    add_custom_command(OUTPUT ${PIPL_RC} ${PIPL_RR} ${PIPL_RRC}
        MAIN_DEPENDENCY ${PIPL_R}
        COMMAND "${CMAKE_CXX_COMPILER};/I;$<JOIN:${PIPL_R_INCLUDE_DIRS},;/I;>;/EP;${PIPL_R};>;${PIPL_RR}" COMMAND_EXPAND_LISTS
        COMMAND ${PIPL_TOOL} ${PIPL_RR} ${PIPL_RRC}
        COMMAND ${CMAKE_CXX_COMPILER} /EP /D MSWindows ${PIPL_RRC} > ${PIPL_RC}
        DEPENDS ${PIPL_TOOL} ${ADD_PIPL_RC_DEPENDS}
    )
endfunction()
