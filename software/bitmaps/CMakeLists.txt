INSTALL(FILES logo.bmp welcomelogo.bmp
        DESTINATION ${TalipotBitmapInstallDir}
        COMPONENT talipot_app)

INSTALL(DIRECTORY samplePictures colorscales
        DESTINATION ${TalipotBitmapInstallDir}
        COMPONENT talipot_app
        PATTERN ".svn" EXCLUDE)
