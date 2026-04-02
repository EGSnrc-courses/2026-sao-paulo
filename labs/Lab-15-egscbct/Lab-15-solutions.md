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
<!-- # 13. -->
<!-- # 14. -->

# 15. Model CBCT scans: solutions <!-- omit in toc -->

- [15.1. CBCT scan using a monoenergetic photon beam](#151-cbct-scan-using-a-monoenergetic-photon-beam)
- [15.2. CBCT scan using a 120 kVp photon beam](#152-cbct-scan-using-a-120-kvp-photon-beam)

## 15.1. CBCT scan using a monoenergetic photon beam

#### How do you reproduce the image on the cover of this lab?

> Use `egs_view` to visualize the head phantom with a clipping plane with normal (1,0,0) at $\large d=0$ and compare it to the `imageJ` view of the scan.

#### How much more inefficient is calculation type `planar`? Why?

> About 2.9 times less efficient. A `ray-tracing` calculation only transports photons without interacting.

#### How much more inefficient is the scatter calculation?

> The scatter simulation is about 26 times less efficient!

#### Figure out how to produce additional projections (...)

> Change the $\large z$ rotation angle to produce 8 projections in 45 increments in the `setup` `input` block without changing the name of the scan file. Import the scan file into `imageJ` as a raw image and set the number of images to 8 in the dialog window. Play the sequence of projections by pressing on the play button on the lower left corner.

## 15.2. CBCT scan using a 120 kVp photon beam

#### Is there a difference in calculation time between monoenergetic and polyenergetic?

> Similar simulation times but increased variance decreases efficiency by 30 % when using the full spectrum!

#### Do you see a qualitative or quantitative difference in the scans?

> There is some loss of contrast, but in general the images are very similar

#### How much more inefficient is the scatter calculation?

> The scatter simulation is about 24 times less efficient!
