#############################################################
# sources
SET(PROJECT_NAME ctkPublicFun)

# 工程实现文件
SET(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/pathbuilder.cpp

)

# 工程头文件
SET(HDRS
	${RootProjectInclude}/ctkPublicFun/pathbuilder.h
)

#设置MOC相关变量
SET(MOC_HDRS
)

IF (WITH_QT5)
	ADD_DEFINITIONS("-DZ_PREFIX")
ENDIF (WITH_QT5)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RootProjectInclude}
					${RootProjectInclude}/ctkPublicFun
					${RootProjectInclude}/QsLog
					${VLD_INCLUDE}
					)
					
#设置需要翻译的源文件				
#SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${SRCS} ${HDRS} PARENT_SCOPE)				

#设置为非console工程 WIN32为关键字
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#查找依赖的库
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${QT_QTMAIN_LIBRARY}
	${QT_QTCORE_LIBRARY}
	${QT_UITOOLS_LIBRARY}
	${VLD_LIB}	#VLD内存泄露检查
	ctkLogger
)


#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin)
