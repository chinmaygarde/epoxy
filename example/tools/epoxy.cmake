if(__epoxy)
  return()
endif()
set(__epoxy INCLUDED)

function(epoxy TARGET EPOXY_TEMPLATE_PATH EPOXY_IDL_PATH OUTPUT_FILE_NAME)
  find_program (EPOXY_PROGRAM epoxy)
  if(NOT EPOXY_PROGRAM)
    message(FATAL_ERROR "Could not find flex on the host in the default search path.")
  endif()

  get_filename_component(EPOXY_TEMPLATE_PATH ${EPOXY_TEMPLATE_PATH} ABSOLUTE)
  get_filename_component(EPOXY_IDL_PATH ${EPOXY_IDL_PATH} ABSOLUTE)

  set(OUTPUT_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/gen/${OUTPUT_FILE_NAME}")

  add_custom_command(
    OUTPUT "${OUTPUT_FILE_PATH}"
    COMMAND "${EPOXY_PROGRAM}" --output "${OUTPUT_FILE_PATH}" --idl "${EPOXY_IDL_PATH}" --template-file "${EPOXY_TEMPLATE_PATH}"
    DEPENDS "${EPOXY_PROGRAM}" "${EPOXY_IDL_PATH}" "${EPOXY_TEMPLATE_PATH}"
  )

  target_sources(${TARGET} PUBLIC "${OUTPUT_FILE_PATH}")

  target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/gen")
endfunction()
