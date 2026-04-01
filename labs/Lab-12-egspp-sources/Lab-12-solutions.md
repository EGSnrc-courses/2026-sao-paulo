<!-- # 1. -->
<!-- # 2. -->
<!-- # 3. -->
<!-- # 4. -->
<!-- # 5. -->
<!-- # 6. -->
<!-- # 7. -->
<!-- # 8. -->
<!-- # 9. -->
<!-- # 10. -->
<!-- # 11. -->

# 12. Investigate egs++ sources and ausgab scoring objects: solutions <!-- omit in toc -->

- [12.2. Running a simulation using an isotropic source](#122-running-a-simulation-using-an-isotropic-source)
- [12.3. Running the simulation using a collimated source](#123-running-the-simulation-using-a-collimated-source)


<!-- ## 12.1. -->

## 12.2. Running a simulation using an isotropic source

### Compare the energy deposited reported by the `egs_dose_scoring` object and the one corresponding to the fraction of energy deposited reported by `tutor7pp`. Are they the same ?

`tutor7pp` reports the total energy entering the geometry, $\large E_\text{tot}$, and
the fractions, $\large f_i$, of that energy deposited in the different regions $\large i$ of
the geometry. In this case there is only one region. The energy deposited in
that region, $\large E_\text{dep}$, is obtained as

$$\large E_\text{dep} = E_\text{tot} \cdot f_0 = 10^4\ \text{MeV} \cdot 0.1934056 = 1934.056\ \text{MeV}$$

The `egs_dose_scoring` scoring object reports the dose deposited in each
region $\large i$ divided by the source's normalization which could be the
number of particles emitted or the fluence. The isotropic point source
uses the number of particles emitted as normalization. Hence to obtain
the energy deposited in the geometry, one has to multiply the reported
energy deposited per particle $\large \left(\frac{E_\text{dep}}{N}\right)$ by
the number of particles emited, $\large N$ (`last case`):

$$\large E_\text{dep} = \left(\frac{E_\text{dep}}{N}\right) \cdot N = 1.2082 \cdot 10^{-5}\ \text{MeV} \cdot 160072124 = 1933.991\ \text{MeV}$$

Note above that 10$^5$ is the number of particles hitting the geometry !

### What is the solid angle, $\large \Omega$, subtended by the isotropic source and
the front face of the geometry ?

The number of particles emited by the isotropic point source in $\large 4\pi$ is
160072124, while only 10$^5$ of these particles actually hit the front face of
the cylinder. Hence $\large \Omega$ can be obtained from

$$\large \Omega = \frac{10^5}{160072124}\cdot 4\pi = 0.00785\ \text{radians}$$

Compute the efficiency $\large \varepsilon$ based on the uncertainty in the
energy deposited calculated with the `egs_dose_scoring` object and take note of
it for a later comparison with the collimated source.

$$\large \varepsilon = \frac{1}{t_\text{CPU}\cdot \sigma^2} = \frac{1}{0.482^2 \cdot 67.67 s} = 0.0636\ s^{-1}$$

## 12.3. Running the simulation using a collimated source

### Compare the *fraction* of energy deposited with the result obtained for the isotropic point source. Are they the same ? Explain !

Although the normalization used by both sources is different, the fraction of
energy deposited in the geometry, $\large f_0$, is the same within statistics
since as a relative quantity, it should be independent of the normalization.

$$\large f_0^\text{isotropic} = 0.1934 ,\quad f_0^\text{collimated} = 0.1948$$

### Compare the energy deposited reported by the `egs_dose_scoring` object with the same result obtained earlier for the isotropic point source. Are they the same ? Explain !

The collimated source uses the fluence $\large F$ as normalization. For this
reason, the values reported by the `egs_dose_scoring` in this case will be
numerically different from the value reported for the isotropic source:

$$\large \frac{E_\text{dep}}{F} = 1.5272 \text{MeV cm}^2\text{ rad.}$$

To get the absolute energy deposited in the geometry, $\large E_\text{dep}$,
one must use the following expression

$$\large E_\text{dep} = \frac{E_\text{dep}}{F} \cdot \frac{N_{\Omega}}{(\Omega \cdot d^2)} = 1.5272 \cdot \frac{10^5}{0.00785 \cdot 10^4} \text{MeV} = 1945.478\ \text{MeV}$$

This value differs by 0.6 % from the value obtained for the isotropic point
source which is within the combined uncertainties of the calculations.

#### A side note

> The value reported in this case for the total energy, called here $\large
> E_\text{tot}'$, does not correspond to the actual total energy $\large
> E_\text{tot}$. The quantity $\large E_\text{tot}'$ is calculated in `tutor7pp`
> as
>
> $$\large E_\text{tot}' = \sum\limits_{i=1}^{N_{\Omega}}{w_i' e_i}$$
>
> which for the collimated source actually corresponds to
>
> $$\large E_\text{tot}' = E_\text{tot}\cdot \Omega\ .$$
>
> Since the purpose of `tutor7pp` is to calculate the energy fractions,
> normalization is irrelevant. To get the actual total energy deposited $\large
> E_\text{dep}$ from $\large E_\text{tot}'$, one needs to use the expression
>
> $$\large E_\text{dep} = \frac{E_\text{tot}' \cdot f_0}{\Omega} = \frac{78.3926\ \text{MeV\ rad} \cdot 0.19482}{0.00785\ \text{rad}} = 1945.535\ \text{MeV}$$
>
> This value is in excellent agreement with the value obtained above from the
> output of `egs_dose_scoring`.

### Compute the efficiency based on the uncertainty in the energy deposited calculated with the `egs_dose_scoring` object and compare it with the efficiency of the isotropic source. Do you see any difference ?

$$\large \varepsilon = \frac{1}{t_\text{CPU}\cdot \sigma^2} = \frac{1}{0.362^2 \cdot 3.02\ s} = 2.5268\ s^{-1}$$

The collimated source is about **40 times more efficient** than the isotropic
point source !
