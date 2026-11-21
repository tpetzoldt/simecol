# Edit \`parms', \`init' or \`times' Slot of \`simecol' Objects

The functions invoke an editor dialog for parameters, initial values or
time steps of `simObj` objects and then assign the new (edited) version
of `x` in the user's workspace. A **Tcl/Tk** version or spreadsheet
editor is displayed if possible, depending on the structure of the
respective slot.

## Usage

``` r
editParms(x)
  editTimes(x)
  editInit(x)
```

## Arguments

- x:

  A valid instance of the `simObj` class.

## See also

[`sEdit`](https://tpetzoldt.github.io/simecol/reference/sEdit.md),
[`simObj`](https://tpetzoldt.github.io/simecol/reference/simecol-package.md),
[`parms`](https://tpetzoldt.github.io/simecol/reference/parms.md),
[`times`](https://tpetzoldt.github.io/simecol/reference/parms.md),
[`init`](https://tpetzoldt.github.io/simecol/reference/parms.md),
[`edit`](https://rdrr.io/r/utils/edit.html)

## Examples

``` r
if (FALSE) { # \dontrun{
require("tcltk")
data(lv)        # load basic Lotka-Volterra model
lv <- editParms(lv)
plot(sim(lv))

data(conway)    # Conway's game of life
init(conway) <- matrix(0, 10, 10)
conway <- editInit(conway) # enter some "1"
sim(conway, animate = TRUE, delay = 100)
} # }
```
