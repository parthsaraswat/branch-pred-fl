#=========================================================================
# CMake script for Group03 Branch Predictor project
#=========================================================================
# To add new implementations or new tests, simply append your new file to
# the corresponding list.
# Author: Parth Saraswat
# Date:   May 09, 2020

set(SRC_FILES
  Pred3.cc
  Pred4.cc
)
set(DEMO_FILES
  single-unique.c
  multi-unique.c
)

set(TEST_MILESTONE_FILES
)

set(TEST_ALL_FILES
  ${TEST_MILESTONE_FILES}
  1L-bht-directed-test.cc
  2L-bht-directed-test.cc
)

set (EVAL_FILES

)

#=========================================================================
# !!!!DO NOT TOUCH THE FOLLOWING CODE!!!!
#=========================================================================

#-------------------------------------------------------------------------
# remove_extension
#-------------------------------------------------------------------------
# Removes the file extension. For example:
# remove_extension(IMPL, "pow-iter.c") stores "pow-iter" into IMPL.

function(remove_extension VARNAME FILE_NAME)
  string(REGEX REPLACE "\\.[^.]*$" "" NAME_WITHOUT_EXT ${FILE_NAME})
  set(${VARNAME} "${NAME_WITHOUT_EXT}" PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------
# Set up environment
#-------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)
enable_language(CXX)
enable_testing()
project(predictors)

# Set and cache the build type
set(default_build_type "debug")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "debug" "eval" )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()
message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}'")

# Flags for g++
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wvla -pedantic-errors -Werror -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g --coverage -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_EVAL "-O3 -g")

# Path to this PA's source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DEMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ubmark")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(EVAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/eval")

# Include path to this PA's header files
include_directories(${SRC_DIR})

#-------------------------------------------------------------------------
# Link source files into a library
#-------------------------------------------------------------------------

foreach(SRC_FILE ${SRC_FILES})
  set(LIB_SRCS ${LIB_SRCS} "${SRC_DIR}/${SRC_FILE}")
endforeach(SRC_FILE)

add_library(${PROJECT_NAME} ${LIB_SRCS})

#-------------------------------------------------------------------------
# Test targets
#-------------------------------------------------------------------------

add_custom_target(check COMMAND ctest "--timeout" "10")

foreach(TEST_FILE ${TEST_ALL_FILES})

  remove_extension(TEST_BIN ${TEST_FILE})
  set(TEST_SRC "${TEST_DIR}/${TEST_FILE}")

  # Add test
  add_executable(${TEST_BIN} EXCLUDE_FROM_ALL ${TEST_SRC})
  target_link_libraries(${TEST_BIN} m ${PROJECT_NAME}) # link the math and src library
  add_dependencies(check ${TEST_BIN})
  add_test(${TEST_BIN} ${TEST_BIN})

endforeach(TEST_FILE)

#-------------------------------------------------------------------------
# Milestone test targets
#-------------------------------------------------------------------------

add_custom_target(check-milestone COMMAND ctest "-L" "milestone" "--timeout" "10")

foreach(TEST_FILE ${TEST_MILESTONE_FILES})

  remove_extension(TEST_BIN ${TEST_FILE})
  add_dependencies(check-milestone ${TEST_BIN})
  set_tests_properties(${TEST_BIN} PROPERTIES LABELS "milestone")

endforeach(TEST_FILE)

#-------------------------------------------------------------------------
# Evaluation targets
#-------------------------------------------------------------------------

add_custom_target(eval)

foreach(EVAL_FILE ${EVAL_FILES})

  remove_extension(EVAL_BIN ${EVAL_FILE})
  set(EVAL_SRC "${EVAL_DIR}/${EVAL_FILE}")

  # Add eval
  add_executable(${EVAL_BIN} EXCLUDE_FROM_ALL ${EVAL_SRC})
  target_link_libraries(${EVAL_BIN} m ${PROJECT_NAME}) # link the math and src library
  add_dependencies(eval ${EVAL_BIN})

endforeach(EVAL_FILE)

#-------------------------------------------------------------------------
# Code coverage target
#-------------------------------------------------------------------------

add_custom_target(coverage
  COMMAND lcov --capture --directory . --output-file coverage.info
  COMMAND genhtml coverage.info --output-directory coverage-html
)

#-------------------------------------------------------------------------
# Mem check target
#-------------------------------------------------------------------------

add_custom_target(memcheck
  COMMAND echo ""
  COMMAND echo "========================================================="
  COMMAND echo " memcheck passed!"
  COMMAND echo "========================================================="
  COMMAND echo ""
)

foreach(TEST_FILE ${TEST_ALL_FILES})

  remove_extension(TEST_BIN ${TEST_FILE})
  add_custom_target(memcheck-${TEST_BIN}
    COMMAND mkdir -p "memcheck-log"
    COMMAND valgrind "--trace-children=yes"
                     "--leak-check=full"
                     "--error-exitcode=1"
                     "--undef-value-errors=no"
                     "--log-file=memcheck-log/${TEST_BIN}.log"
                     "${CMAKE_CURRENT_BINARY_DIR}/${TEST_BIN}"
                     ">" "memcheck-log/${test_bin}.stdout"
    COMMAND sed "-r" "\"s/\\x1B\\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g\""
                "memcheck-log/${test_bin}.stdout"
                ">" "memcheck-log/${test_bin}.stdout"
    DEPENDS ${TEST_BIN}
  )
  add_dependencies(memcheck memcheck-${TEST_BIN})

endforeach(TEST_FILE)
