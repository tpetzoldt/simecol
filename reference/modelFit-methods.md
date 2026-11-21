# Show Results of Model Fits

Functions to access the results of parameter fits.

## Usage

``` r
# S4 method for class 'modelFit'
coef(object, ...)

# S4 method for class 'modelFit'
deviance(object, ...)

# S4 method for class 'modelFit'
summary(object, ...)

# S4 method for class 'modelFit'
x$name

# S4 method for class 'modelFit'
x[i, j, ..., drop = TRUE]

# S4 method for class 'modelFit'
x[[i, j, ...]]
```

## Arguments

- object, x:

  'modelFit' object from which to extract element(s).

- i, j:

  indices specifying elements to extract. Indices are numeric or
  character vectors or empty (missing) or NULL.

- name:

  a literal character string or a name (possibly backtick quoted). For
  extraction, this is normally partially matched to the names of the
  object.

- drop:

  For matrices and arrays. If TRUE the result is coerced to the lowest
  possible dimension.

- ...:

  other arguments pased to the methods

## See also

[`fitOdeModel`](https://tpetzoldt.github.io/simecol/reference/fitOdeModel.md),
[`Extract`](https://rdrr.io/r/base/Extract.html)
