# Mix Two Named Vectors, Resolving Name Conflicts

The function mixes two named vectors. The resulting vectors contains all
elements with unique name and only one of the two versions of the
elements which have the same name in both vectors.

## Usage

``` r
mixNamedVec(x, y, resolve.conflicts = c("x", "y"), warn = TRUE)
```

## Arguments

- x:

  first named vector,

- y:

  second named vector,

- resolve.conflicts:

  name of the vector from which all elements are taken,

- warn:

  an indicator if a warning should be given if elements are not unique.
  This argument should usually set to FALSE, but the default is TRUE to
  be on the safe side.

## Value

a vector with all elements from one vector and only these elements of
the second, that have a unique name not contained in the other vector.

## Author

Thomas Petzoldt

## See also

[`which`](https://rdrr.io/r/base/which.html)

## Examples

``` r
x <- c(a=1, b=2, c=3)
y <- c(a=1, b=3, d=3)

mixNamedVec(x, y)
#> Warning: name duplicates found: “a”, “b”
#> d a b c 
#> 3 1 2 3 
mixNamedVec(x, y, resolve.conflicts="x")
#> Warning: name duplicates found: “a”, “b”
#> d a b c 
#> 3 1 2 3 

mixNamedVec(x, y, resolve.conflicts="x", warn=FALSE)
#> d a b c 
#> 3 1 2 3 
mixNamedVec(x, y, resolve.conflicts="y", warn=FALSE)
#> c a b d 
#> 3 1 3 3 

## without names, returns vector named in "resolve conflicts"
x <- as.vector(x)
y <- as.vector(y)
mixNamedVec(x, y)
#> [1] 1 2 3
mixNamedVec(x, y, resolve.conflicts="y")
#> [1] 1 3 3

## names partly
x <- c(4, a=1, b=2, c=3, 4, 9)
y <- c(a=1, 6, b=3, d=3, 8)

mixNamedVec(x, y)
#> Warning: name duplicates found: “”, “a”, “b”, “”, “”
#> d   a b c     
#> 3 4 1 2 3 4 9 
mixNamedVec(x, y, resolve.conflicts="y")
#> Warning: name duplicates found: “”, “a”, “b”, “”, “”
#> c a   b d   
#> 3 1 6 3 3 8 
```
