# Transform Data Between Unconstrained and Box-constrained Scale

These functions can be used to transform a vector of data or parameters
between unconstrained \[-Inf, Inf\] and box-constrained representation
(interval \[lower, upper\]).

## Usage

``` r
p.constrain(p, lower = -Inf, upper = Inf, f = 1)
p.unconstrain(p, lower = -Inf, upper = Inf, f = 1)
```

## Arguments

- p:

  vector of data (e.g. model parameters),

- lower, upper:

  vectors with lower resp. upper bounds used for transformation,

- f:

  optional scaling factor.

## Details

These functions are employed by
[`fitOdeModel`](https://tpetzoldt.github.io/simecol/reference/fitOdeModel.md)
[`ssqOdeModel`](https://tpetzoldt.github.io/simecol/reference/ssqOdeModel.md)
in order to be able to use the unconstrained optimizers of
[`optim`](https://rdrr.io/r/stats/optim.html) for constrained
optimization.

The transformation functions are

\$\$p' = \tan(\pi/2 \cdot (2 p - upper - lower) / (upper - lower)) \cdot
1/f\$\$

and its inverse

\$\$p = (upper + lower)/2 + (upper - lower) \cdot \arctan(f \cdot
p')/\pi\$\$.

## Value

vector with transformed (resp. back-transformed) values.

## References

This trick seems to be quite common, but in most cases it is preferred
to apply optimizers that can handle constraints internally.

Reichert, T. (1998) AQUASIM 2.0 User Manual. Computer Program for the
Identification and Simulation of Aquatic Systems. Swiss Federal
Institute for Environmental Science and Technology (EAWAG), CH - 8600
Duebendorf Switzerland,
<https://www.eawag.ch/de/abteilung/siam/software/>.

## See also

[`fitOdeModel`](https://tpetzoldt.github.io/simecol/reference/fitOdeModel.md),
[`ssqOdeModel`](https://tpetzoldt.github.io/simecol/reference/ssqOdeModel.md)

## Examples

``` r
xx <- seq(-100, 100, 2)
plot(xx, yy<-p.constrain(xx, -20, 45), type="l")
points(p.unconstrain(yy, -20, 45), yy, col="red")
```
