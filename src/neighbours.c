/*
 Neighbour functions for cellular automata
 Th. Petzoldt
 */

#include <R.h>
#include "simecol.h"

int imax(int x, int y) {
  return (x > y) ? x : y;
}

int imin(int x, int y) {
  return (x < y) ? x : y;
}

/* ---- generalized boundaries (open / torus / [todo: reflection]) ---------
 Per edge:  bit set = torus (wrap);  bit unset = open (out-of-range -> 0).
 Bit order (matching the R wrapper packing sum(bounds * c(1,2,4,8)) and the
 neighbours() documentation order bottom, left, top, right):
 bit 1 = bottom (i >= n)
 bit 2 = left   (j <  0)
 bit 4 = top    (i <  0)
 bit 8 = right  (j >= m)
 Every out-of-range index either wraps to a valid cell or returns 0.0,
 so the final array access is always in bounds.                          */
double xgetpixel(int n, int m, int i, int j, int bound, double* x) {
  int ii = i, jj = j;
  if (i >= n) { if (bound & 1) ii = i % n;             else { return 0.0; } }
  if (i <  0) { if (bound & 4) ii = ((i % n) + n) % n; else { return 0.0; } }
  if (j <  0) { if (bound & 2) jj = ((j % m) + m) % m; else { return 0.0; } }
  if (j >= m) { if (bound & 8) jj = j % m;             else { return 0.0; } }
  //if (i < 0 || i >= n)   /* should NEVER print in cylinder mode */
  //  Rprintf("VERTICAL WRAP LEAK: i=%d j=%d bound=%d ii=%d jj=%d\n", i,j,bound,ii,jj);
  return x[ii + n * jj];
}


/* ---- basic neighbourhood function for Conway's Game of Life -------------
 Open (dead) boundary: cells beyond the edge count as 0.                  */
void c_eightneighbours(int* n, int* m, double* x, double* y) {
  int nn = *n, mm = *m;
  double c = 0;
  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < mm; j++) {
      c = getpixelb(nn, mm, i+1, j,   x, 0.0) + getpixelb(nn, mm, i,   j+1, x, 0.0) +
        getpixelb(nn, mm, i-1, j,   x, 0.0) + getpixelb(nn, mm, i,   j-1, x, 0.0) +
        getpixelb(nn, mm, i+1, j+1, x, 0.0) + getpixelb(nn, mm, i+1, j-1, x, 0.0) +
        getpixelb(nn, mm, i-1, j+1, x, 0.0) + getpixelb(nn, mm, i-1, j-1, x, 0.0);
        setpixel(nn, mm, i, j, y, c);      /* by value */
    }
  }
}


/* ---- generalized neighbourhood function for cellular automata -----------
 n     = number of rows in grid
 m     = number of columns in grid
 x     = input grid matrix
 y     = output grid matrix
 ndist = number of rows and columns in distance matrix
 wdist = weights of distance matrix
 state = value to check for
 tol   = tolerance when comparing states
 
 Open boundary: the window is clipped to the grid, so off-grid positions
 contribute nothing (equivalent to bounds = 0 in c_xneighbours).          */
void c_neighbours(int* n, int* m, double* x, double* y,
                  int* ndist, double* wdist, double* state, double* tol) {
  int    nn = *n, mm = *m, nd = *ndist, d;
  double s = 0, c = 0, dstate = *state, dtol = *tol;
  
  d = (int)floor(*ndist / 2);
  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < mm; j++) {
      c = 0; 
      for (int ii = imax(-d, -i); ii <= imin(nn - i, d); ii++) {
        for (int jj = imax(-d, -j); jj <= imin(mm - j, d); jj++) {
          s = getpixelb(nn, mm, i + ii, j + jj, x, 0.0);
          if (fabs(s - dstate) < dtol) {
            c += wdist[ii + d + nd * (jj + d)];
          }
        }
      }
      setpixel(nn, mm, i, j, y, c);      /* by value */
    }
  }
}


/*  generalized neighbourhood function for cellular automata
 Extended version with 'boundaries'.
 Interior cells (whose full nd x nd window lies inside the grid) use a
 fast path with direct array access; only true boundary cells call
 xgetpixel. For large grids the interior dominates, so the boundary
 branching and function-call overhead are avoided for ~all cells.       */
void c_xneighbours(int* n, int* m,
                   double* x, double* y,
                   int* ndist, double* wdist,
                   double* state, double* tol, int* boundaries) {
  int    nn = *n, mm = *m, nd = *ndist, d;
  double dstate = *state, dtol = *tol, s, c;
  int    bound = *boundaries, i, j, ii, jj;
  
  d = (int)floor(*ndist / 2);
  
  /* interior region: cells whose entire window stays in bounds */
  int i_lo = d, i_hi = nn - d;      /* interior rows:    [d, nn-d) */
  int j_lo = d, j_hi = mm - d;      /* interior columns: [d, mm-d) */
  
  for (i = 0; i < nn; i++) {
    int interior_row = (i >= i_lo && i < i_hi);
    for (j = 0; j < mm; j++) {
      c = 0.0; 
      
      if (interior_row && j >= j_lo && j < j_hi) {
        /* -------- fast path: no boundary handling needed -------- */
        for (ii = 0; ii < nd; ii++) {
          /* hoist weight row */
          const double* wrow = wdist + nd * ii;         
          for (jj = 0; jj < nd; jj++) {
            /* direct access */
            s = x[(i + ii - d) + nn * (j + jj - d)];
            if (fabs(s - dstate) < dtol)
              c += wrow[jj];
          }
        }
      } else {
        /* -------- slow path: boundary cells via xgetpixel -------- */
        for (ii = 0; ii < nd; ii++) {
          const double* wrow = wdist + nd * ii;
          for (jj = 0; jj < nd; jj++) {
            s = xgetpixel(nn, mm, i + ii - d, j + jj - d, bound, x);
            if (fabs(s - dstate) < dtol)
              c += wrow[jj];
          }
        }
      }
      
      setpixel(nn, mm, i, j, y, c);
    }
  }
}

