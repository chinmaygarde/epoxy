# This source file is part of Epoxy licensed under the MIT License.
# See LICENSE.md file for details.

if(__grammar)
  return()
endif()
set(__grammar INCLUDED)

function(add_lexer TARGET FLEX_FILE_PATH)
  if(WINDOWS)
    find_program (FLEX_PROGRAM win_flex)
  else()
    find_program (FLEX_PROGRAM flex PATHS "${EPOXY_FLEX_SEARCH_PATH}" NO_DEFAULT_PATH)
    find_program (FLEX_PROGRAM flex)
  endif()

  if(NOT FLEX_PROGRAM)
    message(FATAL_ERROR "Could not find flex (>=2.6.3) on the host.")
  endif()
  get_filename_component(FLEX_FILE_PATH ${FLEX_FILE_PATH} ABSOLUTE)
  add_custom_command(
    OUTPUT ${ARGN}
    COMMAND ${FLEX_PROGRAM} --noline ${FLEX_FILE_PATH}
    DEPENDS ${FLEX_FILE_PATH}
  )
  target_sources(${TARGET}
    PRIVATE
      ${ARGN}
  )
endfunction()

function(add_parser TARGET BISON_FILE_PATH)
  if(WINDOWS)
    find_program (BISON_PROGRAM win_bison)
  else()
    find_program (BISON_PROGRAM bison PATHS "${EPOXY_BISON_SEARCH_PATH}" NO_DEFAULT_PATH)
    find_program (BISON_PROGRAM bison)
  endif()

  if(NOT BISON_PROGRAM)
    message(FATAL_ERROR "Could not find bison (>=3.3.2) on the host.")
  endif()
  get_filename_component(BISON_FILE_PATH ${BISON_FILE_PATH} ABSOLUTE)
  add_custom_command(
    OUTPUT ${ARGN}
    COMMAND ${BISON_PROGRAM} -Werror ${BISON_FILE_PATH}
    DEPENDS ${BISON_FILE_PATH}
  )
  target_sources(${TARGET}
    PRIVATE
      ${ARGN}
  )
endfunction()
