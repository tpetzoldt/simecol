## R interface to seedfill ("color" fill with double precision numbers)
## for matrices, useful for grid-based models and spatial statistics

seedfill <- function(z, x = 1, y = 1, fcol = 0, bcol = NULL, tol = 1e-6) {
  # Input validation
  if (!is.matrix(z)) stop("z must be a matrix")
  if (!is.numeric(z)) stop("z must be a numeric matrix")
  if (!is.numeric(x) || length(x) != 1) stop("x must be a single numeric value")
  if (!is.numeric(y) || length(y) != 1) stop("y must be a single numeric value")
  if (!is.numeric(fcol)) stop("fcol must be numeric")
  if (!is.numeric(tol)) stop("tol must be numeric")
  
  n <- dim(z)[1]
  m <- dim(z)[2]
  
  # Index validation
  if (x < 1 || x > n || y < 1 || y > m) {
    stop("x and y must be within the bounds of the matrix")
  }
  
  # Dynamically determine the seed color (value at the seed point)
  seed_color <- z[x, y]
  
  # Determine the mode based on bcol
  if (is.null(bcol) || (length(bcol) == 1 && is.na(bcol))) {
    # Mode B: Seed-based fill (stop when color differs from seed)
    mode <- 1L
    bcol <- 0  # Boundary color is irrelevant in this mode
  } else {
    # Mode A: Boundary-based fill (stop at bcol)
    mode <- 0L
  }
  
  # If the seed color is already the fill color, return the matrix as is
  if (seed_color == fcol) {
    return(z)
  }
  
  # Call the C function
  filled_z <- .C("c_seedfill",
                 as.integer(n),
                 as.integer(m),
                 as.integer(x - 1),  # Convert to zero-based indexing
                 as.integer(y - 1),  # Convert to zero-based indexing
                 z = as.double(z),
                 as.double(fcol),    # Final fill color
                 as.double(bcol),    # Boundary color (used only in Mode A)
                 as.double(tol),     # Tolerance for comparisons
                 as.double(seed_color),  # Seed color (used only in Mode B)
                 as.integer(mode),   # Mode: 0 = boundary-based, 1 = seed-based
                 PACKAGE = "simecol")$z
  
  # Return the result as a matrix
  return(matrix(filled_z, nrow = n, ncol = m))
}
