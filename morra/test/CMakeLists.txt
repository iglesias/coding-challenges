option(ENABLE_TESTING "Enable unit tests" OFF)

if (ENABLE_TESTING)
  find_package(GTest QUIET)

  if (NOT GTest_FOUND)
    message(STATUS "System GTest not found - fetching via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        v1.17.0
      GIT_SHALLOW    TRUE
    )
    # Prevent GTest from overriding the project's compiler and linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
  endif()

  add_executable(game_test game_test.cpp)
  target_link_libraries(game_test gtest morra_lib)
  target_compile_options(game_test PRIVATE -Wall -Wconversion -Wextra -pedantic)
  add_test(NAME test_game COMMAND $<TARGET_FILE:game_test>)
  set_tests_properties(test_game
      PROPERTIES ENVIRONMENT
      "LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/prof_%p.profraw")

  add_executable(strategy_test strategy_test.cpp)
  target_link_libraries(strategy_test gtest morra_lib)
  target_compile_options(strategy_test PRIVATE -Wall -Wconversion -Wextra -pedantic)
  add_test(NAME test_strategy COMMAND $<TARGET_FILE:strategy_test>)
  set_tests_properties(test_strategy
      PROPERTIES ENVIRONMENT
      "LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/prof_%p.profraw")
endif()
