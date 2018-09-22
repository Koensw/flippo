set(SINGLE_FILE "${CMAKE_BINARY_DIR}/flippo.cpp")

set(BLACKHOLE_SRCS
    util/rand.cpp
    game/board.cpp
    game/player.cpp
    flippo.cpp)

SET(BLACKHOLE_HDRS
    util/rand.h
    game/player.h
    game/board.h)

add_definitions(-DDEBUG)
add_executable(klippo ${BLACKHOLE_SRCS})

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
                    
