INSTALL(FILES logo.png welcomelogo.png
        DESTINATION ${TalipotBitmapInstallDir}
        COMPONENT talipot_app)

INSTALL(DIRECTORY colorscales
        DESTINATION ${TalipotBitmapInstallDir}
        COMPONENT talipot_app)
