#############################################################
# sources
SET(PROJECT_NAME meteoGisMosaicProvider)

SET(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/mosaicdataprovider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mosaicdata.cpp
)

SET(HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/mosaicdataprovider.h
	${CMAKE_CURRENT_SOURCE_DIR}/mosaicdata.h
	
)

#设置MOC相关变量
SET(MOC_HDRS

)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/QGis
					${RadarProjectInclude}/3dpart/QGis/geos
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/org.radar.dataadapter
					${RadarProjectInclude}/org.meteogis.core
					${VLD_INCLUDE}
					)



#设置为非console工程 WIN32为关键字
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

# 设置动态库dll生成路径
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/gisplugins)
SET_TARGET_PROPERTIES(meteoGisMosaicProvider PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/gisplugins)

ADD_DEFINITIONS("-DCORE_EXPORT=${DLLIMPORT}")
ADD_DEFINITIONS("-DGUI_EXPORT=${DLLIMPORT}")
ADD_DEFINITIONS("-DAPP_EXPORT=${DLLIMPORT}")

#查找依赖的库
IF (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${QT_QTMAIN_LIBRARY}
		${QT_QTCORE_LIBRARY}
		${QT_QTGUI_LIBRARY}
		${QT_QTXML_LIBRARY}
		${QT_QTSQL_LIBRARY}
		${QT_UITOOLS_LIBRARY}
		${VLD_LIB}	#VLD内存泄露检查
		ctkLogger
		ctkPublicFun
		${RadarProjectLib}/qgis_core.lib
		
	)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${QT_QTMAIN_LIBRARY}
		${QT_QTCORE_LIBRARY}
		${QT_UITOOLS_LIBRARY}
		${VLD_LIB}
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libctkLogger.so	#需要明确路径
	)
ENDIF (WIN32)

#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/gisplugins/
		LIBRARY DESTINATION bin/gisplugins/)
