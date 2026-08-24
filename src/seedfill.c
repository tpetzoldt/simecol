/*
 Seedfill for images and numeric matrices
 Th. Petzoldt
 
 Matrix layout: column-major, as used by R.
 Element (i, j) with i = row (0..n-1), j = column (0..m-1)
 is stored at x[i + n * j].
 */

#include <R.h>
#include <math.h>
#include "util.h"

/* ---- stack operations --------------------------------------------------- */
void pushSeed(int i, int j, int* istack, int* jstack, int* ptr, int maxptr,
              int n, int m) {
  if (!isInside(n, m, i, j)) return;
  if (*ptr >= maxptr) {
    error("Stack overflow while pushing seed (%d, %d).\n", i, j);
  }
  istack[*ptr] = i;
  jstack[*ptr] = j;
  (*ptr)++;
}

int popSeed(int* i, int* j, int* istack, int* jstack, int* ptr) {
  if (*ptr > 0) {
    (*ptr)--;
    *i = istack[*ptr];
    *j = jstack[*ptr];
    return 1;
  }
  return 0;
}

/* ---- stop predicate ----------------------------------------------------- */
/* mode 0 = boundary-fill: stop at the boundary colour.
   mode 1 = seed-fill    : stop at anything that is not the seed colour.     */

int is_stop(double pixel, double bcol, double seed_color, double fill,
            double tol, int mode) {
  if (fabs(pixel - fill) <= tol) return 1;      /* already filled (marker) -> stop, BOTH modes */
  if (mode == 0)
    return fabs(pixel - bcol) <= tol;           /* boundary: stop at boundary colour */
  else
    return fabs(pixel - seed_color) > tol;      /* flood: stop at non-seed colour */
}


// int is_stop(double pixel, double bcol, double seed_color, double fill,
//             double tol, int mode) {
//   if (mode == 0) {                              /* boundary fill */
//     return fabs(pixel - bcol) <= tol;           /* stop ONLY at boundary */
//   } else {                                      /* flood / seed fill */
//     if (fabs(pixel - fill) <= tol) return 1;    /* already filled -> stop */
//     return fabs(pixel - seed_color) > tol;      /* stop at non-seed colour */
//   }
// }



/* ---- fill one contiguous horizontal span -------------------------------- */
/* Fills cells to the left and right of the seed until a stop cell is met.
 Returns the leftmost and rightmost filled row indices in *iLeft / *iRight.  */
void FillContiguousSpan(int i0, int j, double bcol, double seed_color,
                        double fill, int n, int m, double* x, double tol,
                        int mode, int* iLeft, int* iRight) {
  int i = i0;
  /* fill "downwards" in row index (i increasing) */
  while (i < n && !is_stop(getpixel(n, m, i, j, x, bcol),
                           bcol, seed_color, fill, tol, mode)) {
    setpixel(n, m, i, j, x, fill);
    i++;
  }
  *iRight = i - 1;
  /* fill "upwards" in row index (i decreasing) */
  i = i0 - 1;
  while (i >= 0 && !is_stop(getpixel(n, m, i, j, x, bcol),
                            bcol, seed_color, fill, tol, mode)) {
    setpixel(n, m, i, j, x, fill);
    i--;
  }
  *iLeft = i + 1;
}

/* ---- main non-recursive fill ------------------------------------------- */
void FillSeedsOnStack(double bcol, double seed_color, double fill,
                      int n, int m, double* x,
                      int* istack, int* jstack, int* ptr, int maxptr,
                      double tol, int mode) {
  int i, j, iLeft, iRight, k;
  double col1, col2;
  
  while (popSeed(&i, &j, istack, jstack, ptr)) {
    if (!isInside(n, m, i, j)) continue;
    
    if (!is_stop(getpixel(n, m, i, j, x, bcol),
                 bcol, seed_color, fill, tol, mode)) {
      
      FillContiguousSpan(i, j, bcol, seed_color, fill, n, m, x, tol, mode,
                         &iLeft, &iRight);
      
      if (iLeft != iRight) {
        /* column above (j + 1) */
        if (j + 1 < m) {
          for (k = iLeft + 1; k <= iRight; k++) {
            col1 = getpixel(n, m, k - 1, j + 1, x, bcol);
            col2 = getpixel(n, m, k,     j + 1, x, bcol);
            if (!is_stop(col1, bcol, seed_color, fill, tol, mode) &&
                is_stop(col2, bcol, seed_color, fill, tol, mode))
              pushSeed(k - 1, j + 1, istack, jstack, ptr, maxptr, n, m);
          }
          col2 = getpixel(n, m, iRight, j + 1, x, bcol);
          if (!is_stop(col2, bcol, seed_color, fill, tol, mode))
            pushSeed(iRight, j + 1, istack, jstack, ptr, maxptr, n, m);
        }
        /* column below (j - 1) */
        if (j - 1 >= 0) {
          for (k = iLeft + 1; k <= iRight; k++) {
            col1 = getpixel(n, m, k - 1, j - 1, x, bcol);
            col2 = getpixel(n, m, k,     j - 1, x, bcol);
            if (!is_stop(col1, bcol, seed_color, fill, tol, mode) &&
                is_stop(col2, bcol, seed_color, fill, tol, mode))
              pushSeed(k - 1, j - 1, istack, jstack, ptr, maxptr, n, m);
          }
          col2 = getpixel(n, m, iRight, j - 1, x, bcol);
          if (!is_stop(col2, bcol, seed_color, fill, tol, mode))
            pushSeed(iRight, j - 1, istack, jstack, ptr, maxptr, n, m);
        }
      } else {
        /* single-cell span */
        if (j + 1 < m) {
          col1 = getpixel(n, m, iLeft, j + 1, x, bcol);
          if (!is_stop(col1, bcol, seed_color, fill, tol, mode))
            pushSeed(iLeft, j + 1, istack, jstack, ptr, maxptr, n, m);
        }
        if (j - 1 >= 0) {
          col2 = getpixel(n, m, iLeft, j - 1, x, bcol);
          if (!is_stop(col2, bcol, seed_color, fill, tol, mode))
            pushSeed(iLeft, j - 1, istack, jstack, ptr, maxptr, n, m);
        }
      }
    }
  }
}

/* ---- entry point called from R ----------------------------------------- */
void c_seedfill(int* n, int* m, int* i, int* j, double* x,
                double* fcol, double* bcol, double* tol,
                double* seed_color, int* mode) {
  int* istack;
  int* jstack;
  int p = 0, *ptr;
  int maxptr = (*n) * (*m);
  
  istack = (int *) R_alloc(maxptr, sizeof(int));
  jstack = (int *) R_alloc(maxptr, sizeof(int));
  ptr = &p;
  
  pushSeed(*i, *j, istack, jstack, ptr, maxptr, *n, *m);
  FillSeedsOnStack(*bcol, *seed_color, *fcol, *n, *m, x,
                   istack, jstack, ptr, maxptr, *tol, *mode);
}

