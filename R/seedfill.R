## R interface to seedfill ("color" fill with double precision numbers)
## for matrices, useful for grid-based models and spatial statistics

seedfill <- function(z, x = 1, y = 1, fcol = 0, bcol = 1, tol = 1e-6,
                     mode = c("boundary", "flood")) {
  # Input validation
  if (!is.matrix(z) || !is.numeric(z))        stop("z must be a numeric matrix")
  if (!is.numeric(x) || length(x) != 1)       stop("x must be a single numeric value")
  if (!is.numeric(y) || length(y) != 1)       stop("y must be a single numeric value")
  if (!is.numeric(fcol) || length(fcol) != 1) stop("fcol must be a single numeric value")
  if (!is.numeric(bcol) || length(bcol) != 1) stop("bcol must be a single numeric value")
  if (!is.numeric(tol)  || length(tol)  != 1) stop("tol must be a single numeric value")
  
  mode  <- match.arg(mode)
  imode <- switch(mode, boundary = 0L, flood = 1L)
  
  # Impossible, finite fill marker (largest representable double; .C-safe, not Inf)
  marker <- .Machine$double.xmax
  
  if (any(is.infinite(z)))
    stop("z must not contain Inf/-Inf.")
  if (any(z == marker))
    stop("z must not contain .Machine$double.xmax (reserved for masking).")
  if (fcol == marker)
    stop("fcol must not be .Machine$double.xmax (reserved for masking).")
  
  n <- dim(z)[1]
  m <- dim(z)[2]
  
  # Index validation
  if (x < 1 || x > n || y < 1 || y > m)
    stop("x and y must be within the bounds of the matrix")
  
  # Seed colour: original value at the start cell (used by flood mode)
  seed_color <- z[x, y]
  
  # Mode-specific friendly guards (no-op fills)
  if (imode == 0L) {
    if (abs(seed_color - bcol) <= tol)
      warning("Seed point lies on the boundary colour; nothing to fill.")
  } else {
    if (abs(seed_color - fcol) <= tol)
      warning("Seed colour already equals the fill colour; nothing to fill.")
  }
  
  # Single call: C fills from the seed with the impossible MARKER (not fcol),
  # stopping natively at bcol (boundary mode) or at any non-seed value (flood).
  zz <- .C(c_seedfill,
           as.integer(n),
           as.integer(m),
           as.integer(x - 1),        # zero-based indexing
           as.integer(y - 1),
           z          = as.double(z),
           as.double(marker),        # fill with the impossible marker
           as.double(bcol),          # boundary colour (used in boundary mode)
           as.double(tol),
           as.double(seed_color),    # seed colour (used in flood mode)
           as.integer(imode),        # 0 = boundary, 1 = flood
           PACKAGE = "simecol")$z
  
  zz <- matrix(zz, nrow = n, ncol = m)
  
  # Map the marker back to the requested fill colour
  zz[zz == marker] <- fcol
  zz
}
