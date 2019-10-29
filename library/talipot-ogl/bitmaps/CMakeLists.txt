ADD_SUBDIRECTORY(textures)

INSTALL(FILES fontb.ttf
              font.ttf
        DESTINATION ${TalipotBitmapInstallDir}
        COMPONENT talipot_ogl)

INSTALL(DIRECTORY fonts
        COMPONENT talipot_ogl
        DESTINATION ${TalipotBitmapInstallDir})
