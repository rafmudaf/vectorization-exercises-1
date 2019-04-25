// A simple matrix multiply code to show some vectorization options.
//For Windows make sure #define Windows is not commented out.
// For Linux make sure #define Windows is commented out. 
// The last option uses Intel MKL.  You may link with MKL, link with another BLAS library
// or just comment out the call to dgemm.  Linux notes compile with: icc -O2 -c -g -qopt-report=3 *.c  
//Linux - link with-lmkl_intel_ilp64 -lmkl_core -lmkl_sequential -lm -lrt
// Linux - This will work with gcc as well - just revert back to standard
// malloc and free if you choose not to use the intel compiler.


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <Accelerate/Accelerate.h>

#ifdef WINDOWS
#include <windows.h>
#endif

#define MATSIZE 2048
#define BLOCKSIZE 32

void setmat() ;
void fillmat() ;
void abasicmm1() ;
void abettermm1() ;
void ablockmm() ;
void ablockmm2() ;
void abasicmm2() ;
void abettermm2();

void checkmatmult() ;
double ntime() ;
double sqadd() ;

int main(int argc, char *argv[])
{

   double *a, *b, *c, *aa ;
   unsigned int n ;
   unsigned i, j, k, iInner, jInner, kInner, blockSize ;
#ifdef WINDOWS
   clock_t ts1, ts2, ts3, ts4, ts5, ts6, ts7, ts8, ts9, ts0, ts10 ;
#else
   struct timespec ts1, ts2, ts3, ts4, ts5, ts6, ts7, ts8, ts9, ts0, ts10;
#endif
      
   printf("Code beginning\n") ;
   n = MATSIZE ; // default settings
   blockSize = BLOCKSIZE ;
   if (argc != 3)
   {
      printf("using matrix size %d and blocksize %d\n",n,blockSize) ;
   } else
   {
      n = atoi(argv[1]) ;
      blockSize = atoi(argv[2]) ;
      printf("matrix size %d blocksize %d\n", n,blockSize) ;
   }
   if (n%blockSize)
   {
      printf("for this simple example matrix size must be a multiple of the block size.\n  Please re-start \n") ;
      exit(0);
   }
// allocate matrices

   a = (double *)malloc((n+blockSize)*(n+blockSize)* sizeof(double)) ;
   b = (double *)malloc((n+blockSize)*(n+blockSize)* sizeof(double)) ;
   c = (double *)malloc((n+blockSize)*(n+blockSize)* sizeof(double)) ;
   aa = (double *)malloc((n+blockSize)*(n+blockSize)* sizeof(double)) ;

   if (aa == NULL) // cheap check only the last allocation checked.
   {
     printf("insufficient memory \n") ;
     exit(0) ;
   }

// fill matrices
      srand(4) ; // set random seed (change to go off time stamp to make it better

   fillmat(n,n,b) ;
   fillmat(n,n,c) ;

   setmat(n, n, a) ;
   setmat(n, n, aa) ;
// multiply matrices

#ifdef WINDOWS
   ts1 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts1);
#endif
      
   abasicmm1 (n,n,a,b,c) ;

#ifdef WINDOWS
   ts2 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts2);
#endif

    abasicmm2 (n,n,aa,b,c) ;
#ifdef WINDOWS
	ts3 = clock();
#else
	clock_gettime(CLOCK_REALTIME, &ts3) ;
#endif
 
   printf("basicmm1 basic time      %lf\n",ntime(ts1,ts2)) ;
   printf("basicmm2-pragma simd     %lf\n",ntime(ts2,ts3)) ;

   setmat(n, n, a) ;
   setmat(n, n, aa) ;

#ifdef WINDOWS
   ts1 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts1) ;
#endif
   abettermm1 (n,n,a,b,c) ;
#ifdef WINDOWS
   ts2 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts2) ;
#endif
   abettermm2 (n,n,aa,b,c) ;

#ifdef WINDOWS
    ts3 = clock();
#else
    clock_gettime(CLOCK_REALTIME, &ts3) ;
#endif

   printf("bettermm1      time      %lf\n",ntime(ts1,ts2)) ;
   printf("betterm2 pragma simd     %lf\n",ntime(ts2,ts3)) ;
 
   setmat(n, n, a) ;
   setmat(n, n, aa) ;

#ifdef WINDOWS
   ts4 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts4) ;
#endif
   ablockmm (n, n, a, b, c, blockSize) ;
#ifdef WINDOWS
   ts5 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts5) ;
#endif

   ablockmm2 (n, n, aa, b, c, blockSize) ;
#ifdef WINDOWS
   ts6 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts6) ;
#endif

   cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
	   n, n, n, 1.0, b, n, c, n, 0.0, a, n);
#ifdef WINDOWS
   ts7 = clock();
#else
   clock_gettime(CLOCK_REALTIME, &ts7);
#endif


/**/
   checkmatmult(n,n,a,aa) ;

   printf("block ordering orig     %lf\n",ntime(ts4,ts5)) ;
   printf("block ordering mm2      %lf\n",ntime(ts5,ts6)) ;
   printf("mkl cblas_dgemm         %lf\n", ntime(ts6, ts7));
     
   free(a);
   free(b);
   free(c);
   free(aa);
}

#ifdef WINDOWS
double ntime( clock_t ts0, clock_t ts1) 
#else
double ntime(struct timespec ts0, struct timespec ts1)
#endif
{
   double t1, tmp ;

#ifdef WINDOWS
   t1 = ((double)(ts1 - ts0)) / CLOCKS_PER_SEC;
#else
      t1 =  ts1.tv_sec-ts0.tv_sec;
   tmp = ts1.tv_nsec-ts0.tv_nsec;
   tmp /= 1.0e+09 ;
   t1 += tmp ;
#endif
   
   return (t1) ; 
}
