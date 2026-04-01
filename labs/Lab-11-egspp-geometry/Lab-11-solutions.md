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

# 11. Build a chamber model in egs++: solutions <!-- omit in toc -->

- [11.2. Visualize your geometry with `egs_view`](#112-visualize-your-geometry-with-egs_view)
- [11.3. Dig in with clipping planes](#113-dig-in-with-clipping-planes)
- [11.4. Build an egs++ model of the 3C chamber](#114-build-an-egs-model-of-the-3c-chamber)
- [11.5. Build an egs++ model of an Exradin A12 chamber](#115-build-an-egs-model-of-an-exradin-a12-chamber)
- [11.6. Compare with a chamber modelled with a mesh](#116-compare-with-a-chamber-modelled-with-a-mesh)

<!-- ## 11.1. (no questions) -->

## 11.2. Visualize your geometry with `egs_view`

### What are the region numbers inside and outside the cube?

> When hovering the mouse over the cube, a small red square appears in the top
> left corner of the image window, indicating that the inside of the cube is
> **region 0.**
>
> The region *outside* the cube is not displayed in the list. In egs++
> everything outside the defined geometry is **region −1.** Particles that reach
> region −1 in the simulation are immediately discarded: they have reached the
> “end of the world”!

## 11.3. Dig in with clipping planes

### What is the list of regions when you hover your mouse over the small central sphere?

> When hovering the mouse over the central sphere — with a clipping plane as
> shown in the figure — the list of regions is **{1, 0, 1, 2}**. These are all
> the regions *under* the mouse pointer, from the nearest to the farthest.
> Regions clipped by clipping planes are not included in the list.

### What is the region number *outside* the defined geometry?

> As always in egs++, the space outside the defined geometry is **region −1**,
> and it is not included in the list of regions in the image window.

## 11.4. Build an egs++ model of the 3C chamber

### What are the region numbers which correspond to the air cavity?

> The air cavity comprises **regions 3 and 7.**

### Where are regions 1 and 2?

> Regions 1 and 2 are not visible in this geometry: they are *virtual* regions.
> In some cases, egs++ attributes region numbers in a way that is more efficient
> internally. But the algorithm is deterministic: the same geometry always
> yields the same region numbers.

### Can you figure out the conestack region numbering scheme?

>In the case of the conestack, egs++ attributes a fixed number of regions to
>each layer. This is more efficient because only a single modulo operation is
>then needed to convert the region number to the layer index.
>
>In the 3C conestack, there are at most three radii per layer, so egs++
>attributes 3 regions per layer, whether or not they are used. The first layer
>has only one radial dimension — region 0 — hence regions 1 and 2 are not used:
>they become *virtual* regions and don’t show up in the viewer. The next layer
>in the conestack then starts at region 3, and the same logic applies the
>virtual region 5.

## 11.5. Build an egs++ model of an Exradin A12 chamber

### What are the region numbers that correspond to the air cavity?

> The air cavity comprises **regions 1, 4 and 22.**

### Why is the region for the spherical tip of the air cavity so large?

> The spherical tip of the air cavity is region 22, because the conestack is
> added first in the cd geometry. Therefore, region numbers are attributed first
> to the conestack (regions 0 to 20, for 7 layers $\large\times$ 3 regions per
> layer), and only then to the spherical regions in the chamber tip.

### How many regions are there in total in this model? How many *real* regions?

> There are **24 regions** in this chamber model, ranging from 0 to 23. However,
> there are many *virtual* regions in the chamber stem, where there is a single
> radius per conestack layer. There are 10 virtual regions (7, 8, 10, 11, 13,
> 14, 16, 17, 19, 20), hence there are only **14 real regions** in the geometry.

### What is the largest region number in this geometry?

> The largest region number in this geometry is **23** (the chamber tip shell).
> Keep in mind that region numbering starts from 0, so if a geometry contains
> $\large n$ regions, the largest region number is $\large n-1$.

## 11.6. Compare with a chamber modelled with a mesh

### For the same number of histories, which model takes longer to simulate, and why?

> The mesh model takes longer to simulate due to the vastly larger number of
> regions. It contains 1.5 million regions, compared to only 24 in the simple
> model. This illustrates one of the major downsides of mesh modelling —
> accurate geometry representation takes far more regions (volumetric geometry
> subdivisions). Each added region increases the number of boundaries that
> particles must cross. Since the simulation must enter single-scattering mode
> around region boundaries, the simulation time therefore increases.

### Does the energy deposited to air agree? Does the dose agree? Why or why not?

> The energies deposited in air agree within the statistical uncertainty (a
> $\large 1\sigma$ uncertainty). The dose does not. This is because we did not
> provide any region volumes, so the code automatically assigned every region in
> the medium a volume of 1 cm³ for the purposes of calculating the mass needed
> for conversion from energy to dose. Since the mesh geometry has many more
> regions and they are each assigned a volume of 1 cm³, the mass is vastly
> overestimated! Watch out for this easily overlooked mistake in your future
> simulations.
