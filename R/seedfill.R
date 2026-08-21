## R interface to seedfill ("color" fill with double precision numbers)
## for matrices, useful for grid-based models and spatial statistics

seedfill <- function(z, x = 1, y = 1, fcol = 0, bcol = 1, tol = 1e-6) {
  # Input validation
  if (!is.matrix(z)) stop("z must be a matrix")
  if (!is.numeric(z)) stop("z must be a numeric matrix")
  if (!is.numeric(x) || length(x) != 1) stop("x must be a single numeric value")
  if (!is.numeric(y) || length(y) != 1) stop("y must be a single numeric value")
  if (!is.numeric(fcol)) stop("fcol must be numeric")
  if (!is.numeric(bcol)) stop("bcol must be numeric")
  if (!is.numeric(tol)) stop("tol must be numeric")
  
  n <- dim(z)[1]
  m <- dim(z)[2]
  
  # Index validation
  if (x < 1 || x > n || y < 1 || y > m) {
    stop("x and y must be within the bounds of the matrix")
  }
  
  # Use .Machine$double.xmax as the initial temporary fill color
  ffcol <- .Machine$double.xmax
  
  # Check if .Machine$double.xmax exists in the matrix
  if (any(z == ffcol, na.rm = TRUE)) {
    # If conflict exists, dynamically select an alternative ffcol
    repeat {
      ffcol <- runif(1, min = -1e6, max = 1e6)  # Generate a random value
      if (!any(z == ffcol, na.rm = TRUE)) break  # Ensure it does not exist in z
    }
  }
  
  # Call the C function
  z <- .C(c_seedfill,
          as.integer(n),
          as.integer(m),
          as.integer(x - 1),  # Convert to zero-based indexing
          as.integer(y - 1),  # Convert to zero-based indexing
          z = as.double(z),
          as.double(ffcol),
          as.double(bcol),
          as.double(tol),
          PACKAGE = "simecol")$z
  
  # Replace temporary fill color with the desired fill color
  z <- ifelse(z == ffcol, fcol, z)
  
  # Return the result as a matrix
  return(matrix(z, nrow = n, ncol = m))
}
