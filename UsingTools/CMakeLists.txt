#---------------------------------
#build executable

cmake_minimum_required(VERSION 2.8.9)
project(directory_test)

#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(testStudent ${SOURCES})

#cmake ..
#cmake --build . --taget <all> <exec name>
################
#cmake -O3 -Wall -Wextra CMAKE_BUILD_TYPE=Release ..
#cmake --build . --config Release --target mydelay

#-------------------------------------
#build shared library

cmake_minimum_required(VERSION 2.8.9)
project(directory_test)
set(CMAKE_BUILD_TYPE Release)

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

#Generate the shared library from the sources
add_library(testStudent SHARED ${SOURCES})

#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS testStudent DESTINATION /usr/lib)

#-----------------------------------
#build static library

cmake_minimum_required(VERSION 2.8.9)

project(directory_test)
set(CMAKE_BUILD_TYPE Release)
 
#Bring the headers, such as Student.h into the project
include_directories(include)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
 
#Generate the static library from the sources
add_library(testStudent STATIC ${SOURCES})
 
#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS testStudent DESTINATION /usr/lib)

#--------------------------------------
#using shared/static library

cmake_minimum_required(VERSION 2.8.9)
project (TestLibrary)
 
#For the shared library:
set ( PROJECT_LINK_LIBS libtestStudent.so )
link_directories( ~/exploringBB/extras/cmake/studentlib_shared/build )
 
#For the static library:
#set ( PROJECT_LINK_LIBS libtestStudent.a )
#link_directories( ~/exploringBB/extras/cmake/studentlib_static/build )
 
include_directories(~/exploringBB/extras/cmake/studentlib_shared/include)
 
add_executable(libtest libtest.cpp)
target_link_libraries(libtest ${PROJECT_LINK_LIBS} )

#----------------------------------
#example 0

cmake_minimum_required(VERSION 2.8)

project(Graphics3DEngine)

set(CMAKE_BUILD_TYPE Debug)
include_directories(include)
#link_directories(${SOURCE_DIR}/src)
file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)
set(PROJECT_LINK_LIBS libGLEW.so libGL.so libGLU.so libglfw.so.3 pthread.so)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS})

#set compiler options
add_defenitions(-std=c++14)
set( CMAKE_CXX_FLAGS "-g -Wall")




#Suppose you want to add those flags (better to declare them in a constant):

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

#There are several ways to add them:

#    The easiest one (not clean, but easy and convenient, and works only for compile flags, C & C++ at once):

    add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

#   Appending to corresponding CMake variables:

    SET(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS})
    SET(CMAKE_EXE_LINKER_FLAGS  ${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS})
//--------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

//--------------------------
set(CMAKE_BUILD_TYPE Release)
$ cmake -D CMAKE_BUILD_TYPE=Debug ..
if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

//---------------------------------------

cmake_minimum_required(VERSION 3.10)
project(granular)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(include)
set(PROJECT_LINK_LIBS libportaudio.a -lrt -lm -lasound -ljack -pthread)
link_directories(lib)
file(GLOB SOURCES "src/*.cpp" "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS})
