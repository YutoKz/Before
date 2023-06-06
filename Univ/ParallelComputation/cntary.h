#include <stdlib.h>

// Usage:
//   int *a;
//   a = carray1i(n, lb);
// => "a" will be an 1d array,
//    for which a[lb] through a[lb+n-1] available
int *carray1i(int n, int lb) {
  int  *array;
  array = (int*)malloc(n*sizeof(int)) - lb;
  return array;
}

// Usage:
//   int **a;
//   a = carray2i(ni, nj, lbi, lbj);
// => "a" will be a 2d array,
//    for which a[lbi][lbj] through a[lbi+ni-1][lbj+nj-1] available
int **carray2i(int ni, int nj, int lbi, int lbj) {
  int  **array;
  int  i;
  array = (int**)malloc(ni*sizeof(int*)) - lbi;
  array[lbi] = (int*)malloc(ni*nj*sizeof(int)) - lbj;
  for(i=lbi+1; i<lbi+ni; i++) array[i] = array[i-1] + nj;
  return array;
}

// Usage:
//   float *a;
//   a = carray1f(n, lb);
// => "a" will be an 1d array,
//    for which a[lb] through a[lb+n-1] available
float *carray1f(int n, int lb) {
  float  *array;
  array = (float*)malloc(n*sizeof(float)) - lb;
  return array;
}

// Usage:
//   float **a;
//   a = carray2f(ni, nj, lbi, lbj);
// => "a" will be a 2d array,
//    for which a[lbi][lbj] through a[lbi+ni-1][lbj+nj-1] available
float **carray2f(int ni, int nj, int lbi, int lbj) {
  float  **array;
  int  i;
  array = (float**)malloc(ni*sizeof(float*)) - lbi;
  array[lbi] = (float*)malloc(ni*nj*sizeof(float)) - lbj;
  for(i=lbi+1; i<lbi+ni; i++) array[i] = array[i-1] + nj;
  return array;
}

// Usage:
//   double *a;
//   a = carray1d(n, lb);
// => "a" will be an 1d array,
//    for which a[lb] through a[lb+n-1] available
double *carray1d(int n, int lb) {
  double  *array;
  array = (double*)malloc(n*sizeof(double)) - lb;
  return array;
}

// Usage:
//   double **a;
//   a = carray2d(ni, nj, lbi, lbj);
// => "a" will be a 2d array,
//    for which a[lbi][lbj] through a[lbi+ni-1][lbj+nj-1] available
double **carray2d(int ni, int nj, int lbi, int lbj) {
  double  **array;
  int  i;
  array = (double**)malloc(ni*sizeof(double*)) - lbi;
  array[lbi] = (double*)malloc(ni*nj*sizeof(double)) - lbj;
  for(i=lbi+1; i<lbi+ni; i++) array[i] = array[i-1] + nj;
  return array;
}
