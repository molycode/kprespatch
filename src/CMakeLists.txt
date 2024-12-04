set(KP_PROJECT_NAME "kprespatch")
message(STATUS "KP_PROJECT_NAME = ${KP_PROJECT_NAME}")
set(SRC_FOLDER "${CMAKE_SOURCE_DIR}/src")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_executable(${KP_PROJECT_NAME})
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${KP_PROJECT_NAME})
	target_link_libraries(${KP_PROJECT_NAME} PRIVATE "Comctl32.lib" "Shlwapi.lib")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_library(${KP_PROJECT_NAME})
	# todo (not really for this project though :))
else()
	message(FATAL_ERROR "Unsupported platform! '${CMAKE_SYSTEM_NAME}'")
endif()

ApplyCommonCompilerAndLinkerFlags(${KP_PROJECT_NAME})

set(HEADER_FILES
	"hexutils.h"
	"main.h"
	"md5.h"
	"resource.h"
	"targetver.h"
)

set(SOURCE_FILES
	"checkkingpinfile.c"
	"checkkpresolution.c"
	"checkmd5file.c"
	"hexutils.c"
	"main.c"
	"md5.c"
	"patchkingpinfile.c"
	"patchkpresolution.c"
)

set(RESOURCE_FILES
	"main.ico"
	"main.rc"
	"small.ico"
)

source_group("Header" FILES ${HEADER_FILES})
source_group("Source" FILES ${SOURCE_FILES})
source_group("Resource" FILES ${RESOURCE_FILES})

target_sources(
		${KP_PROJECT_NAME}
	PRIVATE
		${HEADER_FILES}
	PRIVATE
		${SOURCE_FILES}
	PRIVATE
		${RESOURCE_FILES}
)
