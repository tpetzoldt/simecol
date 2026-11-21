# A Random Walk Particle Diffusion Model

simecol example: This is a random walk (basic particle diffusion) model.

## Usage

``` r
data(diffusion)
```

## Format

An S4 object according to the
[`rwalkModel`](https://tpetzoldt.github.io/simecol/reference/odeModel.md)
specification. The object contains the following slots:

- `main`:

  A function with the movement rules for the particles.

- `parms`:

  A list with the following components:

  `ninds`

  :   number of simulated particles,

  `speed`

  :   speed of the particles,

  `area`

  :   vector with 4 elements giving the coordinates (left, bottom,
      right, top) of the coordinate system.

- `times`:

  Simulation time (discrete time steps, `by`-argument ignored).

- `init`:

  Data frame holding the start properties (Cartesian coordinates `x` and
  `y` and movement angle `a`) of the particles.

## Details

To see all details, please have a look into the implementation.

## See also

[`sim`](https://tpetzoldt.github.io/simecol/reference/sim.md),
[`parms`](https://tpetzoldt.github.io/simecol/reference/parms.md),
[`init`](https://tpetzoldt.github.io/simecol/reference/parms.md),
[`times`](https://tpetzoldt.github.io/simecol/reference/parms.md).

## Examples

``` r
##============================================
## Basic Usage:
##   explore the example
##============================================
if (FALSE) { # \dontrun{
data(diffusion)
## (1) minimal example
plot(sim(diffusion))
## show "grid of environmental conditions"
image(inputs(diffusion))

## (2) scenario
##     with homogeneous environment (no "refuge" in the middle)
no_refuge <- diffusion # Cloning of the whole model object
inputs(no_refuge) <- matrix(1, 100, 100)
plot(sim(no_refuge))
  
##============================================
## Advanced Usage:
##   Assign a function to the observer-slot.
##============================================
observer(diffusion) <- function(state, ...) {
  ## numerical output to the screen
  cat("mean x=", mean(state$x),
      ", mean y=", mean(state$y),
      ", sd   x=", sd(state$x),
      ", sd   y=", sd(state$y), "\n")
  ## animation
  par(mfrow=c(2,2))
  plot(state$x, state$y, xlab="x", ylab="y", pch=16, col="red", xlim=c(0, 100))
  hist(state$y)
  hist(state$x)
  ## default case: return the state --> iteration stores it in "out"
  state
}

sim(diffusion)

## remove the observer and restore original behavior
observer(diffusion) <- NULL
diffusion <- sim(diffusion)
} # }

##============================================
## Implementation:
##   The code of the diffusion model.
##   Note the use of the "initfunc"-slot.
##============================================
diffusion <- rwalkModel(
  main = function(time, init, parms, inputs = NULL) {
    speed   <- parms$speed
    xleft   <- parms$area[1]
    xright  <- parms$area[2]
    ybottom <- parms$area[3]
    ytop    <- parms$area[4]

    x <- init$x  # x coordinate
    y <- init$y  # y coordinate
    a <- init$a  # angle (in radians)
    n <- length(a)

    ## Rule 1: respect environment (grid as given in "inputs")
    ## 1a) identify location on "environmental 2D grid" for each individual
    i.j <- array(c(pmax(1, ceiling(x)), pmax(1, ceiling(y))), dim=c(n, 2))

    ## 1b) speed dependend on "environmental conditions"
    speed <- speed * inputs[i.j]

    ## Rule 2: Random Walk
    a  <- (a + 2 * pi / runif(a))  # (2 * pi) ?
    dx <- speed * cos(a)
    dy <- speed * sin(a)
    x  <- x + dx
    y  <- y + dy

    ## Rule 3: Wrap Around
    x <- ifelse(x > xright, xleft, x)
    y <- ifelse(y > ytop, ybottom, y)
    x <- ifelse(x < xleft, xright, x)
    y <- ifelse(y < ybottom, ytop, y)
    data.frame(x=x, y=y, a=a)
  },
  times  = c(from=0, to=100, by=1),
  parms  = list(ninds=50, speed = 1, area = c(0, 100, 0, 100)),
  solver = "iteration",
  initfunc = function(obj) {
    ninds   <- obj@parms$ninds
    xleft   <- obj@parms$area[1]
    xright  <- obj@parms$area[2]
    ybottom <- obj@parms$area[3]
    ytop    <- obj@parms$area[4]
    obj@init <- data.frame(x = runif(ninds) * (xright - xleft) + xleft,
                           y = runif(ninds) * (ytop - ybottom) + ybottom,
                           a = runif(ninds) * 2 * pi)
    inp <- matrix(1, nrow=100, ncol=100)
    inp[, 45:55] <- 0.2
    inputs(obj) <- inp
    obj
  }
)
```
