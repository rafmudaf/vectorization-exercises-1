// A simple matrix multiply code to show affect of ordering and blocking
// to compile this use gcc -O2 mm.c -lrt or icc -O2 mm.c -lrt -lmkl
// remove or keep line 100 specifying AVX2
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void setmat();
void fillmat();
void abasicmm1();
void abettermm();
void ablockmm() ;
void ablockmm2() ;
void abasicmm2(); 
void abettermm2() ;
//
void checkmatmult() ;
double ntime() ;

void setmat(int n, int m, double a[n][m])
{
   int i, j ;

   for (i=0;i<n; i++)
      for (j = 0 ; j<m; j++)
      {
         a[i][j] = 0.0 ;
      }
}

void fillmat(int n, int m, double a[n][m])
{
   int i, j ;

   for (i = 0; i<n; i++)
      for (j = 0 ; j < m; j++)
        a[i][j] = (double)rand() / 3.1e09 ;
}

void abasicmm1(int n, int m, double a[n][m], double b[n][m], double c[n][m])
{
   int i, j, k ;

   for (i=0;i<n; i++)
      for (j = 0; j<n; j++)
         for (k=0;k<n; k++)
            a[i][j] += b[i][k]* c[k][j] ;
}

void abasicmm2(int n, int m, double a[n][m], double b[n][m], double c[n][m])
{
	// pragmas added so compiler can generate better code
   int i, j, k ;

   for (i=0;i<n; i++)
      for (j = 0; j<n; j++)
#pragma simd
         for (k=0;k<n; k++)
            a[i][j] += b[i][k]* c[k][j] ;
}

void abettermm1(int n, int m, double a[n][m], double b[n][m], double c[n][m])
{
   int i, j, k ;

   for (i=0;i<n; i++)
      for (k=0;k<n; k++)
         for (j = 0; j<n; j++)
            a[i][j] += b[i][k]* c[k][j] ;
}
void abettermm2(int n, int m, double a[n][m], double b[n][m], double c[n][m])
{
   int i, j, k ;
   // pragmas added so compiler can generate better code

   for (i=0;i<n; i++)
      for (k=0;k<n; k++)
#pragma vector aligned
#pragma simd
         for (j = 0; j<n; j++)
            a[i][j] += b[i][k]* c[k][j] ;
}
void ablockmm(int n,int m,double a[n][m],double b[n][m],double c[n][m], int blockSize)
{
// fastest icc gcc
   int i, j, k, iInner, jInner, kInner ;
#pragma vector aligned
   for (i = 0; i < n; i+=blockSize)
      for (k = 0 ; k < n; k+=blockSize)
         for (j=0; j<n ; j+= blockSize)
            for (iInner = i; iInner<i+blockSize; iInner++)
               for (kInner = k ; kInner<k+blockSize ; kInner++)
#pragma vector aligned
#pragma simd
                  for (jInner = j ; jInner<j+blockSize; jInner++)
                        a[iInner][jInner] += b[iInner][kInner] * c[kInner][jInner] ;
}
// remove or keep this pragma based on your platform
#pragma intel optimization_parameter target_arch=CORE-AVX2
void ablockmm2(int n, int m, double a[n][m], double b[n][m], double c[n][m], int blockSize)
{
	// fastest icc gcc
	int i, j, k, iInner, jInner, kInner;
#pragma vector aligned
	for (i = 0; i < n; i += blockSize)
		for (k = 0; k < n; k += blockSize)
			for (j = 0; j<n; j += blockSize)
				for (iInner = i; iInner<i + blockSize; iInner++)
					for (kInner = k; kInner<k + blockSize; kInner++)
#pragma vector aligned
#pragma simd
						for (jInner = j; jInner<j + blockSize; jInner++)
							a[iInner][jInner] += b[iInner][kInner] * c[kInner][jInner];
}

void checkmatmult(int n,int m, double a[n][m], double aa[n][m])
{
// crude check.  Never compare floating point or double with ==.
// most floating point results are too sensitive to order of operations.
// this worked(sizes up to about 4600) it was quick, in general this is not safe
   int i, j ;
/*for (i=0;i<n;i++)
for (j=0;j<m;j++)
   if (a[i][j]-aa[i][j] != 0.0) printf("diff i %d %d diff %lf\n",i,j,a[i][j]-aa[i][j]) ;
   /**/
printf("check OK\n") ;
}

