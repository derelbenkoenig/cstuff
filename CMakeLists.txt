cmake_minimum_required(VERSION 3.13.4)
project(cstuff C)

if("${ENABLE_ASAN}")
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -fsanitize=address -fno-omit-frame-pointer")

	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -fsanitize=address -fno-omit-frame-pointer")
endif()

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -g")

macro(STANDALONE_EXECUTABLE EXE_FILENAME)
	add_executable(${EXE_FILENAME} ${EXE_FILENAME}.c)
endmacro(STANDALONE_EXECUTABLE)

STANDALONE_EXECUTABLE(blanklines)

STANDALONE_EXECUTABLE(derpbob)

STANDALONE_EXECUTABLE(endian_long)

#STANDALONE_EXECUTABLE(fixfinaleol)

STANDALONE_EXECUTABLE(fizzbuzz)

STANDALONE_EXECUTABLE(backspaces)

STANDALONE_EXECUTABLE(handle_signal)

STANDALONE_EXECUTABLE(insert_newlines)

STANDALONE_EXECUTABLE(modulo)

STANDALONE_EXECUTABLE(multi_files)

STANDALONE_EXECUTABLE(pointers_equal)

STANDALONE_EXECUTABLE(sizeof_things)

STANDALONE_EXECUTABLE(sizeof_void)

STANDALONE_EXECUTABLE(to_inserts)

STANDALONE_EXECUTABLE(udp_listen)

STANDALONE_EXECUTABLE(use_gai)

STANDALONE_EXECUTABLE(shifting)

STANDALONE_EXECUTABLE(stackoverflow)

STANDALONE_EXECUTABLE(bad_mem_stuff)

add_executable(use_gai_a use_gai_a.c)
target_link_libraries(use_gai_a pthread anl)
