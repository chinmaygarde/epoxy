
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
endif(WINDOWS)

add_library(epoxy
  # main.cc
  driver.h
  driver.cc
  types.h
  types.cc
  macros.h
  scanner.h
  scanner.cc
  sema.h
  sema.cc
)

add_lexer(epoxy
  epoxy.l
    lexer.h
    lexer.cc
)

add_parser(epoxy
  epoxy.y
    location.hh
    stack.hh
    position.hh
    parser.h
    parser.cc
)

target_include_directories(epoxy
  PUBLIC
    .
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(epoxy_unittests
  driver_unittests.cc
)

target_link_libraries(epoxy_unittests
  epoxy
  gtest
  gtest_main
)

