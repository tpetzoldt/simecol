#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

int isInside(int n, int m, int i, int j);

double getpixel(int n, int m, int i, int j, double* x);

double getpixelb(int n, int m, int i, int j, double* x, double b);

void setpixel(int n, int m, int i, int j, double* x, double val); 

int imax(int x, int y);
  
int imin(int x, int y);



