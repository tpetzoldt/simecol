# Linear Interpolation with Complete Matrices or Data Frames

Return a data frame, matrix or vector which linearly interpolates data
from a given matrix or data frame.

## Usage

``` r
approxTime(x, xout, ...)
approxTime1(x, xout, rule = 1)
```

## Arguments

- x:

  a matrix or data frame with numerical values giving coordinates of
  points to be interpolated. The first column needs to be in ascending
  order and is interpreted as independent variable (e.g. time), the
  remaining columns are used as dependent variables.

- xout:

  a vector (or single value for `approxTime1`) of independend values
  specifying where interpolation has to be done.

- rule:

  an integer describing how interpolation is to take place outside the
  interval \[min(x), max(x)\]. If `rule` is 1 then `NA`s are returned
  for such points and if it is 2, the value at the closest data extreme
  is used.

- ...:

  optional parameters passed to `approx`.

## Value

`approxTime` returns a matrix resp. data frame of the same structure as
`x` containing data which interpolate the given data with respect to
`xout`. `approxTime1` is a performance optimized special version with
less options than the original `approx` function. It returns an
interpolated vector.

## Details

The functions can be used for linear interpolation with a complete
matrix or data frame. This can be used for example in the main function
of an `odeModel` to get input values at a specified time `xout`. The
version `approxTime1` is less flexible (only one single value for xout
and only linear interpolation) but has increased performance. Both
functions are faster if `x` is a matrix instead of a data frame.

## See also

[`approxfun`](https://rdrr.io/r/stats/approxfun.html)

## Examples

``` r
inputs <- data.frame(time = 1:10, y1 = rnorm(10), y2 = rnorm(10, mean = 50))
input  <- approxTime(inputs, c(2.5, 3), rule = 2)
```
