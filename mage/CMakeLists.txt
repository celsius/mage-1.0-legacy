# build the libraries or the library (depending on ${MAKE_ONE_LIB})
IF(MAKE_ONE_LIB)
	MESSAGE("MAGE will be built into a single library")
	#TODO make this a bit cleaner separate in variables like THREAD_HDR_FILES, THREAD_SRC_FILES, ... 
	#TODO and concatenate all of them into HDR and SRC

	#list all the .c and .h files in the four folders (+ add to the path)
	FOREACH(VAR ${MAGE_SRC_SUBDIRS})
		INCLUDE_DIRECTORIES(${VAR})
		FILE(GLOB SRC ${SRC} ${VAR}/*.cpp ${VAR}/*.c)
		FILE(GLOB HDR ${HDR} ${VAR}/*.hpp ${VAR}/*.h)
	ENDFOREACH()

	#build one lib with all these files
	ADD_LIBRARY(${MAGE_LIB_NAME} ${SRC} ${HDR})
	TARGET_LINK_LIBRARIES(${MAGE_LIB_NAME} m)

	SET_TARGET_PROPERTIES(${MAGE_LIB_NAME} PROPERTIES
                     SOVERSION ${MAGE_PROJECT_VERSION_MAJOR}
                     VERSION ${MAGE_PROJECT_VERSION})

	IF(BUILD_SHARED_LIBS)
		INSTALL(TARGETS ${MAGE_LIB_NAME} LIBRARY DESTINATION lib/${MAGE_LIB_INSTALL_SUBPATH})
	ELSE()
		INSTALL(TARGETS ${MAGE_LIB_NAME} ARCHIVE DESTINATION lib/${MAGE_LIB_INSTALL_SUBPATH})
	ENDIF()
	#by default header files are put in /usr/local/include/mage/
	#to use these files in another project, people will either
	# use #include <mage/MAGE.h>
	# or 
	# configure their search path to include /usr/local/include/mage/
	# (i.e. gcc -I/usr/local/include/mage, or CMake's INCLUDE_DIRECTORIES(/usr/local/include/mage) )
	# and then use #include <MAGE.h>
	INSTALL(FILES ${HDR} DESTINATION include/${MAGE_HDR_INSTALL_SUBPATH})
ELSE()
	MESSAGE("Each component of MAGE will be built in its own library")
	FOREACH(VAR ${MAGE_SRC_SUBDIRS})
		INCLUDE_DIRECTORIES(${VAR})
		ADD_SUBDIRECTORY(${VAR})
	ENDFOREACH()
ENDIF()

