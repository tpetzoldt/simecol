/* 
 Neighbour functions for cellular automata
 Th. Petzoldt
 */

#include <R.h>
#include "simecol.h"
  
/* basic neighbourhood function for Conway's Game of Life */
void c_eightneighbours(int* n, int* m, double* x, double* y) {
  int nn = *n, mm = *m;
  double c = 0;
  for (int i = 0; i < nn; i++) {
      for (int j = 0; j < mm; j++) {
        c = getpixel(nn, mm, i+1, j,   x) +
            getpixel(nn, mm, i,   j+1, x) +
            getpixel(nn, mm, i-1, j,   x) +
            getpixel(nn, mm, i,   j-1, x) +
            getpixel(nn, mm, i+1, j+1, x) +
            getpixel(nn, mm, i+1, j-1, x) +
            getpixel(nn, mm, i-1, j+1, x) +
            getpixel(nn, mm, i-1, j-1, x);
        setpixel(nn, mm, i, j, y, &c);
      }
  }
}

/* generalized neighbourhood function for cellular automata */
void c_neighbours(int* n, int* m, double* x, double* y, 
                int* ndist, double* wdist, double* state, double* tol) {
  /* 
    n = number of rows in grid
    m = number of columns in grid
    x = input grid matrix
    y = output grid matrix
    ndist = number of rows and columns in distance matrix
    wdist = weights of distance matrix
    state = value to check for
    tol   = tolerance when comparing states
  */
  int   nn = *n, mm = *m, nd = *ndist, d;
  double s = 0,  c = 0, dstate = *state, dtol = *tol;

  d = (int)floor(*ndist / 2); 
  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < mm; j++) {
      c = 0; /* cum. neighbourhood */
      for (int ii = imax(-d, -i); ii <= imin(nn - i, d); ii++) {
	      for (int jj = imax(-d, -j); jj <= imin(mm - j, d); jj++) {
          s = getpixel(nn, mm, i + ii, j + jj,   x);
          if (fabs(s - dstate) < dtol) {
            c += wdist[ii + d + nd * (jj + d)];
          }
        }
      }
      setpixel(nn, mm, i, j, y, &c);
    }
  }
}

/*  generalized neighbourhood function for cellular automata   */
/* === extended version with additional argument 'boundaries' === */
void c_xneighbours(int* n, int* m, double* x, double* y, 
                int* ndist, double* wdist, double* state, 
                double* tol, int* boundaries) {
  /* 
    n = number of rows in grid
    m = number of columns in grid
    x = input grid matrix
    y = output grid matrix
    ndist = number of rows and columns in distance matrix
    wdist = weights of distance matrix
    state = value to check for
    tol   = tolerance when comparing states
  */
  int   nn = *n, mm = *m, nd = *ndist, d;
  double s = 0,  c = 0, dstate = *state, dtol = *tol;
  int bound = *boundaries, i, j, ii, jj;

  d = (int)floor(*ndist / 2); 
  for (i = 0; i < nn; i++) {
    for (j = 0; j < mm; j++) {
      c = 0; /* cum. neighbourhood */
      for (ii = 0; ii < nd; ii++) {      
 	      for (jj = 0; jj < nd; jj++) { 	      
          s = xgetpixel(nn, mm, i + ii - d, j + jj - d, bound, x);
          if (fabs(s - dstate) < dtol) {
            c += wdist[ii + nd * jj];
          }
        }
      }
      setpixel(nn, mm, i, j, y, &c);
    }
  }
}

