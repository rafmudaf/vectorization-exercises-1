cmake_minimum_required(VERSION 3.6)

# Linux notes compile with: icc -O2 -c -g -qopt-report=3 *.c
# //Linux - link with-lmkl_intel_ilp64 -lmkl_core -lmkl_sequential -lm -lrt

project(submm VERSION 0.1 LANGUAGES C)

if(CMAKE_C_COMPILER_ID MATCHES GNU)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage")
endif()

set(CMAKE_BUILD_TYPE "RelWithDebInfo")

find_package(blas)
message("BLAS_LIBRARIES ${BLAS_LIBRARIES}")

# configure the sqrt program
set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/basemain.c ${CMAKE_CURRENT_LIST_DIR}/submm.c)
add_executable(basemain ${SOURCES})
target_link_libraries(basemain ${BLAS_LIBRARIES})
