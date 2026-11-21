# Generate Plackett Bivariate Random Numbers

Generate bivariate uniform random numbers according to the Plackett
distribution.

## Usage

``` r
pcu(x, alpha = rho2alpha(rho), rho)
pcuseries(n, alpha = rho2alpha(rho), rho, min = 0, max = 1)
alpha2rho(alpha)
rho2alpha(rho)
```

## Arguments

- n:

  number of observations.

- x:

  vector of uniformly \[0, 1\] distributed real numbers.

- alpha:

  association coefficient of the Plackett distribution.

- rho:

  Pearson correlation coefficient.

- min, max:

  lower and upper limits of the distribution. Must be finite.

## Details

The functions can be used to generate bivariate distributions with
uniform marginals. Function `pcu` generates a vector of uniform random
values of `length(x)` which are correlated to the corresponding vector
`x`, `pcuseries` generates an auto-correlated series, and `alpha2rho`
resp. `rho2alpha` convert between the Pearson correlation coefficient
and the association measure of the Plackett distribution.

## References

Johnson, M., Wang, C., & Ramberg, J. (1984). Generation of multivariate
distributions for statistical applications. *American Journal of
Mathematical and Management Sciences*, **4**, 225-248.

Nelsen, R. B. (2006). *An Introduction to Copulas*. Springer, New York.

## See also

[`runif`](https://rdrr.io/r/stats/Uniform.html)

## Examples

``` r
x <- runif(100)
y <- pcu(x, rho = 0.8)
plot(x, y)

cor(x, y)
#> [1] 0.8214562

x <- pcuseries(1000, rho=0.8)
plot(x, type="l")

acf(x)

pacf(x)
```
