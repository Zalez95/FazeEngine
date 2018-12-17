include(ExternalProject)

# Download the project
ExternalProject_Add(SoilDownload
	DOWNLOAD_COMMAND	git submodule update --init "${EXTERNAL_PATH}/soil"
	SOURCE_DIR			"${EXTERNAL_PATH}/soil"
	INSTALL_DIR			"${EXTERNAL_INSTALL_PATH}/soil"
	CMAKE_ARGS			-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
						-DCMAKE_BUILD_TYPE=$<CONFIG>
						-DCMAKE_DEBUG_POSTFIX=${MY_DEBUG_POSTFIX}
						-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
						-DCMAKE_C_FLAGS=$<$<BOOL:${FORCE_STATIC_VCRT}>:/MT>
						-DCMAKE_C_FLAGS_RELEASE=$<$<BOOL:${FORCE_STATIC_VCRT}>:/MT>
						-DCMAKE_C_FLAGS_DEBUG=$<$<BOOL:${FORCE_STATIC_VCRT}>:/MTd>
						--no-warn-unused-cli
)

# Get the properties from the downloaded target
ExternalProject_Get_Property(SoilDownload INSTALL_DIR)

set(SOIL_FOUND TRUE)
set(SOIL_INCLUDE_DIR "${INSTALL_DIR}/include/")
set(SOIL_LIBRARY_DIR "${INSTALL_DIR}/lib/")
if(BUILD_SHARED_LIBS)
	set(SOIL_LIBRARY "${CMAKE_SHARED_LIBRARY_PREFIX}SOIL${CMAKE_SHARED_LIBRARY_SUFFIX}")
	set(SOIL_DEBUG_LIBRARY "${CMAKE_SHARED_LIBRARY_PREFIX}SOIL${MY_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
	set(SOIL_LIBRARY "${CMAKE_STATIC_LIBRARY_PREFIX}SOIL${CMAKE_STATIC_LIBRARY_SUFFIX}")
	set(SOIL_DEBUG_LIBRARY "${CMAKE_STATIC_LIBRARY_PREFIX}SOIL${MY_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

# Create the target and add its properties
add_library(Soil INTERFACE)
target_include_directories(Soil INTERFACE ${SOIL_INCLUDE_DIR})
target_link_libraries(Soil INTERFACE
	optimized "${SOIL_LIBRARY_DIR}${SOIL_LIBRARY}"
	debug "${SOIL_LIBRARY_DIR}${SOIL_DEBUG_LIBRARY}"
)
add_dependencies(Soil SoilDownload)
