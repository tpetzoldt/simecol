# Methods for Function plot in Package \`simecol'

Methods for function `plot` in package simecol.

## Usage

``` r
# S4 method for class 'simObj,missing'
plot(x, y, ...)
  # S4 method for class 'odeModel,missing'
plot(x, y, ...)
  # S4 method for class 'odeModel,odeModel'
plot(x, y, ...)
  # S4 method for class 'gridModel,missing'
plot(x, y, index=1:length(x@out), delay=0, ...)
  # S4 method for class 'rwalkModel,missing'
plot(x, y, index=1:length(x@out), delay=0, ...)
```

## Arguments

- x:

  an object of class `simObj`,

- y:

  either a second `odeModel` object or ignored,

- index:

  index of time steps to be plotted,

- delay:

  delay (in ms) between consecutive images (for `gridModel`s) or
  xy-plots (for `rwalkModel`s),

- ...:

  optional plotting parameters.

## Methods

- x = "ANY", y = "ANY":

  Generic function: see
  [`plot`](https://rdrr.io/r/graphics/plot.default.html).

- x = "simObj", ...:

  template function, does nothing and only issues a warning.

- x = "odeModel", ...:

  plots time series of the state variables where one or more `odeModel`
  objects can be supplied. Optional plotting parameters are possible,
  too. See
  [`plot.deSolve`](https://rdrr.io/pkg/deSolve/man/plot.deSolve.html)
  for details.

- x = "gridModel", ...:

  displays a series of images for the simulated grid.

- x = "rwalkModel", ...:

  displays a series of x-y plots of the simulated individuals.
