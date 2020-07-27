
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(
    /wd4267
    /wd4065
    /wd4101
    /wd4005
  )
else()
  add_compile_options(
    -Wall
    -Werror
    -fdiagnostics-color=always
  )
endif()

if(WINDOWS)
  add_compile_definitions(NOMINMAX YY_NO_UNISTD_H _CRT_NONSTDC_NO_DEPRECATE)
endif(WINDOWS)

add_library(epoxy_lib
  STATIC
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
  PRIVATE
    epoxy_lib
)

add_executable(epoxy_unittests
  driver_unittests.cc
  sema_unittests.cc
  code_gen_unittests.cc
)

add_test(epoxy_unittests epoxy_unittests)

target_link_libraries(epoxy_unittests
  PRIVATE
    epoxy_lib
    gtest
    gtest_main
)
