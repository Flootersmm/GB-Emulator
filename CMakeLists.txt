cmake_minimum_required(VERSION 3.10)
project(GB_Emulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(SRCDIR src)
set(IDIR include)
set(IDIR_DEAR_IMGUI include/dear_imgui)

include_directories(${IDIR} ${IDIR_DEAR_IMGUI})

file(GLOB_RECURSE CSRCS ${SRCDIR}/*.c)
file(GLOB_RECURSE CPPSRCS ${SRCDIR}/*.cpp ${SRCDIR}/imgui_impl_glfw.cpp ${SRCDIR}/imgui_impl_opengl3.cpp)
set(SRCS ${CSRCS} ${CPPSRCS})

set(ODIR ${CMAKE_BINARY_DIR}/obj)
file(MAKE_DIRECTORY ${ODIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wpedantic -Wformat-extra-args -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wpedantic -std=c++11")

set(PEDANTIC_CFLAGS "-std=c11 -Werror -Wpedantic -Wall -Wextra -Wformat=2 -O -Wuninitialized -Winit-self -Wswitch-enum -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Wno-long-long -Wglobal-constructors -Wshorten-64-to-32")

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.2
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)
  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

include_directories(${glfw_SOURCE_DIR}/include)

add_executable(gui ${SRCS})

target_link_libraries(gui glfw ${CMAKE_DL_LIBS} m dl readline GL)

set_target_properties(gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ODIR})

file(GLOB_RECURSE HEADER_FILES ${IDIR}/* ${IDIR_DEAR_IMGUI}/*)
foreach(HEADER_FILE ${HEADER_FILES})
    get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
    string(REPLACE ${IDIR} ${ODIR}/include HEADER_DEST_DIR ${HEADER_DIR})
    string(REPLACE ${IDIR_DEAR_IMGUI} ${ODIR}/include/dear_imgui HEADER_DEST_DIR ${HEADER_DEST_DIR})
    file(MAKE_DIRECTORY ${HEADER_DEST_DIR})
    configure_file(${HEADER_FILE} ${HEADER_DEST_DIR} COPYONLY)
endforeach()

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

