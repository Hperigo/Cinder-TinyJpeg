if( NOT TARGET Cinder-TinyJpeg )
	get_filename_component( TINYJPEG_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )
	get_filename_component( CINDER_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE )

	add_library( Cinder-TinyJpeg ${TINYJPEG_SOURCE_PATH}/ImageTargetTinyJpeg.cpp )

	target_include_directories( Cinder-TinyJpeg PUBLIC "${TINYJPEG_SOURCE_PATH}" )
	target_include_directories( Cinder-TinyJpeg SYSTEM BEFORE PUBLIC "${CINDER_PATH}/include" )

	if( NOT TARGET cinder )
		    include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
		    find_package( cinder REQUIRED PATHS
		        "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
		        "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
	endif()
	target_link_libraries( Cinder-TinyJpeg PRIVATE cinder )

endif()
