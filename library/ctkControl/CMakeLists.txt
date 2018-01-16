#############################################################
# sources
#PROJECT(ctkControl)
SET(PROJECT_NAME ctkControl)

SET(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/control.cpp
)

SET(HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/control.h
	
	
)

#设置MOC相关变量
SET(MOC_HDRS
	
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RootProjectInclude}
					${RootProjectInclude}/QsLog
					${VLD_INCLUDE}
					)

#设置为非console工程 WIN32为关键字
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#设置动态库dll生成路径
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/procpe/sys)

#查找以来的库
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${VLD_LIB}
  ctkLogger
)

#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin/
  LIBRARY DESTINATION bin/)
		