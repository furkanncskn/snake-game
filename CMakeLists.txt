cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(snakeGame VERSION 1.0.0)

include_directories(${PROJECT_SOURCE_DIR}/inc) 
include_directories(${PROJECT_SOURCE_DIR}/src) 

set(
    SOURCES
  	main.c
	  src/snake_game.c
    inc/snake_game.h
	)

add_executable(game ${SOURCES})
