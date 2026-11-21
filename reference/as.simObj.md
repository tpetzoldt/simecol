# Coerce simObj Objects to Lists and Vice-Versa

These functions can be used to coerce (i.e. convert) simecol model
objects (`simObj` objects) to ordinary lists.

## Usage

``` r
# S4 method for class 'list'
as.simObj(x, ...)
# S4 method for class 'simObj'
as.list(x, ...)
## alternative usage:
# as(x, "list")
# as(x, "simObj")
```

## Arguments

- x:

  object to be coerced

- ...:

  for compatibility

## Details

Function `as.list` converts a `simObj` model to an ordinary list with an
additional element 'class' storing the class name of the original
object.

Function `as.simObj` converts in the opposite direction where the type
of the object to be created is determined by a class name stored in the
list element 'class'. If it is missing or contains a non-existing class
name, an error message is printed. Additional list elements which are
not slot names of the corresponding S4 object are omitted.

## See also

[`odeModel`](https://tpetzoldt.github.io/simecol/reference/odeModel.md),
[`new`](https://rdrr.io/r/methods/new.html),
[`as`](https://rdrr.io/r/methods/as.html),
[`as.list`](https://rdrr.io/r/base/list.html),
[`simecol-package`](https://tpetzoldt.github.io/simecol/reference/simecol-package.md)

## Examples

``` r
data(lv3)
llv3 <- as(lv3, "list")
olv3 <- as(llv3, "simObj")

llv3 <- as.list(lv3)
olv3 <- as.simObj(llv3)

dput(as.list(lv3), control="useSource")
#> list(c(0.1, 0.1, 0.1, 0.1, 0.1, 0), c(1, 1, 1), function (time, 
#>     init, parms, inputs) 
#> {
#>     s.in <- approxTime1(inputs, time, rule = 2)["s.in"]
#>     with(as.list(c(init, parms)), {
#>         ds <- s.in - b * s * p + g * k
#>         dp <- c * s * p - d * k * p
#>         dk <- e * p * k - f * k
#>         list(c(ds, dp, dk), s.in = s.in)
#>     })
#> }, c(0, 200, 1), c(0, 99, 100, 101, 200, 0.1, 0.1, 0.5, 0.1, 
#> 0.1), "lsoda", "odeModel")
if (FALSE) { # \dontrun{
## save human readable object representation
dput(as.list(lv3), file="lv3.R", control=c("all"))
## read it back and test it
l_lv3 <- dget("lv3.R")
o_lv3 <- as.simObj(l_lv3)
plot(sim(o_lv3))
} # }
```
