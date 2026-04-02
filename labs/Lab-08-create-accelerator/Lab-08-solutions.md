<!-- # 1. -->
<!-- # 2. -->
<!-- # 3. -->
<!-- # 4. -->
<!-- # 5. -->
<!-- # 6. -->
<!-- # 7. -->

# 8. Create your own accelerator: solutions <!-- omit in toc -->

- [8.1. Build and run your accelerator model](#81-build-and-run-your-accelerator-model)

## 8.1. Build and run your accelerator model

### What CMs are needed?

> For the example accelerator in this lab, six component
> modules are required:
>
> 1. `SLABS` for the exit window and target
> 2. `FLATFILT` for the target / collimator / flattening filter assembly
> 3. `CHAMBER` for the monitor chamber
> 4. `MIRROR` for the mirror
> 5. `JAWS` for the jaws
> 6. `SLABS` again for the air between the jaws and the SSD.

### Where is most of the energy deposited in your accelerator?

> Most of the energy is deposited in the tungsten collimator.

### How many particles are in any given scoring plane per incident particle?

> With:
>
> - DBS turned on;
> - electron splitting at the bottom of the flattening filter (​$\large z = 9.479\text{ cm}$​) with radial redistribution of particles; and
> - a Russian roulette plane at $\large z=9.0\text{ cm}$.
>
> this linac produces, at SSD:
>
> - 3.92 total particles *per incident history*
> - 3.85 photons *per incident history.*

### How long does it take on average to run one history?

> 0.63 millisecond per history (on a 2.5 GHz Intel Core i5).

### For which CM do you find it most difficult to prepare the input?

> `FLATFILT`!
