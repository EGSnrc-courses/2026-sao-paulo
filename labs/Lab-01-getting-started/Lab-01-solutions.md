# 1. Getting started: solutions <!-- omit in toc -->

- [1.2. Write your own egs++ application](#12-write-your-own-egs-application)
- [1.3. Add a dose scoring object](#13-add-a-dose-scoring-object)
- [1.4. Explore the simulation parameters](#14-explore-the-simulation-parameters)
- [1.5. Monitor a simulation in detail](#15-monitor-a-simulation-in-detail)

<!-- ## 1.1. -->

## 1.2. Write your own egs++ application

### Does `myapp` provide any useful information when it runs?

> Not really. The application prints some configuration details and timing
> statistics, but the result is always zero (with 100% uncertainty) because no
> physical quantity has been *scored* yet:
>
> ```text
> Running 1000 histories
>
>   Batch             CPU time        Result   Uncertainty(%)
> ==========================================================
>       1                0.08              0         100.00
>       2                0.16              0         100.00
>       3                0.23              0         100.00
>       4                0.31              0         100.00
>       5                0.40              0         100.00
>       6                0.49              0         100.00
>       7                0.56              0         100.00
>       8                0.64              0         100.00
>       9                0.72              0         100.00
>      10                0.80              0         100.00
>
>
> Finished simulation
>
> Total cpu time for this run:            0.80 (sec.) 0.0002(hours)
> Histories per hour:                     4.5e+06
> Number of random numbers used:          13938479
> Number of electron CH steps:            345747
> Number of all electron steps:           1.2252e+06
> ```

## 1.3. Add a dose scoring object

### How much energy is deposited? What is the dose?

> With the dose scoring object, the simulation now reports a **Summary of region
> dosimetry**. The energy deposited in the tantalum plate is
> $(2.5506 \pm 1.828\\%)$ MeV per incident electron. The dose is
> $(2.4535\times10^{-12} \pm 1.828\\%)$ Gy.
>
> ```text
> ==> Summary of region dosimetry (per particle)
> ir medium      rho/[g/cm3]  V/cm3      Edep/[MeV]              D/[Gy]
> -----------------------------------------------------------------------------
> 1 tantalum  16.654    10.0000 2.5506e+00 +/- 1.828  % 2.4535e-12 +/- 1.828  %
> -----------------------------------------------------------------------------
> ```

### Can you manually convert deposited energy to dose?

> Dose is energy deposited per unit mass, reported in Gray (1 Gy = 1 J/kg). The
> plate has density 16.654 g/cm³ and volume 10 cm³, so its mass is 166.54 g:
>
> $$D = \frac{2.5506 \text{ MeV}}{166.54\text{ g}} = 0.01531 \text{
> MeV/g} = 2.454 \times 10^{-12} \text{ J/kg}$$

### Why is the relative uncertainty the same for energy and dose?

> The relative uncertainty is 1.828% for both. Dose is simply energy divided by
> mass, and the mass comes from simulation inputs (density and volume) which are
> exact constants. Dividing by a constant does not change the relative
> uncertainty.

### Why has the deposited energy not increased by a factor of 10?

> With `ncase = 1e4`, the deposited energy is $(2.5893 \pm 0.572\\%)$ MeV —
> essentially unchanged. This is because Monte Carlo results are reported *per
> incident particle*, not as totals. The relative uncertainty, however,
> decreased by about a factor of 3. This follows the $1/\sqrt{N}$ scaling
> typical of random sampling: $\sqrt{10} \approx 3.2$.

## 1.4. Explore the simulation parameters

### Scenario A — Photons instead of electrons

**Is the simulation faster with electrons or with photons?**

> Setting `charge = 0` switches to photons. The simulation runs about 10 times
> faster because photons interact far less frequently with matter than electrons
> do.

**How did the dose change?**

> The dose decreased by roughly a factor of 10 compared to electrons, consistent
> with fewer interactions in the plate. The uncertainty is also larger, since
> fewer energy deposition events are sampled.

**Are positrons generated? Is this expected?**

> Yes — they appear as blue tracks in `egs_view`. This is expected: the
> incident photons (20 MeV) are well above the pair production threshold of
> 1.022 MeV.

### Scenario B — Lower photon energy

**What is the biggest qualitative difference compared to 20 MeV?**

> There are far fewer secondary charged particles, and no positrons at all.
> This is because the incident energy (1 MeV) is below the pair production
> threshold of 1.022 MeV.

**Did the dose increase or decrease?**

> The dose decreased further, to $(5.509\times 10^{-14} \pm 3.6\\%)$ Gy.

**How did the simulation time change?**

> It decreased. Lower energy means fewer secondary particles to transport,
> which makes the simulation faster.

### Scenario C — Low-energy electrons on lead

**What is happening to the electrons?**

> The electrons are either absorbed in the lead plate or back-scattered in the
> $-z$ direction.

**Is that consistent with the deposited energy?**

> Yes. The deposited energy is $(0.0588 \pm 0.71\\%)$ MeV per incident electron.
> Since the incident energy is 0.1 MeV, only about 60% of the energy stays in
> the plate — the rest is carried away by back-scattered electrons.

**What is the file size of `slab.ptracks`?**

> About 5.3 MB. Track files grow quickly, but by default `egs_track_scoring`
> limits the number of saved events to 1024, regardless of `ncase`.

### Scenario D — Electrons in water (stopping power check)

**Is the deposited energy consistent with the stopping power?**

> The simulation reports about 175 keV deposited per electron. This is *less*
> than the 245 keV expected from the stopping power calculation
> ($2.454 \times 1.0 \times 0.1 = 0.245$ MeV). The difference arises because
> the stopping power gives the *total* energy loss, but some of that energy is
> carried away from the plate by secondary particles (mainly bremsstrahlung
> photons) rather than being deposited locally.
>
> This distinction between energy *lost* and energy *deposited* is fundamental
> in dosimetry.

## 1.5. Monitor a simulation in detail

### What is the most common interaction type?

> Møller scattering dominates (1554 events), followed by bremsstrahlung (297) and
> photoelectric interactions (184). There are also a handful of Rayleigh, Compton
> and Fluorescence events.

### How many electrons initially produce bremsstrahlung vs. a knock-on electron?

> 78 histories (incident electrons) initially undergo Møller scattering, while
> 22 first undergo a bremsstrahlung event.

### What is the largest number of particles on the stack at once?

> The maximum stack size is 5, reached in history #50.
>
> It builds up through a cascade: a photoelectric event produces an electron and
> a fluorescence photon (stack: 3 electrons + 1 photon). That fluorescence
> photon itself undergoes photoelectric absorption, producing another electron
> and another fluorescence photon — bringing the stack to 4 electrons + 1
> photon = 5.

### Are most particles discarded by energy cutoff or by leaving the geometry?

> Energy cutoff is the dominant mechanism: Most particles are discarded because
> their energy falls below the threshold, compared to a relatively small number
> that exit the geometry.
