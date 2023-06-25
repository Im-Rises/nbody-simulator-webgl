cmake_minimum_required(VERSION 3.8)

project(nbody-simulator-webgl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "Build type not specified: Use Release by default")
endif ()

if (EMSCRIPTEN)
    #    set(CMAKE_EXECUTABLE_SUFFIX ".html") # Add default .html file
    #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s PTHREAD_POOL_SIZE=4 -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD --bind") # PThreads (for multithreading)
    #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MODULARIZE=1 -s WASM=1 -s EXPORT_ES6=1 -s NO_FILESYSTEM=1 -s FULL_ES3=1 -s USE_GLFW=3 -s EXPORT_NAME=nbodysimulatorstarter") # For module (to use in react, etc...)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1") # For simple .html file
endif ()

add_subdirectory(NBodySimulator)

if (EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/web)
    #    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index") # Change output name
endif ()
