<!-- # 1. -->
<!-- # 2. -->
<!-- # 3. -->
<!-- # 4. -->
<!-- # 5. -->
<!-- # 6. -->

# 7. BEAMnrc variance reduction: solutions <!-- omit in toc -->

- [7.2. Optimization](#72-optimization)
- [7.3. Uniform bremsstrahlung splitting (UBS)](#73-uniform-bremsstrahlung-splitting-ubs)
- [7.4. Directional bremsstrahlung splitting (DBS)](#74-directional-bremsstrahlung-splitting-dbs)
- [7.5. Bremsstrahlung cross section enhancement](#75-bremsstrahlung-cross-section-enhancement)
- [7.6. Range rejection](#76-range-rejection)
- [7.7. Photon forcing](#77-photon-forcing)
- [7.8. ECUT](#78-ecut)
- [7.9. Russian roulette](#79-russian-roulette)

<!-- ## 7.1. Introduction -->

## 7.2. Optimization

### Are the simulation results identical?

> CPU time for EX16MVp on a 2.5 GHz Intel Core i5:
>
> - with `-O3`: **97.6 seconds;**\
> - with `-O0`: **247.8 seconds.**
>
> Fluence and dose results are identical.

## 7.3. Uniform bremsstrahlung splitting (UBS)

### Calculate the efficiency of the simulation without bremsstrahlung splitting, and with UBS.

> | UBS splitting number |       photon $\large ϵ$        |      electron $\large ϵ$       |
> | :------------------: | :----------------------------: | :----------------------------: |
> |          —           | $\large 10^{−4}\text{ s}^{−1}$ | $\large 10^{−4}\text{ s}^{−1}$ |
> |          0           |             0.687              |               0                |
> |          2           |             1.053              |               0                |
> |          10          |             2.221              |             0.025              |
> |          20          |             2.028              |               0                |

## 7.4. Directional bremsstrahlung splitting (DBS)

### Compare to the results obtained with no splitting and with UBS.

> | DBS splitting radius | electron splitting |       photon $\large ϵ$        |  electron $\large ϵ$  |
> | :------------------: | :----------------: | :----------------------------: | :-------------------: |
> |          cm          |         —          | $\large 10^{−4}\text{ s}^{−1}$ | $\large\text{s}^{−1}$ |
> |         10.0         |        off         |             54.061             |           0           |

### Turn on electron splitting at the back of the flattening filter (...).

> | DBS splitting radius | electron splitting |       photon $\large ϵ$        |      electron $\large ϵ$       |
> | :------------------: | :----------------: | :----------------------------: | :----------------------------: |
> |          cm          |         —          | $\large 10^{−4}\text{ s}^{−1}$ | $\large 10^{−4}\text{ s}^{−1}$ |
> |         10.0         |         on         |             29.999             |             0.113              |

### Optional: Vary the splitting radius to 5 cm, 7.5 cm and 30 cm and (...)

> | DBS splitting radius | electron splitting |       photon $\large ϵ$        |      electron $\large ϵ$       |
> | :------------------: | :----------------: | :----------------------------: | :----------------------------: |
> |          cm          |         —          | $\large 10^{−4}\text{ s}^{−1}$ | $\large 10^{−4}\text{ s}^{−1}$ |
> |         10.0         |        off         |             54.061             |               0                |
> |         10.0         |         on         |             29.999             |             0.113              |
> |         5.0          |         on         |             34.372             |             0.044              |
> |         7.5          |         on         |             29.002             |             0.156              |
> |         30.0         |         on         |             15.986             |             0.042              |

## 7.5. Bremsstrahlung cross section enhancement

### At what splitting number is the calculation most efficient? What is the efficiency?

> Highest efficiency is at a splitting number of 2000. Without cross section
> enhancement, $\large \epsilon$ at this splitting number is $\large
> 129.24\times10^{-4}\text{ s}^{-1}$. Here are the efficiencies, on a 2.5 GHz
> Intel Core i5:
>
> | DBS splitting number |           $\large ϵ$           |
> | :------------------: | :----------------------------: |
> |          —           | $\large 10^{−4}\text{ s}^{−1}$ |
> |         100          |             43.963             |
> |         200          |             79.33              |
> |         300          |            108.332             |
> |         500          |            144.891             |
> |         700          |             186.26             |
> |         1000         |             214.86             |
> |         1500         |             248.5              |
> |         2000         |             265.8              |
> |         4000         |             251.54             |

### Optional: What is the efficiency of your calculation at this optimum splitting number? (...)

> Based on the fit to the above data, the optimal splitting number is $\large
> N_\text{s} \sim 2600$. At this splitting number the efficiency of the
> calculation is $\large 230.58\times10^{-4}$. Oh well, you can’t win ’em all.
> Seriously, though, this underscores two important points:
>
> - Efficiency is subject to variations in CPU time which, in turn, depends on
>   load, network traffic, etc.
>
> - Near the optimum splitting number, efficiency does not vary much, so the
>   exact splitting number chosen does not matter as long as it is in the
>   neighbourhood of the optimum value.

## 7.6. Range rejection

### Calculate the relative efficiency with `off`,  `on with set ECUTRR` and  `on with varying ECUTRR`.

> Relative efficiency $\large \epsilon$ (with respect to the `off` case) for
> photon (EX16MVp) and electron (EX10MeVe) fluence efficiency in central scoring
> zone, on a 2.5 GHz Intel Core i5:
>
> | accelerator | range rejection `off` | fixed `ECUTRR` | varying `ECUTRR` |
> | :---------: | :-------------------: | :------------: | :--------------: |
> |   EX16MVp   |           1           |     3.858      |      9.537       |
> |  EX10MeVe   |           1           |     2.074      |      2.293       |

## 7.7. Photon forcing

### Quantify the effects of `Photon forcing` set to `on` in the air past the accelerator head (...).

> Effects of photon forcing in EX16MVp, on a 2.5 GHz Intel Core i5, with photon
> and electron fluence uncertainties:
>
> | photon forcing | CPU time | uncertainty photons | uncertainty electrons | Particle count |     weights      |
> | :------------: | :------: | :-----------------: | :-------------------: | :------------: | :--------------: |
> |                |    s     |          —          |           —           |       —        |        —         |
> |       on       |  103.6   |        4.31%        |         7.99%         |     39491      | 0.0000267 – 0.05 |
> |      off       |  112.8   |        4.59%        |        70.71%         |     18121      |   0.05 – 0.05    |


## 7.8. ECUT

### Find the effect of changing the global electron cutoff energy `ECUT` to 0.8 MeV and 1.0 MeV. (...)

> Results on a 2.5 GHz Intel Core i5; quantities of interest are: central-axis
> photon fluence (EX16MVp) and central axis electron fluence (EX10MeVe):
>
> | accelerator | `ECUT` | CPU time |      quantity of interest      |
> | :---------: | :----: | :------: | :----------------------------: |
> |             |  MeV   |    s     | $\large 10^{−4}\text{ s}^{−1}$ |
> |   EX16MVp   |  0.7   |  225.5   |   0.4501 $\large \pm$ 4.31%    |
> |             |  0.8   |  251.3   |   0.4136 $\large \pm$ 4.55%    |
> |             |  1.0   |  248.9   |   0.4285 $\large \pm$ 4.43%    |
> |  EX10MeVe   |  0.7   |   77.4   |    3.697 $\large \pm$ 5.26%    |
> |             |  0.8   |   73.1   |    4.046 $\large \pm$ 5.02%    |
> |             |  1.0   |   73.1   |    3.933 $\large \pm$ 5.08%    |

## 7.9. Russian roulette

### Quantify the effects of setting the `Russian Roulette` option to `on` or `off` with UBS.

> Results for the central axis fluence, EX16MVp with 20 $\large\times$ UBS and
> no photon interaction forcing, on a 2.5 GHz Intel Core i5:
>
> | Russian roulette |   $\large \epsilon$ photons    |  $\large \epsilon$ electrons   |
> | :--------------: | :----------------------------: | :----------------------------: |
> |                  | $\large 10^{−4}\text{ s}^{−1}$ | $\large 10^{−4}\text{ s}^{−1}$ |
> |       off        |             4.207              |             0.018              |
> |        on        |             11.587             |               0                |

### Do the same with augmented range rejection with DBS.

> Results for central axis fluence, EX16MVp with $100\times$ DBS, splitting
> field radius $=10$ cm, SSD $=100$ cm, electron splitting on, no photon >
> interaction forcing:
>
> | augmented range rejection |   $\large \epsilon$ photons    |  $\large \epsilon$ electrons   |
> | :-----------------------: | :----------------------------: | :----------------------------: |
> |                           | $\large 10^{−4}\text{ s}^{−1}$ | $\large 10^{−4}\text{ s}^{−1}$ |
> |            off            |             51.626             |             0.204              |
> |            on             |             64.353             |             0.242              |
