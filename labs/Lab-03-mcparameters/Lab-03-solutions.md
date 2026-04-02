<!-- # 1. -->
<!-- # 2. -->

# 3. Vary transport parameters: solutions <!-- omit in toc -->

- [3.2. The `slab.egsinp` input file](#32-the-slabegsinp-input-file)
- [3.3. Atomic relaxations](#33-atomic-relaxations)
- [3.4. Spin effects for electron elastic scattering](#34-spin-effects-for-electron-elastic-scattering)
- [3.5. Transport algorithm](#35-transport-algorithm)
- [3.6. Binding effects in Compton scattering](#36-binding-effects-in-compton-scattering)

<!-- ## 3.1. -->

## 3.2. The `slab.egsinp` input file

### What fraction of the incident energy is **reflected**, **deposited**, and **transmitted** ?

The energy fractions are listed as part of the simulation output:

```text
Energy fractions
======================================================
   Reflected:   0.003351 +/- 0.000110   (  3.288798 %)
   Deposited:   0.044540 +/- 0.000580   (  1.302380 %)
   Transmitted: 0.952108 +/- 0.000621   (  0.065206 %)
```

### Which fraction has the highest statistical uncertainty, and why ?

The highest uncertainty is 3.29% on the reflected energy fraction, because
relatively few particles back-scatter from the lead plate, so correspondingly
few reflection events are sampled during this simulation.

### Change the plate material to germanium (you need to figure out how to define a germanium medium). How do the energy fractions change ? Was the simulation faster or slower, and why ?

Results show that more energy is transmitted through germanium that through
lead, which is expected (however at this energy the difference is marginal,
because 1 MeV photons already go through 1 mm of lead at a rate of 95%).

```text
Energy fractions
======================================================
   Reflected:   0.002372 +/- 0.000083   (  3.493151 %)
   Deposited:   0.011805 +/- 0.000257   (  2.180956 %)
   Transmitted: 0.985823 +/- 0.000316   (  0.032021 %)
```

The germanium simulation runs about 10 times faster than with lead, because
fewer electrons are generated and transported. To first order, cross sections
scale with density (and they also typically increase with the atomic number).

### Change the incident particles to electrons to model an electron beam. How do the energy fractions change? Was the simulation faster or slower, and why ?

With electrons the picture is quite different: now most of the energy is
deposited in the plate, a 15% fraction is reflected, and less than 1% goes
through.

```text
Energy fractions
======================================================
   Reflected:   0.152265 +/- 0.000866   (  0.568435 %)
   Deposited:   0.838607 +/- 0.000869   (  0.103653 %)
   Transmitted: 0.009128 +/- 0.000170   (  1.858955 %)
```

The simulation with electrons is *50 times slower* than with photons. As a
general rule, transporting electrons is much more costly than transporting
photons, because electrons are charged particles, which interact with matter
*continuously*.

## 3.3. Atomic relaxations

### Is more energy reflected with relaxation  `On`  or  `Off`, and why ?

About 20% more energy is reflected when atomic relaxations are turned on,
because relaxation is isotropic. Hence relaxation increases the back-scattered
fraction relative to other interactions which generate secondary particles in
the direction of the incident beam, predominantly.

### How large is the difference in the simulation time ? Would you turn it on or off in your own simulations ?

The simulation time increases by about 20% when relaxations are turned on. It is
good practice to start with the most accurate transport parameters possible, and
to turn off some options—selectively—if simulation efficiency becomes an issue.
You should always verify that using a less accurate simulation parameter as no
significant impact on the problem under investigation.

## 3.4. Spin effects for electron elastic scattering

### Is more energy reflected with spin  `On`  or  `Off`, and why ?

There is about 8% more energy reflected when spin effects are turned on. The net
effect of including spin is to decrease the electron range in high-Z materials,
so it akin to increasing the material density. Hence there is less energy
transmitted, and more energy reflected. The opposite is true in low-Z materials;
try it !

### How large is the difference in the simulation time ?

Turning on spin effects in this case increases simulation time by about 35%. The
scattering model is more accurate, but the corresponding calculations more
involved. Look at the *“Number of random numbers used”* in the output: it
increases by about 35% when spin effects are on, explaining the increase in
simulation time.

## 3.5. Transport algorithm

### Is more energy reflected with  `EGSnrc`  transport or with  `PRESTA-I` transport ?

About 2–3% more energy is reflected from the plate when using the EGSnrc
transport algorithm, compared to the legacy PRESTA-I algorithm (the difference
can be shown to be statistically significant if enough histories are simulated).

### Which one do you think is more accurate ?

The EGSnrc transport algorithm is more accurate; there is hardly any valid
argument for choosing PRESTA-I; unless you intend to study the algorithm itself
or reproduce old simulation results, of course.

### How significant is the difference in the simulation time ?

In this scenario, the more accurate EGSnrc algorithm is about 40% slower than
the less accurate PRESTA-I algorithm.

### Do you recover the  `EGSnrc`  result when you decrease  `Ximax` ? What is the difference in simulation time now ?

As you reduce the  `Ximax`  parameter, the results converge towards the EGSnrc
values, however the simulation time increases dramatically. At `Ximax = 0.05`,
 the time is already 4 times that of the EGSnrc algorithm.

## 3.6. Binding effects in Compton scattering

### What is the Compton edge energy for a 200 keV photon ?

The Compton edge energy is the maximum energy an electron can acquire from
photons via the Compton process. The maximum energy transfer occurs for a
scattering angle of $\large \pi$. The final energy $\large k_\text{c}$ of a photon of initial
energy $\large k$ scattered at an angle $\large \theta$ is given by the energy-angle relation:

$$\large k_\text{c} = \frac{k}{1+\frac{k}{m_\text{0}c^2}(1-\cos\theta)} .$$

For $\large k=200$ keV and $\large \theta=\pi$, we find $\large
k_\text{c}=112.18$ keV, hence the Compton edge energy is $\large
200-112.18=87.82$ keV.

### What is the Compton energy for a 200 keV photon scattered at 90 ?

The energy-angle relation yields a photon energy of 143.74 keV. Correspondingly,
the scattered electron then acquires an energy of 56.26 keV.

### Is the Compton edge smoother with or without binding effects, and why ?

The simulation with binding effects  `On`  yields a smoother, more realistic
profile, because the motion of the target atomic electron is taken into account.
This motion induces Doppler-broadening of the Compton edge. By contrast, without
binding the target electron is assumed at rest, and the Compton edge is very
sharp.

### Why is there a marked dip in the Compton profile near 56 keV ?

The Compton profile normally follows a smooth curve up to the Compton edge. Here
we find instead a sharp dip at an electron energy of 56 keV, which corresponds
to a 90Compton scattering angle.

This dip occurs because the plate extends laterally around the beam axis. Photon
scattered at right angles are likely to interact again and *deposit more energy
during the same history*. Such a history will contribute to a higher energy bin
in the spectrum. Accordingly, the energy bins around 56 keV are depleted (and
the photo peak is increased). You can verify that the dip vanishes if you reduce
the size of the plate.

Thus the response of a detector is not only a function of the detector
materials, but may also depend on its geometry, owing to energy angle
constraints, multiple interactions, etc.
