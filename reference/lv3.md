# Lotka-Volterra-Type Model with Resource, Prey and Predator

simecol example: predator prey-model with three equations: predator,
prey and resource (e.g. nutriens, grassland).

## Usage

``` r
data(lv3)
```

## Format

A valid S4 object according to the
[`odeModel`](https://tpetzoldt.github.io/simecol/reference/odeModel.md)
specification. The object contains the following slots:

- `main`:

  Lotka-Volterra equations for predator prey and resource

- `parms`:

  Vector with named parameters of the model:

  `c`

  :   growth rate of the prey population,

  `d`

  :   encounter rate of predator and prey,

  `e`

  :   yield factor (allows conversion with respect to `d`),

  `f`

  :   death rate of the predator population,

  `g`

  :   recycling parameter.

- `inputs`:

  Time series specifying external delivery of resource.

- `times`:

  Simulation time and integration interval.

- `init`:

  Vector with start values for `s`, `p` and `k`.

  `s`

  :   Resource (e.g. grassland or phosphorus).

  `p`

  :   Producer (prey).

  `k`

  :   Consumer (predator).

- `solver`:

  Character string specifying the integration method.

## See also

[`simecol-package`](https://tpetzoldt.github.io/simecol/reference/simecol-package.md),
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
data(lv3)
plot(sim(lv3))

times(lv3)["by"] <- 5    # set maximum external time step to a large value
plot(sim(lv3))           # wrong! automatic time step overlooks internal inputs

plot(sim(lv3, hmax = 1)) # integration with correct maximum internal time step


##============================================
## Implementation:
##   The code of the model
##============================================
lv3 <- new("odeModel",
  main = function(time, init, parms, inputs) {
    s.in <- approxTime1(inputs, time, rule = 2)["s.in"]
    with(as.list(c(init, parms)),{
      ds <- s.in  - b*s*p + g*k
      dp <- c*s*p - d*k*p
      dk <- e*p*k - f*k
      list(c(ds, dp, dk), s.in = s.in)
    })
  },
  parms = c(b = 0.1, c = 0.1, d = 0.1, e = 0.1, f = 0.1, g = 0),
  times  = c(from = 0, to = 200, by = 1),
  inputs = as.matrix(
    data.frame(
      time = c(0,   99, 100,  101, 200),
      s.in = c(0.1, 0.1, 0.5, 0.1, 0.1)
    )
  ),
  init = c(s = 1, p = 1, k = 1), # substrate, producer, consumer
  solver = "lsoda"
)  
```
