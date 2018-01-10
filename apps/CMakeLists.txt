set (BUILD_DPC_MANAGER FALSE CACHE BOOL "whether to build the manager projects")
set (BUILD_DPC_SERVER TRUE CACHE BOOL "whether to build the server projects")
set (BUILD_DPC_AGENT FALSE CACHE BOOL "whether to build the agent projects")
set (APPS
)

#开启单元测试功能
if (BUILD_DPC_MANAGER)
set(APPS
  ${APPS}
  manager
)
endif ()

IF (BUILD_DPC_SERVER)
	set (APPS
		${APPS}
		  server
	)
ENDIF ()

IF (BUILD_DPC_AGENT)
	set (APPS
		${APPS}
		  agent
	)
ENDIF ()

foreach(app ${APPS})
  option(BUILD_APP_${app} "Build the ${app} application" ON)
  add_subdirectory(${app})
endforeach()

#将需要翻译的文件，传递到外部
#SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} PARENT_SCOPE)

#综合显示客户端翻译文件
#SET(RADAR_MOSAIC_CLIENT_FILES_TO_TRANSLATE ${RADAR_MOSAIC_CLIENT_FILES_TO_TRANSLATE} PARENT_SCOPE)

#数据传输系统翻译文件
#SET(RADAR_DATATRANSFER_FILES_TO_TRANSLATE ${RADAR_DATATRANSFER_FILES_TO_TRANSLATE} PARENT_SCOPE)

#雷达后台处理加工系统翻译文件
#SET(RADAR_SERVER_V2_FILES_TO_TRANSLATE ${RADAR_SERVER_V2_FILES_TO_TRANSLATE} PARENT_SCOPE)

#消息服务翻译文件全局变量
#SET(RADAR_MESSAGECENTRE_FILES_TO_TRANSLATE ${RADAR_MESSAGECENTRE_FILES_TO_TRANSLATE} PARENT_SCOPE)
#message(STATUS "****${FILES_TO_TRANSLATE}")
