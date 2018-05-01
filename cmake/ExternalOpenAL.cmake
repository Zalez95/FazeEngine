include(ExternalProject)

# Download the project
ExternalProject_Add(openalDownload
	DOWNLOAD_COMMAND	git submodule update --init "${EXTERNAL_PATH}/openal"
	SOURCE_DIR			"${EXTERNAL_PATH}/openal"
	INSTALL_DIR			"${EXTERNAL_INSTALL_PATH}/openal"
	CMAKE_ARGS			-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
						-DCMAKE_BUILD_TYPE=$<CONFIG>
						-DCMAKE_DEBUG_POSTFIX=${MY_DEBUG_POSTFIX}
						-DLIBTYPE=$<$<BOOL:${BUILD_SHARED_LIBS}>:SHARED>$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:STATIC>
						-DALSOFT_UTILS=OFF
						-DALSOFT_NO_CONFIG_UTIL=ON
						-DALSOFT_EXAMPLES=OFF
						-DALSOFT_TESTS=OFF
)

# Get the properties from the downloaded target
ExternalProject_Get_Property(openalDownload INSTALL_DIR)

set(OPENAL_FOUND TRUE)
set(OPENAL_INCLUDE_DIR "${INSTALL_DIR}/include")
set(OPENAL_LIBRARY_DIR "${INSTALL_DIR}/lib/")
IF(NOT WIN32)
    SET(openalLibName "openal")
ELSE()
    SET(openalLibName "OpenAL32")
ENDIF()
if(BUILD_SHARED_LIBS)
	set(OPENAL_LIBRARY "${CMAKE_SHARED_LIBRARY_PREFIX}${openalLibName}${CMAKE_SHARED_LIBRARY_SUFFIX}")
	set(OPENAL_DEBUG_LIBRARY "${CMAKE_SHARED_LIBRARY_PREFIX}${openalLibName}${MY_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
	set(OPENAL_LIBRARY "${CMAKE_STATIC_LIBRARY_PREFIX}${openalLibName}${CMAKE_STATIC_LIBRARY_SUFFIX}")
	set(OPENAL_DEBUG_LIBRARY "${CMAKE_STATIC_LIBRARY_PREFIX}${openalLibName}${MY_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

# Create the target and add its properties
add_library(openal INTERFACE)
target_include_directories(openal INTERFACE ${OPENAL_INCLUDE_DIR})
target_link_libraries(openal INTERFACE
	optimized "${OPENAL_LIBRARY_DIR}${OPENAL_LIBRARY}"
	debug "${OPENAL_LIBRARY_DIR}${OPENAL_DEBUG_LIBRARY}"
)
add_dependencies(openal openalDownload)