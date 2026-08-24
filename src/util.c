/*
 Utility functions for package simecol
 Th. Petzoldt
 */

/* ---- bounds test -------------------------------------------------------- */
int isInside(int n, int m, int i, int j) {
  return (0 <= i && i < n && 0 <= j && j < m);
}

/* ---- single, defensive pixel accessor ----------------------------------- */
/* Out-of-range reads return the boundary colour, so the image edge behaves
 like a natural wall in boundary-fill mode. */
double getpixel(int n, int m, int i, int j, double* x, double b) {
  return isInside(n, m, i, j) ? x[i + n * j] : b;
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

void setpixel(int n, int m, int i, int j, double* x, double val) {
  if (isInside(n, m, i, j))
    x[i + n * j] = val;
}
