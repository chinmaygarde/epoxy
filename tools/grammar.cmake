if(__grammar)
  return()
endif()
set(__grammar INCLUDED)

function(add_lexer TARGET FLEX_FILE_PATH)
  get_filename_component(FLEX_FILE_PATH ${FLEX_FILE_PATH} ABSOLUTE)
  add_custom_command(
    OUTPUT ${ARGN}
    COMMAND flex --noline ${FLEX_FILE_PATH}
    DEPENDS ${FLEX_FILE_PATH}
  )
  target_sources(${TARGET}
    PRIVATE
      ${ARGN}
  )
endfunction()

function(add_parser TARGET BISON_FILE_PATH)
  get_filename_component(BISON_FILE_PATH ${BISON_FILE_PATH} ABSOLUTE)
  add_custom_command(
    OUTPUT ${ARGN}
    COMMAND bison -Werror ${BISON_FILE_PATH}
    DEPENDS ${BISON_FILE_PATH}
  )
  target_sources(${TARGET}
    PRIVATE
      ${ARGN}
  )
endfunction()
