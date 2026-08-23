/* 
   Seedfill for images and numeric matrices
   Th. Petzoldt
*/

#include <R.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#include <R.h>
#include <math.h>

// Stack operations
void pushSeed(int x, int y, int* xstack, int* ystack, int* ptr, int maxptr, int n, int m) {
  if (x < 0 || x >= n || y < 0 || y >= m) return;
  if (*ptr >= maxptr) {
    Rprintf("Error: Stack overflow while pushing seed (%d, %d).\n", x, y);
    return;
  }
  xstack[*ptr] = x;
  ystack[*ptr] = y;
  (*ptr)++;
}

int popSeed(int* x, int* y, int* xstack, int* ystack, int* ptr) {
  if (*ptr > 0) {
    (*ptr)--;
    *x = xstack[*ptr];
    *y = ystack[*ptr];
    return 1;
  }
  return 0;
}

int isInside(int n, int m, int i, int j, double* x) {
  if ((0 <= i) & (i < n) & (0 <= j) & (j < m))  
    return TRUE; 
  else 
    return FALSE;
}


/* version 1: basic version */
double getpixel(int n, int m, int i, int j, double* x) {
  if (isInside(n, m, i, j, x)) {
    return x[i + n * j];
  } else {
    return 0;
  }
}

// Pixel access helpers
double getpixelb(int n, int m, int x, int y, double* xx) {
  return xx[x + n * y];
}

void setpixel(int n, int m, int x, int y, double* xx, double* val) {
  xx[x + n * y] = *val;
}


// mode: 0 = boundary-fill, 1 = seed-fill
int is_stop(double pixel, double bcol, double seed_color, double tol, int mode) {
  if (mode == 0) { // Boundary-fill
    return fabs(pixel - bcol) <= tol;
  } else { // Seed-fill
    return fabs(pixel - seed_color) > tol;
  }
}


/* fill pixels to the left and right of the seed pixel until you hit 
    boundary pixels.  Return the locations of the leftmost and rightmost 
    filled pixels.*/
void FillContiguousSpan(int x, int y, double bcol, double seed_color, double fill, 
                        int n, int m, double* xx, double tol, int mode, int* xLeft, int* xRight) {
  int i = x;
  // Fill right
  while (i < n && !is_stop(getpixelb(n, m, i, y, xx), bcol, seed_color, tol, mode)) {
    setpixel(n, m, i, y, xx, &fill);
    i++;
  }
  *xRight = i - 1;
  // Fill left
  i = x - 1;
  while (i >= 0 && !is_stop(getpixelb(n, m, i, y, xx), bcol, seed_color, tol, mode)) {
    setpixel(n, m, i, y, xx, &fill);
    i--;
  }
  *xLeft = i + 1;
}


/* the main routine */
void FillSeedsOnStack(double bcol, double seed_color, double fill, 
                      int n, int m, double* xx,
                      int* xstack, int* ystack, int* ptr, int maxptr, double tol, int mode) {
  double col1 = 0, col2 = 0;
  int x, y, xLeft, xRight, i;
  
  while (popSeed(&x, &y, xstack, ystack, ptr)) {
    if (x < 0 || x >= n || y < 0 || y >= m) continue;
    double pixel = getpixelb(n, m, x, y, xx);
    if (!is_stop(pixel, bcol, seed_color, tol, mode)) {
      FillContiguousSpan(x, y, bcol, seed_color, fill, n, m, xx, tol, mode, &xLeft, &xRight);
      
      if (xLeft != xRight) {
        // Row above
        if (y + 1 < m) {
          for (i = xLeft + 1; i <= xRight; i++) {
            col1 = getpixelb(n, m, i - 1, y + 1, xx);
            col2 = getpixelb(n, m, i, y + 1, xx);
            if (!is_stop(col1, bcol, seed_color, tol, mode) &&
                is_stop(col2, bcol, seed_color, tol, mode)) {
              pushSeed(i - 1, y + 1, xstack, ystack, ptr, maxptr, n, m);
            }
          }
          col2 = getpixelb(n, m, xRight, y + 1, xx);
          if (!is_stop(col2, bcol, seed_color, tol, mode)) {
            pushSeed(xRight, y + 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
        // Row below
        if (y - 1 >= 0) {
          for (i = xLeft + 1; i <= xRight; i++) {
            col1 = getpixelb(n, m, i - 1, y - 1, xx);
            col2 = getpixelb(n, m, i, y - 1, xx);
            if (!is_stop(col1, bcol, seed_color, tol, mode) &&
                is_stop(col2, bcol, seed_color, tol, mode)) {
              pushSeed(i - 1, y - 1, xstack, ystack, ptr, maxptr, n, m);
            }
          }
          col2 = getpixelb(n, m, xRight, y - 1, xx);
          if (!is_stop(col2, bcol, seed_color, tol, mode)) {
            pushSeed(xRight, y - 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
      } else {
        // Single-pixel span
        if (y + 1 < m) {
          col1 = getpixelb(n, m, xLeft, y + 1, xx);
          if (!is_stop(col1, bcol, seed_color, tol, mode)) {
            pushSeed(xLeft, y + 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
        if (y - 1 >= 0) {
          col2 = getpixelb(n, m, xLeft, y - 1, xx);
          if (!is_stop(col2, bcol, seed_color, tol, mode)) {
            pushSeed(xLeft, y - 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
      }
    }
  }
}



/* entry routine for seedfill */
void c_seedfill(int* n, int* m, int* i, int* j, double* x, 
                double* fcol, double* bcol, double* tol, double* seed_color, int* mode) {
  int* xstack;
  int* ystack;
  int p = 0, *ptr;
  int maxptr = (*n) * (*m);
  xstack = (int *) R_alloc(maxptr, sizeof(int));
  ystack = (int *) R_alloc(maxptr, sizeof(int));
  ptr = &p;
  pushSeed(*i, *j, xstack, ystack, ptr, maxptr, *n, *m);
  FillSeedsOnStack(*bcol, *seed_color, *fcol, *n, *m, x, xstack, ystack, ptr, maxptr, *tol, *mode);
}


