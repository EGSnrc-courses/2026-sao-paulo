# Lab 16 solutions

**16.2.1 What is the average tracklength dose to the scoring region? What is its uncertainty?**  
$1.85\times10^{-12}$ Gy/hist ($\pm~3.35\%$)  

**16.2.2 Compare the tracklength dose and its uncertainty to the average energy deposition dose and its uncertainty. What is the relative efficiency of the tracklength dose compared to the energy deposition dose?**  
The energy deposition dose is $1.84\times10^{-12}$ Gy/hist ($\pm~11\%$). Since the CPU times are identical (both tracklength and interaction doses are calculated in the same run), the relative efficiency boils down to the ratio of their squared uncertainties: $11^2 / 3.35^2 =  10.8$.
So tracklength scoring is much more efficient in this situation and yields a result in excellent agreement with the energy deposition dose.  

**16.2.3 How many particle steps were taken in the source geometry? In the phantom?**  
Approximately 52% of particle steps were taken in the source and 43% in the phantom. This result highlights that a significant fraction of the simulation time is spent transporting particles within the source geometry and motivates the use of particle recycling wherever applicable.  

**16.3.1 Compare the average dose and its uncertainty to the previous results. Which dose is smaller?**  
$1.75\times10^{-12}$ Gy/hist ($\pm~3.53\%$). This dose is smaller due in part to inter-seed attenuation.  

**16.3.2  How many particle steps were taken in the source geometry? How many were taken in the phantom? Compare to the previous simulation.**  
8% of particle steps were taken in the source and 84% in the phantom. The difference comes from particle recycling, which avoids unnecessarily repeating transport through source geometries.  

**16.3.3  What is the relative efficiency of this simulation compared to the previous one?**  
This time, we need to also take their respective CPU time into account. Use the CPU time reported on the line `Total cpu time for this run` in the output. On my system, this simulation is approximately 2.9 times more efficient than the previous one.  


