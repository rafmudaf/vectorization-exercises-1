cmake_minimum_required(VERSION 3.6)

# Linux notes compile with: icc -O2 -c -g -qopt-report=3 *.c
# //Linux - link with-lmkl_intel_ilp64 -lmkl_core -lmkl_sequential -lm -lrt

project(submm VERSION 0.1 LANGUAGES C)

if(CMAKE_Fortran_COMPILER_ID MATCHES GNU)
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -g -fprofile-arcs -ftest-coverage")
endif()

find_package(blas)
message("BLAS_LIBRARIES ${BLAS_LIBRARIES}")

# configure the sqrt program
set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/basemain.c ${CMAKE_CURRENT_LIST_DIR}/submm.c)
add_executable(basemain ${SOURCES})
target_link_libraries(basemain ${BLAS_LIBRARIES})
