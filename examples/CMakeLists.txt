#List the files in "examples" and if there are directories, add them to the list of (potential) examples

FILE(GLOB DIRLIST * )
SET(FLAG_EXAMPLE_FOUND 0)
FOREACH(PLUGINDIR ${DIRLIST})
	IF(IS_DIRECTORY ${PLUGINDIR})
		SET(FLAG_EXAMPLE_FOUND 1)
	    ADD_SUBDIRECTORY(${PLUGINDIR})
	ENDIF()
ENDFOREACH()

IF(${FLAG_EXAMPLE_FOUND} EQUAL 0)
	MESSAGE("No example found")
ENDIF()

