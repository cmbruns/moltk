
find_program(PYSIDE_UIC_EXE pyside-uic)
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/moltk_ui.py"
    COMMAND "${PYSIDE_UIC_EXE}"
    ARGS moltk.ui --output=moltk_ui.py
    DEPENDS moltk.ui
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating moltk_ui.py"
)
add_custom_target(MoltkUi ALL
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/moltk_ui.py"
    SOURCES moltk.ui)
