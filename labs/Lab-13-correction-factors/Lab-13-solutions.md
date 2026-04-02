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
<!-- # 12. -->

# 13. Calculate chamber correction factors with `egs_chamber`: solutions <!-- omit in toc -->

- [13.8. Run the simulation](#138-run-the-simulation)
- [13.9. Run EGSnrc in parallel](#139-run-egsnrc-in-parallel)
- [13.10. Calculate a chamber correction factor](#1310-calculate-a-chamber-correction-factor)

<!-- ## 13.1. -->
<!-- ## 13.2. -->
<!-- ## 13.3. -->
<!-- ## 13.4. -->
<!-- ## 13.5. -->
<!-- ## 13.6. -->
<!-- ## 13.7. -->

## 13.8. Run the simulation

### What is the dose to the air cavity of the 3C chamber, and its uncertainty ?

> The exact numerical output of the simulation will depend on the random numbers
> you selected and on the computer hardware (because floating-point errors are
> different on different architectures). Let's consider a sample output:
>
> ```text
> Running 10000000 histories
>
>   Batch             CPU time        Result   Uncertainty(%)
> ==========================================================
>       1                2.73    4.44129e-12           2.14
>       2                5.44    4.58095e-12           1.52
>       3                8.15    4.56477e-12           1.24
>       4               10.86    4.50218e-12           1.07
>       5               13.58     4.5057e-12           0.96
>       6               16.30    4.50286e-12           0.88
>       7               19.00    4.51235e-12           0.81
>       8               21.69    4.50094e-12           0.76
>       9               24.42     4.4799e-12           0.72
>      10               27.12    4.48469e-12           0.68
> ```
>
> ```text
> Finished simulation
>
> Total cpu time for this run:            27.12 (sec.) 0.0075(hours)
> Histories per hour:                     1.32743e+09
> Number of random numbers used:          602898842
> Number of electron CH steps:            2.93854e+07
> Number of all electron steps:           3.29099e+07
>
>
>  last case = 10000000 fluence = 1.68487e+06
>
> Geometry                        Cavity dose
> -----------------------------------------------
> 3C                        4.4848e-12 +/- 0.680  %
> ```
>
> In this example, the dose to the 3C air cavity is
> **$\large \mathbf{4.4848\times10^{-12} \pm 0.680}$ % Gy** *per incident particle.*
> Your result should be consistent with this value, considering the 0.680 %
> statistical uncertainty.

### How much CPU time would be required to reduce the uncertainty by a factor 10 ?

> Typically, the uncertainty in a Monte Carlo simulation (a random sampling
> process) decreases like $\large 1/\sqrt{N}$, with $\large N$ the number of independent
> histories (samples). The 3C simulation example above shows that it took 27
> seconds to run 10$^7$ histories to reach an uncertainty of 0.680 %. To reduce
> the uncertainty by a factor of 10 would require 100 times as many histories, ,
> about 2700 seconds (45 minutes).

### What is the efficiency of this simulation ?

> The efficiency $\large \epsilon = 1/(\sigma^2 t)$ provides a robust metric of
> simulation *speed*—which is independent of the number of histories—by
> combining the simulation time $\large t$ and the attained statistical uncertainty
> $\large \sigma$. The 3C simulation example above yields:
>
>$$\large \epsilon = \frac{1}{0.00680^2\cdot 27.12} \approx 800 \text{ s}^{-1}$$
>
> Here we used the *relative* uncertainty in the calculation. You can use the
> absolute uncertainty if you prefer (this amounts to scaling the result), as
> long as you use one or the other consistently.
>
> This number means that in this simulation the relative variance is expected to
> be about $\large 1/(800t)$. So we know that after 100 seconds, for example, the
> relative variance will be $\large 1.25 \times 10^{-5}$, or $\large \sigma \approx 0.35$ %.

### Compare the dose to the one obtained with CAVRZnrc previously. Is it the same ?

`...`

## 13.9. Run EGSnrc in paralell

### How does the efficiency change when running your simulation on a single thread versus 10 threads?

The efficiency improves significantly, increasing by approximately a factor of
10 when using 10 threads compared to a single thread.

### Is the calculated dose numerically the same when running on a single thread versus multiple threads?

Yes, the calculated dose is numerically identical regardless of whether the
simulation is run on a single thread or multiple threads. Parallelization does
not alter the computation; it simply distributes the workload (the total number
of histories) across the available threads.

### Find the optimal number of threads. Try to run with more threads than are available on your machine. What happens?

The `egs-parallel` script limits the number of threads to match the available
system resources. Increasing the thread count beyond this number causes threads
to compete for CPU resources, leading to reduced efficiency. This drop in
efficiency is due to the overhead introduced by context switching, where the CPU
has to spend additional time to switch between threads instead of calculating.

To experiment with efficiency degradation when the number of threads exceeds the
available CPU cores, you *could* comment out [lines
119–122](https://github.com/nrc-cnrc/EGSnrc/blob/fe83057e6db99768a281e343498e3eedf185206d/HEN_HOUSE/scripts/egs-parallel-cpu#L119-L122)
in the `HEN_HOUSE/scripts/egs-parallel-cpu` script.

## 13.10. Calculate a chamber correction factor

### What is the dose to the air cavity in the aluminum chamber, and its uncertainty ?

> Here is a sample output for the 3C aluminum chamber simulation:
>
> ```text
>   Batch             CPU time        Result   Uncertainty(%)
> ==========================================================
>       1                6.01    4.75262e-12           2.35
>       2               12.10    4.77143e-12           1.65
>       3               18.13    4.79307e-12           1.34
>       4               24.17    4.82215e-12           1.16
>       5               30.21     4.8458e-12           1.04
>       6               36.20     4.8355e-12           0.95
>       7               42.19      4.847e-12           0.88
>       8               48.24    4.84485e-12           0.82
>       9               54.21    4.87064e-12           0.77
>      10               60.18    4.88447e-12           0.73
>
> Total cpu time for this run:            60.18 (sec.) 0.0167(hours)
> Histories per hour:                     5.98205e+08
> Number of random numbers used:          1569059014
> Number of electron CH steps:            8.37547e+07
> Number of all electron steps:           8.78651e+07
>
>
>  last case = 10000000 fluence = 1.68487e+06
>
> Geometry                        Cavity dose
> -----------------------------------------------
> 3C_alu                    4.8845e-12 +/- 0.735  %
> ```
>
> Hence the dose to the cavity is now $\large 4.8845\times10^{-12} \pm
> 0.735 \%$ Gy per incident particle. The efficiency has decreased to about
> 300 $\text{s}^{-1}$. Simulations in denser materials are often less efficient: not
> all the extra electrons generated contribute to dose.

### Is it different from the dose in a graphite chamber ?

> Yes, the dose deposited in the air cavity is affected by the material of
the ionization chamber wall.

### What is the calculated correction factor for dose between a graphite 3C chamber and an aluminum 3C chamber ?

> The ratio between the dose in the graphite chamber and the dose in the
> aluminum chamber is $\large \mathbf{D_\text{\textbf{C}}/D_\text{\textbf{Al}} = 0.918
> \pm 1\text{\%}}$. The difference from unity is statistically significant. If
> dose is measured using an aluminum 3C chamber, the result has to be multiplied
> by 0.918 before it is compared to a graphite chamber measurement.

### Repeat the simulation for a PMMA chamber, and answer the same questions.

> Here is a sample output for a 3C pmma chamber simulation:
> ```
>    Batch             CPU time        Result   Uncertainty(%)
> ==========================================================
>       1                2.25    4.46915e-12           2.27
>       2                4.49    4.43651e-12           1.56
>       3                6.73    4.39535e-12           1.26
>       4                8.98    4.41966e-12           1.09
>       5               11.24    4.41883e-12           0.98
>       6               13.49     4.4286e-12           0.89
>       7               15.74    4.40399e-12           0.82
>       8               17.98    4.39829e-12           0.77
>       9               20.22    4.39479e-12           0.73
>      10               22.48    4.38887e-12           0.69
>
>    Finished simulation
>
>    Total cpu time for this run:            22.48 (sec.) 0.0062(hours)
>    Histories per hour:                     1.60142e+09
>    Number of random numbers used:          472773580
>    Number of electron CH steps:            2.22635e+07
>    Number of all electron steps:           2.5682e+07
>
>
>     last case = 10000000 fluence = 1.68487e+06
>
>    Geometry                        Cavity dose
>    -----------------------------------------------
>    3C_pmma                   4.3889e-12 +/- 0.689  %

> The dose to the cavity is **$\large \mathbf{4.3889\times10^{-12} \pm 0.689}$ % Gy**
> per incident particle. The efficiency is now higher than in the graphite
> chamber, at about 940 $\text{s}^{-1}$. The same statistical uncertainty was reached
> in a shorter simulation time.
>
> The ratio between the dose in the graphite chamber and the dose in the pmma
> chamber is $\large \mathbf{D_\text{\textbf{C}}/D_\text{\textbf{pmma}} = 1.02 \pm
> 1\text{\%}}$. The statistical significance of the result here is less clear,
> as we are within $\large 2\sigma$ of unity. It would be prudent to run longer
> simulations to reduce the statistical uncertainty on the ratio, in order to
> confirm the correction factor.
