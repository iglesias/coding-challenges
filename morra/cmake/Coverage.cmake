option(ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "Code Coverage: Enabling LLVM source-based coverage instrumentation")
        add_compile_options(-fprofile-instr-generate -fcoverage-mapping -O0 -g)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)

        if(APPLE)
            # FIXME: assumed that binaries are resolved via xcrun
            set(LLVM_COV      xcrun llvm-cov)
            set(LLVM_PROFDATA xcrun llvm-profdata)
        else()
            # FIXME: hardcoded to version 20
            find_program(LLVM_COV_PATH NAMES llvm-cov-20 llvm-cov REQUIRED)
            find_program(LLVM_PROFDATA_PATH
                NAMES llvm-profdata-20 llvm-profdata REQUIRED)

            set(LLVM_COV      ${LLVM_COV_PATH})
            set(LLVM_PROFDATA ${LLVM_PROFDATA_PATH})
        endif()

        if(LLVM_COV AND LLVM_PROFDATA)
            add_custom_target(ccov
                COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure

                COMMAND ${LLVM_PROFDATA} merge
                    -sparse
                    -o default.profdata *.profraw

                COMMAND ${LLVM_COV} report
                    $<TARGET_FILE:game_test>
                    -object $<TARGET_FILE:strategy_test>
                    -instr-profile=default.profdata
                    -ignore-filename-regex="googletest|_test.cpp"

                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Coverage summary:"
            )
        else()
            message(WARNING "llvm-cov and/or llvm-profdata not found. Coverage target 'ccov' will not be created.")
        endif()

    endif()
endif()
