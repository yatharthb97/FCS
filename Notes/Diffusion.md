# Diffusion

As diffusion does not require an external energy source for the transport of particles, it is also called **passive transport**.

The particles diffuse from an region of low concentration to a region of high concentration. This can be interpreted by the fundamental relation:
$$
\langle x^{2} \rangle = 2nDt
$$
Since $\langle x \rangle = 0$, the variance $\sigma^{2} = 2nDt$. The variance of the random stochastic variable $x$ grows linearly with time $t$.

The distinct low and high concentration regions represent the concentration gradient in the diffusive environment. This **diffusion gradient** helps set up the flows required for the diffusion or the passive transport.

The flow is established when the system is ergodic. Since the update scheme for our particles is unbiased, and no other artifacts are present in the system — after a sufficiently long time interval $t$, the probability for the particle to occupy any sub-volume $\delta V$ inside the total volume $V$ is equal. 

The number  of microstates that can be defined for each particles is thus:
$$
\Omega = \frac{V}{\delta V}
$$
We now picture a small volume $\Delta V$, which is placed at the center. After a sufficiently long time $t$, we say that **Dynamic Equilibrium** is established. 



1. TK → Include Density fluctuation equations.

2. Diffusion is an emergent property.

3. Diffusion depends on molecular mass and size in gases.

4. Diffusion depends on Temperature.

5. Typical diffusivity for gases $10^{-5} Pa \cdot s$.

6. Typical diffusivity for liquids - $10^{-5} - 10^{-12} \space Pa \cdot s$.

7. Mass transfer is proportional to diffusivity, concentration gradient, and inverse of the distance. 

8. We don't need to worry about the Navier-Stokes Equations as ... TK

9. Rate of molecular diffusion can be expressed in terms of molar flux. The molar flux is defined as the moles of a substance transferring through area in unit time.
   $$
   Molar flux = \frac{Moles}{(Area)(time)}
   $$
   The area is measured normal to the direction of the diffusion.

   For mixtures, two types of fluxes are used to describe motion of each component, N-type and J-type.

   ​	a.	The N-type flux is relative to a fixed location in space.

   ​	b. The J-type flux is relative to the average molar velocity of all the components.

   ![image-20210612045432394](C:\Users\Yatharth_Nitro\AppData\Roaming\Typora\typora-user-images\image-20210612045432394.png)

10. Fick's First law of Molecular Diffusion: Molar flux is directly proportional to the concentration gradient. Molecular diffusion occurs from a place of high concentration to the place of low concentration.

    For a binary mixture with A and B. Diffusion of A **in** B in the z direction:
    $$
    J_{A} = -D_{AB} \frac{\partial C_{A}}{\partial z}
    $$
    Negative sign represents the fact that the process occurs in a direction of decreasing concentration.

11. Diffusion coefficient is the ratio of molar flux to its concentration gradient. It is the measure of the diffusive mobility of a component.

12. The dimensions of diffusion coefficients: $[L]^{2}[T]^{-1}$.

The Stokes-Einstein model assumes the liquid to be of much smaller size than our solute — $R_{solvent} << R_{solute}$.  In this case, the solvent can be approximated as a continuum like entity and the Stoke's law is said to be valid. 

This model also requires that the solution is dilute and the Viscosity is not temperature dependent in the working range. Since, higher concentrations result in crowding and increase in the Viscosity of the solvent. 

Usually, we assume the mass densities of the solute and the solvent to be equivalent. Hence, mass can be used as a proxy for the size of the particle and vice versa. We can then use the Stokes-Einstein relation for solutes which have a molar mass much larger than that of the solvent.



For cases when the above assumptions don't hold, when the solvent molecules are larger, the **Wilke-Chang Equation**.
$$
D = 5.878 \cdot 10^{-17} \frac{\phi M_{solv}T}{\mu_{sol}V_{A_{b}}^{0.6}}
$$
 $V_{A_{b}}$ is the molar volume at the boiling point for the solute. $\phi$ is the association constant.





## KMT

The "Kinetic Molecular Theory" assumes the particles to be always in motion. The average speed of the movement defines the kinetic energy of the system. When other constrains are introduced, distinctions between different phases in the system can be made.

The KMT results in a fundamental relation between the average speed of the particles in the system and its Temperature, called the equipartition theorem.
$$
K_{i} = \frac{1}{2} k_{B} T
$$


Note: In biological systems, if the substance to be transported, does not have an adequate diffusivity, a special systems called **facilitated diffusion** is observed. Some common examples include the transport of micronutrients via the transport proteins and  transport of $O_{2}$ via Hemoglobin.







