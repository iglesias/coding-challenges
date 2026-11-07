add_library(morra_lib OBJECT game.cpp strategy.cpp input.cpp)
target_include_directories(morra_lib PUBLIC ${PROJECT_SOURCE_DIR}/inc)
set_target_properties(morra_lib PROPERTIES OUTPUT_NAME morra)
target_compile_options(morra_lib PRIVATE -Wall -Wconversion -Wextra -pedantic)
