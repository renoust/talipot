TALIPOT_INSTALL_PYTHON_FILES("" ${CMAKE_CURRENT_SOURCE_DIR}/talipotplugins.py)

# talipotplugins.py must be found by python tests and doc
# but it cannot be compiled in the its source dir
# so we will copy it in the current binary dir
ADD_CUSTOM_TARGET(copyTalipotPluginsPyInBuild ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/talipotplugins.py ${CMAKE_CURRENT_BINARY_DIR}/talipotplugins.py)

IF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_CUSTOM_TARGET(copyTalipotPluginsPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/talipotplugins.py ${TALIPOT_PYTHON_FOLDER}/../talipotplugins/__init__.py)
  ADD_DEPENDENCIES(wheel copyTalipotPluginsPy)
ENDIF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
