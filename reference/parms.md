# Accessor Functions for \`simObj' Objects

Get or set simulation model parameters, main or sub-equations, initial
values, time steps or solvers and extract simulation results.

## Usage

``` r
parms(obj, ...)
parms(obj) <- value

main(obj, ...)
main(obj) <- value

equations(obj, ...)
equations(obj) <- value

init(obj, ...)
init(obj) <- value

inputs(obj, ...)
inputs(obj) <- value

times(obj, ...)
times(obj) <- value

solver(obj, ...)
solver(obj) <- value

#observer(obj, ...)
#observer(obj) <- value

initfunc(obj, ...)
initfunc(obj) <- value

out(obj, ...)
out(obj) <- value
```

## Arguments

- obj:

  A valid `simObj` instance.

- value:

  Named list, vector, function or other data structure (depending on the
  slot and model class) with the same structure as the value returned by
  `parms`. Either all or a subset of values (e.g. single elements of
  vectors or lists) can be changed at once.

- ...:

  Reserved for method consistency.

## Details

These are the accessing functions for `parms`, `times` etc.

Please take care of the semantics of your model when changing slots.
All, element names, data structure and values have to correspond to you
model object definition. For example in `init` the applied names must
exactly correspond to the names and number (!) of state variables. The
restrictions of `parms` or `equations` are less strict (additional
values for “future use” are allowed).

The function `times` allows either to assign or to modify a special
vector with three elements named `from`, `to` and `by` or to overwrite
`times` with an un-named sequence (e.g. `seq(1, 100, 0.1)`.

To ensure object consistency function `out` cannot assign arbitrary
values. It can only extract or delete the contents (by assigning `NULL`)
of the `out`-slot.

## Value

A list, named vector, matrix or function (for `main` slot) or list of
functions (`equation` slot) or other appropriate data structure
depending on the class of the model object.

## See also

General explanation of the slots can be found in
[`simecol-package`](https://tpetzoldt.github.io/simecol/reference/simecol-package.md).

Usage of the `observer` slot is found in the special help file
[`observer`](https://tpetzoldt.github.io/simecol/reference/observer.md).

## Examples

``` r
data(lv)
parms(lv)
#>  k1  k2  k3 
#> 0.2 0.2 0.2 
parms(lv)       <- c(k1 = 0.2, k2 = 0.5, k3 = 0.3)
parms(lv)["k2"] <- 0.5

data(conway)
parms(conway)
#> $srv
#> [1] 2 3
#> 
#> $gen
#> [1] 3
#> 
parms(conway)$srv <- c(2, 2)
parms(conway)
#> $srv
#> [1] 2 2
#> 
#> $gen
#> [1] 3
#> 

## add a new named parameter value
parms(lv)["dummy"] <- 1
## remove dummy parameter
parms(lv) <- parms(lv)[names(parms(lv)) != "dummy"]

## simulation and extraction of outputs
lv <- sim(lv)
o <- out(lv)

## remove outputs from object
out(lv) <- NULL

## store object persistently to the disk
if (FALSE) { # \dontrun{
save(lv, file = "lv.Rdata")           # in binary form
dput(as.list(lv), file = "lv-list.R") # in text form
} # }
```
