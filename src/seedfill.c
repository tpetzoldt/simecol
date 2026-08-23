/* 
   Seedfill for images and numeric matrices
   Th. Petzoldt
*/

#include <R.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

int imax(int x, int y) {
    if (x > y) {
      return x;
    } else {
      return y;
    }
}

int imin(int x, int y) {
    if (x < y) {
      return x;
    } else {
      return y;
    }
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

/* version 2: with generalized boundaries */
/* open, torus, [todo: reflection] */
double xgetpixel(int n, int m, int i, int j, int bound, double* x) {
    double ret = 0;

    if ((0 <= i) & (i < n) & (0 <= j) & (j < m)) { /*(isInside(n, m, i, j, x)) */
        ret = x[i + n * j];
    } else {
      int ii = i, jj = j;
      /* bit set == torus; bottom, left, top, right */
      if (bound & 1) jj = MIN(jj, n-1);
      if (bound & 2) ii = MIN(ii, n-1);
      if (bound & 4) jj = MAX(jj, 0);
      if (bound & 8) ii = MAX(ii, 0);
      /* open boundaries or torus */
      if (isInside(n, m, ii, jj, x)) {
         ret = x[((i + n) % n) + n * ((j + m) % m)]; /* modulo */
      }
    }
    return ret;
}


/* version 3: return boundary color if outside the area */
double getpixelb(int n, int m, int i, int j, double* x, double cbound) {
    if (isInside(n, m, i, j, x)) {
      return x[i + n * j];
    } else {
      return cbound;
    }
}

void setpixel(int n, int m, int i, int j, double* x, double* fcol) {
    if (isInside(n, m, i, j, x)) {
       x[i + n * j] = *fcol;
    }
}
 

/* recursive version of seedfill */
void fill(int* n, int* m, int* i, int* j, double* x, 
          double* fcol, double* bcol, double* tol) {
  int ii = *i, jj = *j; double col;
  if (isInside(*n, *m, *i, *j, x)) {
    col = getpixel(*n, *m, *i, *j, x);

    if( col != *fcol && col != *bcol ) {
      setpixel(*n, *m, *i, *j, x, fcol);
      ii=*i+1; fill(n, m, &ii, j,   x, fcol, bcol, tol);
      jj=*j+1; fill(n, m, i,   &jj, x, fcol, bcol, tol);
      ii=*i-1; fill(n, m, &ii, j,   x, fcol, bcol, tol);
      jj=*j-1; fill(n, m, i,   &jj, x, fcol, bcol, tol);
    }
  }
}  

/*------------------------*/
/* non-recursive seedfill */
/*------------------------*/

/* Push a seed onto the stack if it is within bounds and there is space */
void pushSeed(int x, int y, int* xstack, int* ystack, int* ptr, int maxptr, int n, int m) {
  // Check if the seed is within the image boundaries
  if (x < 0 || x >= n || y < 0 || y >= m) {
    // Out-of-bounds seed, do not push
    return;
  }
  
  // Check if the stack has enough space
  if (*ptr >= maxptr) {
    // Stack overflow, log an error and exit gracefully
    Rprintf("Error: Stack overflow while pushing seed (%d, %d).\n", x, y);
    error("Stack overflow: Unable to continue seed fill operation.");
    return;
  }
  
  // Push the seed onto the stack
  xstack[*ptr] = x;
  ystack[*ptr] = y;
  (*ptr)++;
}



int popSeed(int* x, int* y, int* xstack, int* ystack, int* ptr) {
  int ret = FALSE;
  if (*ptr > 0) {
    *ptr = *ptr - 1;
    *x = xstack[*ptr];
    *y = ystack[*ptr];
    ret = TRUE;
  }
  return ret;    
}

/* fill pixels to the left and right of the seed pixel until you hit 
    boundary pixels.  Return the locations of the leftmost and rightmost 
    filled pixels.*/
void FillContiguousSpan(int x, int y, double bound, double fill, int *xLeft, int *xRight,
                                 int n, int m, double* xx, double tol) {
  int i = x;
  
  // Fill right span
  while (i < n && fabs(getpixelb(n, m, i, y, xx, bound) - bound) > tol) {
    setpixel(n, m, i, y, xx, &fill);
    i++;
  }
  *xRight = i - 1;
  
  // Fill left span
  i = x - 1;
  while (i >= 0 && fabs(getpixelb(n, m, i, y, xx, bound) - bound) > tol) {
    setpixel(n, m, i, y, xx, &fill);
    i--;
  }
  *xLeft = i + 1;
}



/* the main routine */
void FillSeedsOnStack(double bound, double fill, 
                      int n, int m, double* xx,
                      int* xstack, int* ystack, int* ptr, int maxptr, double tol) {
  double col1 = 0, col2 = 0;
  int x, y;              /* Current seed pixel */
  int xLeft, xRight;     /* Current span boundary locations */
  int i;

  while (popSeed(&x, &y, xstack, ystack, ptr)) {
    /* Boundary check for the current seed */
    if (x < 0 || x >= n || y < 0 || y >= m) continue;
  
    /* Check if the current pixel is within the tolerance of the boundary value */
    if (fabs(getpixelb(n, m, x, y, xx, bound) - bound) > tol) {
      /* Fill the contiguous span and get its boundaries */
      FillContiguousSpan(x, y, bound, fill, &xLeft, &xRight, n, m, xx, tol);

      /* Handle multi-pixel spans */
      if (xLeft != xRight) {
        /* Handle the row above */
        if (y + 1 < m) {  // Boundary check for the row above
          for (i = xLeft + 1; i <= xRight; i++) {
            col1 = getpixelb(n, m, i - 1, y + 1, xx, bound);
            col2 = getpixelb(n, m, i, y + 1, xx, bound);
            if (fabs(col1 - bound) > tol && fabs(col1 - fill) > tol && fabs(col2 - bound) <= tol) {
              pushSeed(i - 1, y + 1, xstack, ystack, ptr, maxptr, n, m);
            }
          }
          col2 = getpixelb(n, m, xRight, y + 1, xx, bound);
          if (fabs(col2 - bound) > tol && fabs(col2 - fill) > tol) {
            pushSeed(xRight, y + 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
      
        /* Handle the row below */
        if (y - 1 >= 0) {  // Boundary check for the row below
          for (i = xLeft + 1; i <= xRight; i++) {
            col1 = getpixelb(n, m, i - 1, y - 1, xx, bound);
            col2 = getpixelb(n, m, i, y - 1, xx, bound);
            if (fabs(col1 - bound) > tol && fabs(col1 - fill) > tol && fabs(col2 - bound) <= tol) {
              pushSeed(i - 1, y - 1, xstack, ystack, ptr, maxptr, n, m);
            }
          }
          col2 = getpixelb(n, m, xRight, y - 1, xx, bound);
          if (fabs(col2 - bound) > tol && fabs(col2 - fill) > tol) {
            pushSeed(xRight, y - 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
      } else {
        /* Handle single-pixel spans */
        if (y + 1 < m) {  // Boundary check for the row above
          col1 = getpixelb(n, m, xLeft, y + 1, xx, bound);
          if (fabs(col1 - fill) > tol) {
            pushSeed(xLeft, y + 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
        if (y - 1 >= 0) {  // Boundary check for the row below
          col2 = getpixelb(n, m, xLeft, y - 1, xx, bound);
          if (fabs(col2 - fill) > tol) {
            pushSeed(xLeft, y - 1, xstack, ystack, ptr, maxptr, n, m);
          }
        }
      }
    }
  }
}


/* entry routine for seedfill */
void c_seedfill(int* n, int* m, int* i, int* j, double* x, 
              double* fcol, double* bcol, double* tol) {
  int* xstack;
  int* ystack;
  int p = 0, *ptr;
  int maxptr;
  xstack = (int *) R_alloc(*n * *m, sizeof(int)); /* sorry. estimated value only */
  ystack = (int *) R_alloc(*n * *m, sizeof(int));
  maxptr = *m * *n;
  ptr = &p;
  pushSeed(*i, *j, xstack, ystack, ptr, maxptr, *n, *m);
  FillSeedsOnStack(*bcol, *fcol, *n, *m, x, xstack, ystack, ptr, maxptr, *tol);
}

