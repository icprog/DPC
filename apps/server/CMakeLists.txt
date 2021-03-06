﻿PROJECT(Server)

# cpp 文件
SET(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/modulelist.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/systemlog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/threadlog.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/menu.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dllmaper.cpp

)

# .h文件
SET(HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/modulelist.h
	${CMAKE_CURRENT_SOURCE_DIR}/systemlog.h
	${CMAKE_CURRENT_SOURCE_DIR}/threadlog.h

	${CMAKE_CURRENT_SOURCE_DIR}/menu.h
	${CMAKE_CURRENT_SOURCE_DIR}/dllmaper.h
)

SET(MOCS
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/modulelist.h
	${CMAKE_CURRENT_SOURCE_DIR}/systemlog.h
	${CMAKE_CURRENT_SOURCE_DIR}/threadlog.h

	${CMAKE_CURRENT_SOURCE_DIR}/menu.h
	${CMAKE_CURRENT_SOURCE_DIR}/dllmaper.h

)

SET(UI ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
		${CMAKE_CURRENT_SOURCE_DIR}/modulelist.ui
		${CMAKE_CURRENT_SOURCE_DIR}/systemlog.ui
		${CMAKE_CURRENT_SOURCE_DIR}/threadlog.ui
		)
	

QT4_WRAP_CPP(MOC_SRCS  ${MOCS})

QT4_WRAP_UI(UI_H ${UI})

INCLUDE_DIRECTORIES(		
					${VLD_INCLUDE}
					${RootProjectInclude}
					${RootProjectInclude}/QsLog
					${RootProjectInclude}/ctkPublicFun
					${CMAKE_CURRENT_BINARY_DIR}/
				)

#设置需要翻译的源文件				
#SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${HDRS} ${SRCS} ${MOC_SRCS} ${UI_H} ${RESOUCES} PARENT_SCOPE)

# Qt4和Qt5的解压缩函数名有区别
IF (WITH_QT5)
    ADD_DEFINITIONS("-DZ_PREFIX")
ENDIF (WITH_QT5)

ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${HDRS} ${SRCS} ${MOC_SRCS} ${UI} ${RESOUCES})


TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${QT_QTMAIN_LIBRARY}
	${QT_QTCORE_LIBRARY}
	${QT_QTGUI_LIBRARY}
	${QT_QTUITOOLS_LIBRARY}
	${VLD_LIB}
	ctkLogger
	ctkPublicFun
)



INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin)
