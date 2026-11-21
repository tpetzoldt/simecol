# Lotka-Volterra Predator-Prey Model

simecol example: basic Lotka-Volterra predator prey-model.

## Usage

``` r
data(lv)
```

## Format

An S4 object according to the
[`odeModel`](https://tpetzoldt.github.io/simecol/reference/odeModel.md)
specification. The object contains the following slots:

- `main`:

  Lotka-Volterra equations for predator and prey.

- `parms`:

  Vector with the named parameters of the model:

  `k1`

  :   growth rate of the prey population,

  `k2`

  :   encounter rate of predator and prey,

  `k3`

  :   death rate of the predator population.

- `times`:

  Simulation time and integration interval.

- `init`:

  Vector with start values for predator and prey.

## Details

To see all details, please have a look into the implementation.

## References

Lotka, A. J. 1925. *Elements of physical biology.* Williams and Wilkins,
Baltimore.

Volterra, V. (1926). Variazionie fluttuazioni del numero d'individui in
specie animali conviventi. *Mem. Acad.Lincei*, **2**, 31-113.

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
data(lv)
print(lv)
#> An object of class odeModel 
#> Slot “main”:
#> function (time, init, parms) 
#> {
#>     x <- init
#>     p <- parms
#>     dx1 <- p["k1"] * x[1] - p["k2"] * x[1] * x[2]
#>     dx2 <- -p["k3"] * x[2] + p["k2"] * x[1] * x[2]
#>     list(c(dx1, dx2))
#> }
#> attr(,"source")
#> [1] "function (time, init, parms) {"                         
#> [2] "    x <- init"                                          
#> [3] "    p <- parms"                                         
#> [4] "    dx1 <-   p[\"k1\"] * x[1] - p[\"k2\"] * x[1] * x[2]"
#> [5] "    dx2 <- - p[\"k3\"] * x[2] + p[\"k2\"] * x[1] * x[2]"
#> [6] "    list(c(dx1, dx2))"                                  
#> [7] "  }"                                                    
#> 
#> Slot “times”:
#>  from    to    by 
#>   0.0 100.0   0.5 
#> 
#> Slot “init”:
#>     prey predator 
#>      0.5      1.0 
#> 
#> Slot “parms”:
#>  k1  k2  k3 
#> 0.2 0.2 0.2 
#> 
#> Slot “solver”:
#> [1] "rk4"
#> 
#> Hint: use print(x, all=TRUE) to see all simObj slots.
plot(sim(lv))


parms(lv) <- c(k1=0.5, k2=0.5, k3=0.5)
plot(sim(lv))


##============================================
## Implementation:
##   The code of the Lotka-Volterra-model
##============================================
lv <- new("odeModel",
  main = function (time, init, parms) {
    x <- init
    p <- parms
    dx1 <-   p["k1"] * x[1] - p["k2"] * x[1] * x[2]
    dx2 <- - p["k3"] * x[2] + p["k2"] * x[1] * x[2]
    list(c(dx1, dx2))
  },
  parms  = c(k1=0.2, k2=0.2, k3=0.2),
  times  = c(from=0, to=100, by=0.5),
  init   = c(prey=0.5, predator=1),
  solver = "rk4"
)
```
