cmake_minimum_required(VERSION 3.5)
project(watchdog)

# set standard:
set(CMAKE_CXX_STANDARD 20)

# compiler flags:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# add source files:
set(SOURCES
	src/main.cpp
)

# add headers
set(HEADERS
)

# create an executable:
add_executable(watchdog ${SOURCES} ${HEADERS})

# Specify the output directory for the binary
set_target_properties(watchdog PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


# command to run in root dir:
add_custom_target(run
	COMMAND watchdog
	DEPENDS watchdog
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
	COMMENT "Running watchdog"
)
# linker flags: no need
#set(LDFLAGS	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi)
