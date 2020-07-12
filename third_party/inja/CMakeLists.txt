add_library(inja
  INTERFACE
)

target_include_directories(inja
  INTERFACE
    .
)

target_link_libraries(inja
  INTERFACE
    json
)
