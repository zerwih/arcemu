#  Find the LibXML2 includes and library
#
#  LIBXML2_INCLUDE_DIR - where to find the headers.
#  LIBXML2_LIBRARIES   - List of libraries.
#  LIBXML2_FOUND       - True if found.
#

IF (LIBXML2_INCLUDE_DIR)
  # Already in cache, be silent
  SET(LIBXML2_FIND_QUIETLY TRUE)
ENDIF (LIBXML2_INCLUDE_DIR)

FIND_PATH(LIBXML2_INCLUDE_DIR tree.h parser.h xmlmemory.h
  /usr/local/include/libxml2/libxml
  /usr/include/libxml2/libxml
  /usr/local/libxml2/include/libxml
  /opt/local/include/libxml2/libxml
)

SET(LIBXML2_NAMES libxml2 xml2 )
FIND_LIBRARY(LIBXML2_LIBRARY
  NAMES ${LIBXML2_NAMES}
  PATHS /usr/lib /usr/lib/libxml2 /usr/local/lib /usr/local/libxml2/lib /usr/local/lib/libxml2 /opt/local/lib/libxml2/libxml
)

IF (LIBXML2_INCLUDE_DIR AND LIBXML2_LIBRARY)
  SET(LIBXML2_FOUND TRUE)
  SET( LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
ELSE (LIBXML2_INCLUDE_DIR AND LIBXML2_LIBRARY)
  SET(LIBXML2_FOUND FALSE)
  SET( LIBXML2_LIBRARIES )
ENDIF (LIBXML2_INCLUDE_DIR AND LIBXML2_LIBRARY)

IF (LIBXML2_FOUND)
  IF (NOT LIBXML2_FIND_QUIETLY)
    MESSAGE(STATUS "Found LIBXML2: ${LIBXML2_LIBRARY}")
  ENDIF (NOT LIBXML2_FIND_QUIETLY)
ELSE (LIBXML2_FOUND)
  IF (LIBXML2_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for LibXML2 libraries named ${LIBXML2_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find LIBXML2 library")
  ENDIF (LIBXML2_FIND_REQUIRED)
ENDIF (LIBXML2_FOUND)

MARK_AS_ADVANCED(
  LIBXML2_LIBRARY
  LIBXML2_INCLUDE_DIR
  )

