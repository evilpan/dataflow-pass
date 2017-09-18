project(cpo C)

add_library(runtime SHARED runtime.c)

set_target_properties(runtime PROPERTIES
    COMPILE_FLAGS "-fsanitize=dataflow -g -std=c11"
)

SET_TARGET_PROPERTIES(runtime PROPERTIES RULE_LAUNCH_LINK
    "${CMAKE_SOURCE_DIR}/cpo ${CMAKE_BINARY_DIR} <OBJECTS> --"
)
