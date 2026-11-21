# Simulation of 'simObj' model objects

This function provides the core functionality of the \`simecol' package.
Several methods depending on the class of the model are available.

## Usage

``` r
sim(obj, initialize=TRUE, ...)
  # sim(obj, animation=FALSE, delay=0, ...)
```

## Arguments

- obj:

  an object of class `simObj` or one of its subclasses.

- initialize:

  re-initialize the object if the object contains a user-defined
  initializing function (`initfunc`). Setting `initialize` to `FALSE`
  can be useful to avoid time-consuming computations during
  initialization or to reproduce simulations of models which assign
  random values during the initialization process.

- animation:

  logical value to switch animation on (for classes `gridModel` and
  `rwalkModel`.

- delay:

  delay (in ms and in addition to the time needed for the simulation)
  between consecutive images (for `gridModel`s) or xy-plots (for
  `rwalkModel`s).

- ...:

  optional parameters passed to the solver function (e.g. `hmax` for
  `lsoda`).

## Methods

- obj = "simObj":

  simulates the respective model object with optional animation.

- obj = "odeModel":

  simulates the respective model object.

- obj = "indbasedModel":

  simulates the respective model object with optional animation.

- obj = "gridModel":

  simulates the respective model object with optional animation.

## Details

`sim` re-initializes the model object (if `initialize==TRUE` and calls
the appropriate solver, specified in the `solver`-slot. Objects of class
`rwalkModel` and `indbasedModel` are simulated by the default `simObj`
method. If you derive own sublasses from `simObj` it may be neccessary
to write an appropriate `sim` method and/or solver function.

## Value

The function returns the complete `simObj` instance with the simulation
results in the `out` slot.

## Examples

``` r
data(lv)
plot(sim(lv))


lv2 <- lv
parms(lv2)["k1"] <- 0.5
lv2 <- sim(lv2)
plot(out(lv2))
```
