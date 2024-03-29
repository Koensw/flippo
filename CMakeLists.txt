cmake_minimum_required(VERSION 3.6)
project(flippo)
set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Wextra -Wno-narrowing -pedantic --std=c++17 ${CMAKE_CXX_FLAGS}")

add_subdirectory(player)

set(LAST_PLAYER klippo_v1)

set(PLAYERS
  klippo
  ${LAST_PLAYER}
  player2
  player3)

add_custom_target(run
  COMMAND cp "$<TARGET_FILE:klippo>" ${CMAKE_SOURCE_DIR}/bin/
  COMMAND ./competition.sh ${PLAYERS}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/
  DEPENDS klippo)
add_custom_target(run-comp
  COMMAND cp "$<TARGET_FILE:klippo>" ${CMAKE_SOURCE_DIR}/bin/
  COMMAND ./competition.sh klippo ${LAST_PLAYER}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/
  DEPENDS klippo)

add_custom_target(check
  COMMAND cp "$<TARGET_FILE:klippo>" ${CMAKE_SOURCE_DIR}/bin/
  COMMAND ./caiaio
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/
  DEPENDS klippo)
