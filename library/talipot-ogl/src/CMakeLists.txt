SET(talipot-ogl_LIB_SRCS
  AbstractGlCurve.cpp
  AroundTexturedSphere.cpp
  Camera.cpp
  CubeOutLined.cpp
  Curves.cpp
  edgeExtremityArrow.cpp
  EdgeExtremityGlyph.cpp
  EdgeExtremityGlyphManager.cpp
  Gl2DRect.cpp
  GlAbstractPolygon.cpp
  GlAxis.cpp
  GlBezierCurve.cpp
  GlBoundingBoxSceneVisitor.cpp
  GlBox.cpp
  GlCatmullRomCurve.cpp
  GlCircle.cpp
  GlColorScale.cpp
  GlComplexPolygon.cpp
  GlComposite.cpp
  GlConvexGraphHull.cpp
  GlConvexHull.cpp
  GlCPULODCalculator.cpp
  GlCubicBSplineInterpolation.cpp
  GlCurve.cpp
  GlEdge.cpp
  GlGlyphRenderer.cpp
  GlGraphComposite.cpp
  GlGraphRenderer.cpp
  GlGraphHighDetailsRenderer.cpp
  GlGraphLowDetailsRenderer.cpp
  GlGraphInputData.cpp
  GlGraphRenderingParameters.cpp
  GlGraphStaticData.cpp
  GlGrid.cpp
  GlHexagon.cpp
  GlLabel.cpp
  GlLayer.cpp
  GlLine.cpp
  GlLines.cpp
  GlMetaNodeRenderer.cpp
  GlNode.cpp
  GlNominativeAxis.cpp
  GlOpenUniformCubicBSpline.cpp
  GlPentagon.cpp
  GlPolygon.cpp
  GlPolyQuad.cpp
  GlProgressBar.cpp
  GlQuad.cpp
  GlQuadTreeLODCalculator.cpp
  GlQuantitativeAxis.cpp
  GlRect.cpp
  GlRegularPolygon.cpp
  GlScene.cpp
  GlSceneObserver.cpp
  GlSceneZoomAndPan.cpp
  GlSelectSceneVisitor.cpp
  GlShaderProgram.cpp
  GlSimpleEntity.cpp
  GlSphere.cpp
  GlStar.cpp
  GlTextureManager.cpp
  GlTools.cpp
  GlTriangle.cpp
  GlVertexArrayManager.cpp
  GlXMLTools.cpp
  Glyph.cpp
  GlyphManager.cpp
  OpenGlConfigManager.cpp
)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude} ${PROJECT_SOURCE_DIR} ${FTGLInclude} ${FREETYPE_INCLUDE_DIRS} ${UTF8CppInclude})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${StbInclude} ${Tess2Include})

ADD_LIBRARY (${LibTalipotOGLName} SHARED ${talipot-ogl_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTalipotOGLName} ${FTGLLibrary} ${GLEW_LIBRARY}
                      ${LibTalipotCoreName} ${OPENGL_gl_LIBRARY}
                      ${FREETYPE_LIBRARY} ${Tess2Library})

INSTALL(TARGETS ${LibTalipotOGLName}
  RUNTIME DESTINATION ${TalipotBinInstallDir} COMPONENT talipot_ogl
  LIBRARY DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_ogl
  ARCHIVE DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_ogl)
