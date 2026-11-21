# Methods for Function \`print' in Package \`simecol'

Methods for function `print` in Package simecol.

## Usage

``` r
# S4 method for class 'simObj'
print(x, all = FALSE, ...)
```

## Arguments

- x:

  an object of class `simObj` or one of its subclasses.

- all:

  specifies whether all slots are printed. Default is that only
  not-empty slots are printed and the contents of `out` are suppressed.

- ...:

  optional parameters passed to print.

## Methods

- x = "ANY":

  generic function: see [`print`](https://rdrr.io/r/base/print.html).

- x = "simObj":

  prints the contents (slots) of the `simObj` object.
