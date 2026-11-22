# Add Functions from a Non-nested List of Named Functions to a Common Environment

Create and set an environment where elements (e.g. functions) within a
non-nested named list of functions see each other. This function is
normally used within other functions.

## Usage

``` r
addtoenv(L, p = parent.frame())
```

## Arguments

- L:

  a non-nested list of named functions.

- p:

  the environment where the functions are assigned to. Defaults to the
  parent frame.

## Details

This function is used by the \`solver functions' of `simecol`.

## Value

The list of functions within a common environment.

## Note

This is a very special function that uses environment manipulations. Its
main purpose is to \`open' the access to interdependend functions within
a common list structure (function list).

## See also

[`attach`](https://rdrr.io/r/base/attach.html),
[`environment`](https://rdrr.io/r/base/environment.html)

## Examples

``` r
eq <- list(f1 = function(x, y)    x + y,
           f2 = function(a, x, y) a * f1(x, y)
          )

fx <- function(eq) {
  eq <- addtoenv(eq)
  print(ls())
  print(environment(eq$f1))
  f1(3,4) + f2(1,2,3)
}

fx(eq)
#> [1] "eq" "f1" "f2"
#> <environment: 0x5a08e490edd0>
#> [1] 12
## eq$f2(2,3,4)       # should give an error outside fx
environment(eq$f2)    # should return R_GlobalEnv again
#> <environment: 0x5a08e490edd0>
```
