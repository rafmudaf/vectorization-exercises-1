# vectorization-exercises-1
Shows variation in vectorization of matrix multiply
Welcome.  This code is used to show variations of performance using a matrix multiplication code and different compmiler pragmas.
The code can compile for both Windows and Linux.  Microsoft Visual Studio projects are included for Windows.
For Linux all you need are the two files submm.c and basemain.c.    They are currently configured to use the intel compiler and
Intel MKL.   The Intel MKL library may be free depending on your usage - check wtih Intel.  It is not included with this distribution.  
You may try to use atlas BLAS routines in place of Intel MKL - or you may choose to commentn out the call to MKL.   
Read the top paragraph of the comments in basemain.c
