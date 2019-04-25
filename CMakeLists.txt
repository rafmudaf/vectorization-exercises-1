cmake_minimum_required(VERSION 3.6)

project(submm VERSION 0.1 LANGUAGES C)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

if(CMAKE_C_COMPILER_ID MATCHES Intel)
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -c")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qopt-report=3")
endif()

# This adds -O2 -g
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

if (CMAKE_Fortran_COMPILER_ID STREQUAL "Intel")
  find_package(MKL)
endif()
if (MKL_FOUND)
  include_directories(${MKL_INCLUDE_DIRS})
  set(BLAS_LIBRARIES ${MKL_LIBRARIES})
else()
  find_package(BLAS REQUIRED)
endif()

set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/basemain.c ${CMAKE_CURRENT_LIST_DIR}/submm.c)
add_executable(basemain ${SOURCES})
target_link_libraries(basemain ${BLAS_LIBRARIES} rt)
