set(SINGLE_FILE "${CMAKE_BINARY_DIR}/flippo.cpp")

set(BLACKHOLE_SRCS
    util/rand.cpp
    game/board.cpp
    game/player.cpp
    game/strategy.cpp
    strats/simple.cpp
    strats/standard_mcts.cpp
    flippo.cpp)

SET(BLACKHOLE_HDRS
    util/rand.h
    game/player.h
    game/board.h
    mcts/node.h
    mcts/mcts.hpp
    game/strategy.h
    strats/simple.h
    strats/standard_mcts.h)

add_definitions(-DDEBUG)
add_executable(klippo ${BLACKHOLE_SRCS})

# SUBMISSION
set(LIST_FILE "${CMAKE_BINARY_DIR}/sources.lst")

file(WRITE ${LIST_FILE} "")
foreach(SRC ${BLACKHOLE_SRCS})
    file(APPEND ${LIST_FILE} "${CMAKE_SOURCE_DIR}/${SRC}\n")
endforeach()

function(PREPEND var prefix)
   set(listVar "")
   foreach(f ${ARGN})
      list(APPEND listVar "${prefix}/${f}")
   endforeach(f)
   set(${var} "${listVar}" PARENT_SCOPE)
endfunction()

PREPEND(COMPILE_FILES ${CMAKE_CURRENT_SOURCE_DIR} "${BLACKHOLE_HDRS};${BLACKHOLE_SRCS}")

add_custom_target(submission
                  COMMAND ${CMAKE_SOURCE_DIR}/tools/make_single_file.sh ${COMPILE_FILES} > ${SINGLE_FILE}.tmp
                  COMMAND python ${CMAKE_SOURCE_DIR}/tools/minifier.py ${SINGLE_FILE}.tmp > ${SINGLE_FILE}
                  COMMAND rm ${SINGLE_FILE}.tmp
                  COMMAND g++ -w -pipe -O2 -g --std=c++17 -lm ${SINGLE_FILE} 
                  COMMAND rm a.out
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# FORMAT
FIND_PROGRAM(CLANG_FORMAT NAMES "clang-format-6.0" "clang-format-5.0" "clang-format-4.0" "clang-format")
IF(CLANG_FORMAT)
    EXEC_PROGRAM(${CLANG_FORMAT} ${CMAKE_CURRENT_SOURCE_DIR} ARGS --version OUTPUT_VARIABLE CLANG_VERSION)
    STRING(REGEX REPLACE ".*([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" CLANG_MAJOR_VERSION ${CLANG_VERSION})

    IF((${CLANG_MAJOR_VERSION} GREATER "4") OR (${CLANG_MAJOR_VERSION} EQUAL "4"))
        MESSAGE(STATUS "Found ${CLANG_FORMAT}, adding formatting target")
        ADD_CUSTOM_TARGET(
            format
            COMMAND
            ${CLANG_FORMAT}
            -i
            -style=file
            ${COMPILE_FILES}
            COMMENT "Auto formatting of all source files"
        )
    ELSE()
        MESSAGE(STATUS "Could only find version ${CLANG_MAJOR_VERSION} of clang-format, but version >= 4 is required.")
    ENDIF()
ELSE()
    MESSAGE(STATUS "Could NOT find clang-format")
ENDIF()

                    
