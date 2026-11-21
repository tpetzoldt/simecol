# The Classical Coway's Game of Life

simecol example: This model simulates a deterministic cellular
automaton.

## Usage

``` r
data(conway)
```

## Format

An S4 object according to the
[`gridModel`](https://tpetzoldt.github.io/simecol/reference/odeModel.md)
specification. The object contains the following slots:

- `main`:

  functions with the state transition rules.

- `parms`:

  A list with two vector elements:

  `srv`

  :   number of neighbours, necessary to survive,

  `gen`

  :   number of neighbours, necessary to generate a new cell.

- `times`:

  number of time steps to be simulated,

- `init`:

  matrix with the initial state of the cellular grid (default: random).

## Details

To see all details, please have a look into the implementation below.

## References

Gardner, Martin (1970) The Fantastic Combinations of John Conway's New
Solitaire Game 'Life.' *Scientific American*, October 1970.

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
data(conway)
plot(sim(conway))


















## more interesting start conditions
m <- matrix(0, 40, 40)
m[5:35, 19:21] <- 1
init(conway) <- m
plot(sim(conway), col=c("white", "green"), axes = FALSE)


















## change survival rules
parms(conway) <- list(srv = c(3,4), gen = c(3, 4))
plot(sim(conway), col = c("white", "green"), axes = FALSE)

















if (FALSE) { # \dontrun{
require("tcltk")
init(conway) <- matrix(0, 10, 10)
conway <- editInit(conway) # enter some "1"
sim(conway, animate = TRUE, delay = 100)

##============================================
## Implementation:
##   The code of Conways Game of Life
##============================================
conway <- new("gridModel",
  main = function(time, init, parms) {
    x      <- init
    nb     <- eightneighbours(x)
    surviv <- (x >  0 & (nb %in% parms$srv))
    gener  <- (x == 0 & (nb %in% parms$gen))
    x      <- (surviv + gener) > 0
    return(x)
  },
  parms  = list(srv = c(2, 3), gen = 3),
  times  = 1:17,
  init   = matrix(round(runif(1000)), ncol = 40),
  solver = "iteration"
)
} # }
```
