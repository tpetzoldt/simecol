# Find Peaks Within xy-Data

The function returns maxima (values which have only smaller neighbours)
and minima (values which have only larger neighbours).

## Usage

``` r
peaks(x, y=NULL, mode="maxmin")
```

## Arguments

- x, y:

  the coordinates of given points.

- mode:

  specifies if both maxima and minima (`mode="maxmin"`) or only maxima
  (`mode="max"`) or minima (`mode="min"`) are requested.

## Value

A list with x and y coordinates of all peaks.

## See also

[`approx`](https://rdrr.io/r/stats/approxfun.html),
[`upca`](https://tpetzoldt.github.io/simecol/reference/upca.md)

## Examples

``` r
x <- sin(seq(0, 10, 0.1))
plot(x)
points(peaks(x), col="red", pch=15)
```
