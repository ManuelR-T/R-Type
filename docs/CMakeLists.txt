find_package(Doxygen)
if (DOXYGEN_FOUND)

set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxy.conf)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)

add_custom_target(doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)

else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
