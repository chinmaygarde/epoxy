
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(
    /W3
    /WX
    /wd4267
    /wd4305
    /wd4244
  )

  add_definitions(
    "-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS=1"
  )
else()
  add_compile_options(
    -Wall
    -Werror

    -Wno-deprecated-declarations
  )

  add_compile_options(
    -fdiagnostics-color=always
  )
endif()

if(WINDOWS)
  add_compile_definitions(NOMINMAX)
  add_compile_definitions(YY_NO_UNISTD_H)
endif(WINDOWS)

add_library(epoxy_lib
  code_gen.cc
  code_gen.h
  command_line.cc
  command_line.h
  driver.cc
  driver.h
  file.cc
  file.h
  macros.h
  scanner.cc
  scanner.h
  sema.cc
  sema.h
  types.cc
  types.h
  version.h
)

add_lexer(epoxy_lib
  epoxy.l
    lexer.cc
    lexer.h
)

add_parser(epoxy_lib
  epoxy.y
    location.hh
    parser.cc
    parser.h
    position.hh
    stack.hh
)

target_link_libraries(epoxy_lib
  inja
)

target_include_directories(epoxy_lib
  PUBLIC
    .
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(epoxy
  epoxy_main.cc
)

target_link_libraries(epoxy
  epoxy_lib
)

add_executable(epoxy_unittests
  driver_unittests.cc
  sema_unittests.cc
  code_gen_unittests.cc
)

target_link_libraries(epoxy_unittests
  epoxy_lib
  gtest
  gtest_main
)
