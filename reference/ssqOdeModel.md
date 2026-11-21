# Sum of Squares Between odeModel and Data

Compute the sum of squares between a given data and an `odeModel`
object.

## Usage

``` r
ssqOdeModel(p, simObj, obstime, yobs, 
  sd.yobs = as.numeric(lapply(yobs, sd)), 
  initialize = TRUE, lower. = -Inf, upper. = Inf, weights = NULL,
  debuglevel = 0, ..., pnames = NULL)
```

## Arguments

- p:

  vector of named parameter values of the model (can be a subset),

- simObj:

  a valid object of class
  [`odeModel`](https://tpetzoldt.github.io/simecol/reference/odeModel.md),

- obstime:

  vector with time steps for which observational data are available,

- yobs:

  data frame with observational data for all or a subset of state
  variables. Their names must correspond exacly with existing names of
  state variables in the
  [`odeModel`](https://tpetzoldt.github.io/simecol/reference/odeModel.md).

- sd.yobs:

  vector of given standard deviations for all observational variables
  given in `yobs`. If no standard deviations are given, these are
  estimated from yobs.

- initialize:

  optional boolean value whether the `simObj` should be re-initialized
  after the assignment of new parameter values. This can be necessary in
  certain models to assign consistent values to initial state variables
  if they depend on parameters.

- lower., upper.:

  named vectors with lower and upper bounds used in the optimisation,

- weights:

  optional weights to be used in the fitting process. Should be `NULL`
  or a data frame with the same structure as `yobs`. If non-NULL,
  weighted least squares is used with `weights` (that is, minimizing
  `sum(w*e^2)`); otherwise ordinary least squares is used.

- debuglevel:

  a positive number that specifies the amount of debugging information
  printed,

- ...:

  additional parameters passed to the solver method (e.g.
  [`lsoda`](https://rdrr.io/pkg/deSolve/man/lsoda.html)),

- pnames:

  names of the parameters, optionally passed from fitOdeModel. This
  argument is a workaround for R versions below 2.8.1. It may be removed
  in future versions of simecol.

## Details

This is the default function called by function
[`fitOdeModel`](https://tpetzoldt.github.io/simecol/reference/fitOdeModel.md).
The source code of this function can be used as a starting point to
develop user-defined optimization criteria (cost functions).

## Value

The sum of squared differences between `yobs` and simulation, by default
weighted by the inverse of the standard deviations of the respective
variables.

## See also

[`fitOdeModel`](https://tpetzoldt.github.io/simecol/reference/fitOdeModel.md),
[`optim`](https://rdrr.io/r/stats/optim.html),
[`p.constrain`](https://tpetzoldt.github.io/simecol/reference/p.constrain.md)

## Examples

``` r
data(chemostat)
cs1 <- chemostat

## generate some noisy data
parms(cs1)[c("vm", "km")] <- c(2, 10)
times(cs1) <- c(from = 0, to = 20, by = 2)
yobs <- out(sim(cs1))
obstime <- yobs$time
yobs$time <- NULL
yobs$S <- yobs$S + rnorm(yobs$S, sd = 0.1 * sd(yobs$S))*2
yobs$X <- yobs$X + rnorm(yobs$X, sd = 0.1 * sd(yobs$X))

## SSQ between model and data
ssqOdeModel(NULL, cs1, obstime, yobs)
#> [1] 0.6925787

## SSQ between model and data, different parameter set
ssqOdeModel(p=c(vm=1, km=2), cs1, obstime, yobs)
#> [1] 70.87387

## SSQ between model and data, downweight second observation
## (both variables)
weights <- data.frame(X=rep(1, nrow(yobs)), S = rep(1, nrow=(yobs)))
ssqOdeModel(p=c(vm=1, km=2), cs1, obstime, yobs, weights=weights)
#> [1] 70.87387

## downweight 3rd data set (row)
weights[3,] <- 0.1
ssqOdeModel(p=c(vm=1, km=2), cs1, obstime, yobs, weights=weights)
#> [1] 70.64388

## give one value double weight (e.g. 4th value of S)
weights$S[4] <- 2
ssqOdeModel(p=c(vm=1, km=2), cs1, obstime, yobs, weights=weights)
#> [1] 71.47724
```
