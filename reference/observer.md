# Get or Set an Observer Functions to an \`simObj' Object

Get or set a user-defined observer to enable user-specified storage of
simulation results, visualisation or logging.

## Usage

``` r
observer(obj, ...)
observer(obj) <- value
```

## Arguments

- obj:

  A valid `simObj` instance.

- value:

  A function specifying an observer, see Details.

- ...:

  Reserved for method consistency.

## Details

The observer can be used with solver
[`iteration`](https://tpetzoldt.github.io/simecol/reference/iteration.md)
or a user-defined solver function. It does not work with differential
equations solvers.

The observer is a function with the following arguments:

`function(state)`

or:

`function(state, time, i, out, y)`

Where `state` is the actual state of the system, `time` and `i` are the
simulation time and the indexof the time step respectively, `out` is the
output of the actual simulation collected so far. The original object
used in the simulation is passed via `y` and can be used to get access
on parameter values or model equations.

If available, the observer function is called for every time step in the
iteration. It can be used for calculations “on the fly” to reduce memory
of saved data, for user-specified animation or for logging purposes.

If the value returned by observer is a vector, than resulting `out` will
be a `data.frame`, otherwise it will be a list of all states.

## Value

The observer function either modifies `obj` or it returns the assigned
observer function or `NULL` (the default).

## See also

[`iteration`](https://tpetzoldt.github.io/simecol/reference/iteration.md)
for the iteration solver,

[`parms`](https://tpetzoldt.github.io/simecol/reference/parms.md) for
accessor and replacement functions of other slots,

[`simecol-package`](https://tpetzoldt.github.io/simecol/reference/simecol-package.md)
for an overview of the package.

## Examples

``` r

## load model "diffusion"
data(diffusion)

solver(diffusion) # solver is iteration, supports observer
#> [1] "iteration"
times(diffusion) <- c(from=0, to=20, by=1) # to can be increased, to e.g. 100

### == Example 1 ===============================================================

## assign an observer for visualisation
observer(diffusion) <- function(state) {
  ## numerical output to the screen
  cat("mean x=", mean(state$x),
      ", mean y=", mean(state$y),
      ", sd   x=", sd(state$x),
      ", sd   y=", sd(state$y), "\n")
  ## animation
  par(mfrow = c(2, 2))
  plot(state$x, state$y, xlab = "x", ylab = "y", pch = 16, 
    col = "red", xlim = c(0, 100))
  hist(state$y)
  hist(state$x)
  
  ## default case: 
  ## return the state --> iteration stores full state in "out"
  state
}

sim(diffusion)
#> mean x= 44.25396 , mean y= 53.20944 , sd   x= 29.67378 , sd   y= 32.32862 

#> mean x= 44.33079 , mean y= 53.25769 , sd   x= 29.73772 , sd   y= 32.15247 

#> mean x= 44.28854 , mean y= 53.13528 , sd   x= 29.50546 , sd   y= 32.11405 

#> mean x= 44.34423 , mean y= 52.99167 , sd   x= 29.58487 , sd   y= 32.11051 

#> mean x= 44.44161 , mean y= 53.04873 , sd   x= 29.58873 , sd   y= 32.20171 

#> mean x= 42.53854 , mean y= 53.25836 , sd   x= 29.26495 , sd   y= 31.92029 

#> mean x= 42.36339 , mean y= 53.20034 , sd   x= 29.34344 , sd   y= 31.90956 

#> mean x= 44.34944 , mean y= 53.27946 , sd   x= 29.8587 , sd   y= 31.87744 

#> mean x= 44.37142 , mean y= 53.26129 , sd   x= 30.01707 , sd   y= 31.96264 

#> mean x= 46.29693 , mean y= 53.2869 , sd   x= 30.44707 , sd   y= 31.93448 

#> mean x= 44.47387 , mean y= 53.21965 , sd   x= 29.93513 , sd   y= 31.70179 

#> mean x= 42.45092 , mean y= 53.25655 , sd   x= 29.40562 , sd   y= 31.64584 

#> mean x= 42.42536 , mean y= 53.14472 , sd   x= 29.35052 , sd   y= 31.69809 

#> mean x= 42.66636 , mean y= 53.17369 , sd   x= 29.38173 , sd   y= 31.79976 

#> mean x= 44.72225 , mean y= 53.13865 , sd   x= 29.89704 , sd   y= 31.89434 

#> mean x= 42.75005 , mean y= 51.18629 , sd   x= 29.34425 , sd   y= 31.97186 

#> mean x= 44.83282 , mean y= 53.26399 , sd   x= 29.89329 , sd   y= 31.81344 

#> mean x= 44.72462 , mean y= 51.27584 , sd   x= 29.96317 , sd   y= 31.9886 

#> mean x= 44.84279 , mean y= 53.35326 , sd   x= 29.99698 , sd   y= 31.74463 

#> mean x= 46.75703 , mean y= 51.38609 , sd   x= 30.25581 , sd   y= 31.93535 

#> mean x= 44.68726 , mean y= 53.50329 , sd   x= 29.90389 , sd   y= 31.81204 

### == Example 2 ===============================================================

## an extended observer with full argument list
observer(diffusion) <- function(state, time, i, out, y) {
  ## numerical output to the screen
  cat("index =", i,
      ", time =", time,
      ", sd   x=", sd(state$x),
      ", sd   y=", sd(state$y), "\n")
  ## animation
  par(mfrow = c(2, 2))
  plot(state$x, state$y, xlab = "x", ylab = "y", pch = 16, 
    col = "red", xlim = c(0, 100))
  hist(state$y)
  hist(state$x)
  if (is.matrix(out)) # important because out may be NULL for the first call
    matplot(out[,1], out[,-1]) # dynamic graph of sd in both directions
  
  ## return a vector with summary information
  c(times = time, sdx=sd(state$x), sdy=sd(state$y))
}

diffusion <- sim(diffusion)

#> index = 1 , time = 0 , sd   x= 29.37 , sd   y= 29.40661 

#> index = 2 , time = 1 , sd   x= 29.49725 , sd   y= 29.34375 

#> index = 3 , time = 2 , sd   x= 29.48303 , sd   y= 29.38866 

#> index = 4 , time = 3 , sd   x= 29.3076 , sd   y= 29.3267 

#> index = 5 , time = 4 , sd   x= 29.25242 , sd   y= 29.18474 

#> index = 6 , time = 5 , sd   x= 29.29641 , sd   y= 29.15053 

#> index = 7 , time = 6 , sd   x= 29.27311 , sd   y= 28.70712 

#> index = 8 , time = 7 , sd   x= 29.33221 , sd   y= 28.60785 

#> index = 9 , time = 8 , sd   x= 29.48677 , sd   y= 28.94043 

#> index = 10 , time = 9 , sd   x= 29.55326 , sd   y= 28.84654 

#> index = 11 , time = 10 , sd   x= 29.69484 , sd   y= 28.98415 

#> index = 12 , time = 11 , sd   x= 29.68166 , sd   y= 28.51518 

#> index = 13 , time = 12 , sd   x= 29.60715 , sd   y= 28.42839 

#> index = 14 , time = 13 , sd   x= 29.65433 , sd   y= 28.77735 

#> index = 15 , time = 14 , sd   x= 29.57758 , sd   y= 28.79642 

#> index = 16 , time = 15 , sd   x= 29.6634 , sd   y= 28.50845 

#> index = 17 , time = 16 , sd   x= 29.67665 , sd   y= 28.60942 

#> index = 18 , time = 17 , sd   x= 29.86132 , sd   y= 28.96519 

#> index = 19 , time = 18 , sd   x= 29.97567 , sd   y= 28.91508 

#> index = 20 , time = 19 , sd   x= 29.84561 , sd   y= 28.74753 

#> index = 21 , time = 20 , sd   x= 29.77836 , sd   y= 28.90305 


### == Restore default =========================================================

observer(diffusion) <- NULL # delete observer
diffusion <- sim(diffusion)
```
