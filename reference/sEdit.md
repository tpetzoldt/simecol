# Simple editing

Simple Editing of Vectors, Lists of Vectors and Other Objects.

## Usage

``` r
sEdit(x, title = "Please enter values:")
```

## Arguments

- x:

  A named object that you want to edit.

- title:

  A title for the dialog box.

## Value

An object with the same type like `x`.

## Details

If called with a vector or list of vectors and if **Tcl/Tk** is
installed, a dialog box is shown in which data can be entered. If the
`x` is not of type vector or list of vectors, a default editing method
is called.

## See also

[`edit`](https://rdrr.io/r/utils/edit.html)
[`editParms`](https://tpetzoldt.github.io/simecol/reference/editParms.md)

## Examples

``` r
if (FALSE) { # \dontrun{
require("tcltk")
## named vector
vec  <- c(a = 1, b = 20, c = 0.03)
new  <- sEdit(vec)
## unnamed vector
sEdit(numeric(10))
## list of vectors
lst <- list(vec = vec, test = 1:10)
sEdit(lst)
## list with numeric and character vectors mixed
lst <- list(vec = vec, test = c("a", "b", "c"))
sEdit(lst)
} # }
```
