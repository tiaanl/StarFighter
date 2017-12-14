
set(JUNCTIONS_ROOT_DIR "../junctions" CACHE PATH "Junctions source root directory")
if(NOT JUNCTIONS_ROOT_DIR)
  message(FATAL_ERROR "Junctions not found.  Please set JUNCTIONS_ROOT_DIR.")
endif()

add_subdirectory("${JUNCTIONS_ROOT_DIR}" "junctions")
