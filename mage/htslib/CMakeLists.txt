FILE(GLOB SRC *.cpp *.c)
FILE(GLOB HDR *.hpp *.h)

ADD_LIBRARY(${HTS_LIB_NAME} ${SRC} ${HDR})
TARGET_LINK_LIBRARIES(${HTS_LIB_NAME} m)

SET_TARGET_PROPERTIES(${HTS_LIB_NAME} PROPERTIES
                     SOVERSION ${MAGE_PROJECT_VERSION_MAJOR}
                     VERSION ${MAGE_PROJECT_VERSION})

IF(BUILD_SHARED_LIBS)
	INSTALL(TARGETS ${HTS_LIB_NAME} LIBRARY DESTINATION lib/${MAGE_LIB_INSTALL_SUBPATH})
ELSE()
	INSTALL(TARGETS ${HTS_LIB_NAME} ARCHIVE DESTINATION lib/${MAGE_LIB_INSTALL_SUBPATH})
ENDIF()

INSTALL(FILES ${HDR} DESTINATION include/${MAGE_HDR_INSTALL_SUBPATH})

