#指定CMake需要的最小版本
cmake_minimum_required(VERSION 2.8.11)

# ctk模板工程

#指定项目名
project(DPC)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(EXTERNAL_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmakeExternals)
set(EXTERNAL_PROJECT_FILE_PREFIX "")
include(ExternalProject)
include(ctkMacroCheckExternalProjectDependency)

#是否需要使用Qt5
SET (WITH_QT5 TRUE CACHE BOOL "use qt5")
SET (QT_VERSIONS)
IF (WITH_QT5)
	find_package(Qt5Core REQUIRED)		#查找并加载外部package；REQUIRED表示若没找到，cmake将终止
	find_package(Qt5Gui REQUIRED)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5Sql REQUIRED)
	find_package(Qt5Xml REQUIRED)
	find_package(Qt5Concurrent REQUIRED)
	find_package(Qt5UiTools REQUIRED)
	find_package(Qt5LinguistTools REQUIRED)
	find_package(Qt5Qml REQUIRED)
	find_package(Qt5Quick REQUIRED)
	find_package(Qt5QuickWidgets REQUIRED)
	find_package(Qt5OpenGl)
	find_package(Qt5Network)
	find_package(Qt5Concurrent)
	include(${CMAKE_MODULE_PATH}/ECMQt4To5Porting.cmake)
	ADD_DEFINITIONS("-DWITH_QT5")
	SET (QT_VERSIONS Qt5)
ELSE (WITH_QT5)
	find_package(Qt4 REQUIRED)
	include(${QT_USE_FILE})
	SET (QT_VERSIONS Qt4)
ENDIF (WITH_QT5)

# 并行计算
SET (WITH_TREC_OPENMP FALSE CACHE BOOL "calculate with openmp")
IF (WITH_TREC_OPENMP)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
ENDIF (WITH_TREC_OPENMP)

MESSAGE(STATUS "${QT5_WRAP_UI}")
#find_package(CTK REQUIRED)

# Qt翻译家
FIND_PROGRAM(QT_LRELEASE_EXECUTABLE
    NAMES lrelease
    PATHS $ENV{PATH}
    NO_DEFAULT_PATH
    )

# 编译版本设置
IF (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # mark_as_advanced(CMAKE_BUILD_TYPE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

#SET (BUILD_TYPE_DEBUG TRUE CACHE BOOL "Determines build version")
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(DEST_BUILD_VERSION Debug)
	SET(QT_SUFFIX d)
	
ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
	ADD_DEFINITIONS("-DCRASH_SILENTLY=1")
	SET(DEST_BUILD_VERSION Release)
	SET(QT_SUFFIX )
ENDIF()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# 设置导出宏
IF (WIN32)
  SET(DLLIMPORT "__declspec(dllimport)")
  SET(DLLEXPORT "__declspec(dllexport)")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
  SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
ELSE (WIN32)
  IF(PEDANTIC AND NOT APPLE)
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined")
  ENDIF(PEDANTIC AND NOT APPLE)

  SET(DLLIMPORT "")
  SET(DLLEXPORT "")
ENDIF(WIN32)

#是否开启内存泄露检查
SET (WITH_CHECK_MEM_LEAK FALSE CACHE BOOL "Check memory leak")
IF (WITH_CHECK_MEM_LEAK)
  FIND_PACKAGE(VLD)  
  SET(VLD_LIB ${VLD_LIBRARY})
  SET(VLD_INCLUDE ${VLD_INCLUDE_DIR})
  ADD_DEFINITIONS("-DWITH_MEM_CHECK")
ELSE (WITH_CHECK_MEM_LEAK)
  SET(VLD_LIB "")
  SET(VLD_INCLUDE "")
ENDIF (WITH_CHECK_MEM_LEAK)

#设置工程变量
SET(RootProject ${CMAKE_SOURCE_DIR})
SET(RootProjectInclude ${RadarProject}/include)
#SET(RadarProjectRunTimeLib ${RadarProject}/3dpart/${QT_VERSIONS}/bin/${DEST_BUILD_VERSION})
#SET(RadarProjectLib ${RadarProject}/3dpart/${QT_VERSIONS}/lib/${DEST_BUILD_VERSION})
#SET(GisPluginDir ${RadarProject}/gisplugins/${DEST_BUILD_VERSION})

#这段用来查找插件存放的目录，解决编译调试版本和最终发布版本插件路径不一致的问题
#此处不存储插件路径，而存储系统的构建路径
#SET(ctkPluginHomeDir ${CMAKE_BINARY_DIR}/bin/plugins/${DEST_BUILD_VERSION})
#文件操作命令
#FILE(WRITE ${CMAKE_BINARY_DIR}/bin/radarprojectbuildpath.txt "${CMAKE_BINARY_DIR}/bin/")

#待翻译的文件，定义全局变量
#SET(FILES_TO_TRANSLATE)

#综合显示客户端翻译文件变量
#SET(RADAR_MOSAIC_CLIENT_FILES_TO_TRANSLATE)

#数据传输客户端翻译文件变量
#SET(RADAR_DATATRANSFER_FILES_TO_TRANSLATE)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/library)	#基础依赖库

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/apps)		#应用

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/i18n) #国际化
