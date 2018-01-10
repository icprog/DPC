PROJECT(DataCollection)

# cpp 文件
SET(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/DataCollection.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sourceset.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pathset.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pathxml.cpp

)

# .h文件
SET(HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/DataCollection.h
	${CMAKE_CURRENT_SOURCE_DIR}/sourceset.h
	${CMAKE_CURRENT_SOURCE_DIR}/pathset.h
	${CMAKE_CURRENT_SOURCE_DIR}/pathxml.h
)

SET(MOCS
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/DataCollection.h
	${CMAKE_CURRENT_SOURCE_DIR}/sourceset.h
	${CMAKE_CURRENT_SOURCE_DIR}/pathset.h
	${CMAKE_CURRENT_SOURCE_DIR}/pathxml.h
)

SET(UI ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
		${CMAKE_CURRENT_SOURCE_DIR}/sourceSet.ui
		${CMAKE_CURRENT_SOURCE_DIR}/pathSet.ui)
	

QT4_WRAP_CPP(MOC_SRCS  ${MOCS})

QT4_WRAP_UI(UI_H ${UI})

INCLUDE_DIRECTORIES(
					${CTK_INCLUDE_DIRS}					
					${VLD_INCLUDE}
					${RadarProjectInclude}
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/org.common.filesystemwatcher
					${RadarProjectInclude}/org.radar.dataclear
					${CMAKE_CURRENT_BINARY_DIR}/
				)

#设置需要翻译的源文件				
#SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${HDRS} ${SRCS} ${MOC_SRCS} ${UI_H} ${RESOUCES} PARENT_SCOPE)

LINK_DIRECTORIES(${RadarProjectLib})

# Qt4和Qt5的解压缩函数名有区别
IF (WITH_QT5)
    ADD_DEFINITIONS("-DZ_PREFIX")
ENDIF (WITH_QT5)

ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${HDRS} ${SRCS} ${MOC_SRCS} ${UI} ${RESOUCES})
LINK_DIRECTORIES(${RadarProjectLib})

IF (WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${QT_QTMAIN_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTUITOOLS_LIBRARY}
		${QT_QTXML_LIBRARY}
        ${VLD_LIB}
        ctkLogger
        ctkPublicFun
        CTKPluginFramework
    )
ELSE()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${QT_QTMAIN_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTUITOOLS_LIBRARY}
		${QT_QTXML_LIBRARY}
        ${VLD_LIB}
        ctkLogger
        ctkPublicFun
        CTKPluginFramework
        ctkPluginManager    #LINUX下依赖
    )
ENDIF()


INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin)
