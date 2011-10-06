# This is the main CMakeLists.txt for project MAGE
#
# Copyright UMons - 2011
#
# Written by alexis


#Project generalities
PROJECT(MAGE_PROJECT)
SET(MAGE_PROJECT_VERSION_MAJOR 1)
SET(MAGE_PROJECT_VERSION_MINOR 0beta)
SET(MAGE_PROJECT_VERSION "${MAGE_PROJECT_VERSION_MAJOR}.${MAGE_PROJECT_VERSION_MINOR}")
SET(MAGE_HDR_INSTALL_SUBPATH mage) # install the header files in /usr/local/include/${MAGE_HDR_INSTALL_SUBPATH}
SET(MAGE_LIB_INSTALL_SUBPATH "") # install the lib(s) in /usr/local/lib/${MAGE_LIB_INSTALL_SUBPATH}
SET(MAGE_SRC mage)
SET(MAGE_SRC_SUBDIRS thread htslib phtslib magelib)
SET(MAGE_EXAMPLES examples)
SET(MAGE_DATA data)

# lib infos
SET(MAGE_LIB_NAME MAGE)
SET(PHTS_LIB_NAME pHTSEngine)
SET(HTS_LIB_NAME HTSEngine)
SET(THREAD_LIB_NAME ringbuffer)

#CMake generalities
CMAKE_MINIMUM_REQUIRED (VERSION 2.6.2)

IF(COMMAND cmake_policy)
	IF(${CMAKE_VERSION} STRLESS "2.8.0")
		#AM: don't know which is the best, but must be < CMP0008 for cmake 2.6.0 and < CMP0012 for cmake 2.6.4
		cmake_policy(SET CMP0003 NEW) 
	ELSE()
		cmake_policy(SET CMP0014 NEW)
	ENDIF()
ENDIF()

#build options
OPTION(BUILD_EXAMPLES "Build executables in 'examples' directory" ON)
OPTION(BUILD_SHARED_LIBS "Build Shared libraries" ON)
OPTION(BUILD_IN_BIN "Build libraries in a bin folder" ON)
OPTION(MAGE_REACTIVE "Make MAGE Reactive" ON)
OPTION(MAKE_ONE_LIB "Build everything into one single lib (instead of four)" ON)

IF(BUILD_IN_BIN)
	MESSAGE("Files will be built in ${PROJECT_SOURCE_DIR}/bin")
	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ENDIF()

IF(MAGE_REACTIVE)
	MESSAGE("MAGE will be REACTIVE :)")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMAGE_REACTIVE")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMAGE_REACTIVE")
ENDIF()

# OS-SPECIFIC SUBPROJECTS COMPILATION (AM: IIRC I put this because of a compilation bug on x64 platform)
IF(UNIX)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

# Build type (default is Release)
# NB: Release mode defines a macro NDEBUG and sets the optimization flag to -O3 (i.e. gcc -DNDEBUG -O3)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF()

#build the mage library
ADD_SUBDIRECTORY(${MAGE_SRC})

#build the examples
IF(BUILD_EXAMPLES)
	MESSAGE("Examples will be built:")
	FOREACH(VAR ${MAGE_SRC_SUBDIRS})
		INCLUDE_DIRECTORIES(${MAGE_SRC}/${VAR}) # add thread, htsengine, phtsengine and magelib to the path
	ENDFOREACH()
	#also note that each file examples/foobar/CMakelists.txt links its example only to libMAGE (which in turn is/isn't linked to libHTS, libpHTS and libringbuffer, depending on MAKE_ONE_LIB)
	ADD_SUBDIRECTORY(examples)
ENDIF()
