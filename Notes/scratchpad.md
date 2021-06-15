# FCS Simulation



# Lit Review

1. Paper 1 → [Fluorescence Correlation Spectroscopy Simulations of Photophysical Phenomena and Molecular Interactions: A Molecular Dynamics/Monte Carlo Approach](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3129548/) by James A. Dix, Erik F. Y. Hom, and A. S. Verkman*
2. 



# Introduction

## FCS Technique

The fluorescence signal received by the detector depends on multiple factors. The list is below:

1. $p$ → polarization of light
2. xxx

Fluorescence is sensitive to environmental conditions which affect the emission of photons by the fluorophores. Thus FCS is a sensitive technique that can be used to measure:

+ changes in nan environment
  + viscosity, pH, solvation
+ size and shape of molecules
+ molecular interactions

## FCS Setup

## FCS Schematics



















# Components

### Box

The simulation is done inside a class object called **Langevin Box**. The box contains the data fields and functions for the simulation.

→ The simulation uses Periodic Boundary Conditions (PBC) to mimic an infinite medium. Hence, the size of the box is only relevant when we calculate the number density of the particles. Because of the PBC, however, we also see the saturation of MSD value as the maximum displacement is restricted to the Edge of the cubic box. This saturation can be calculated  by:
$$
T_{saturation} = edge/(2 * dim * D)
$$
The particles are randomly initialized inside the boundaries of the box. The initial position is stored in order to calculate the displacements after every move, and ultimately the MSD value from those displacements.



### PSF

The PSF is described by the equation:
$$
\large
I = I_{•} e^{ \frac{-2x^{2}}{w_{x}^{2}}  \frac{-2y^{2}}{w_{y}^{2}}  \frac{-2z^{2}}{w_{z}^{2}} }
$$
 This equation describes a 3D gaussian which is symmetric along the x-y plane, but stretches along the z-axis, effectively forming an **ellipsoid**, which describes the Airy Disc if a plane is cut across the ellipsoid. The extension in *z* is described by a parameter called the **structure factor(sf)**. We say that:
$$
w_{z} = sf \times w_{x} = sf \times w_{y}
$$
Hence, the PSF can be describes by the above equation and two parameters: the radius and the structure factor(sf).

The contributions outside the Ellipsoid are small and can be ignored for optimization reasons. For a large set of particles, the optimization gives the following algorithm:

1. For each particle → detect if the particle is inside the ellipsoid. The following equation must be satisfied (Equation of ellipsoid)
   $$
   \large
   \frac{x^{2}}{w_{x}^2} + \frac{y^{2}}{w_{y}^2} + \frac{z^{2}}{w_{z}^2} \leq 1
   $$

2. For all particles, which are inside the Effective Volume → Calculate the probability of flashing.



### Laser

The Laser object is an object that describes a theoretical laser and a method Laser::prob() that gives the probabilistic contribution of the laser during photon emission. The laser object can operate in two modes:

1. Pulsing
2. Always-On

When the mode is set to "pulsing", the laser pulses with a set Period. The effect of the pulse, as there is a finite time of operation, depends on the Characteristic Decay Time of the laser which is set according to the need. The probability by the method  Laser::prob() is given by:
$$
p(laser; pulsing) = e^{-\frac{t_{now} - period}{char\_decay}}
$$


When the laser is set to "always-on":
$$
p(laser ; always-on) = 1
$$

### 







## Thermal Motion

The thermodynamic temperature is a measure of the average kinetic energy of the system. Thus the fluctuation is proportional to the temperature of the medium. Temperature is rather a quantity derived from the average fluctuation of the velocity of the particles. The temperature is assigned to the stationary value of this average.

The energy balance equation for the Langevin equation can be obtained by simply multiplying equation ($\ref{main_lang}$) by $v$ and applying the ensemble average operator:
$$
\frac{m}{2} \frac{d\langle v^{2} \rangle}{dt} + \gamma \langle v^{2} \rangle = \langle v F_\xi \rangle
$$
The first term is the average kinetic energy $K$, the evolution of this equation in time is given by: 
$$
\frac{dK}{dt} = \langle v F \rangle - 2 \gamma K \label{dKdt}
$$
The terms :

+ $\langle v F_\xi \rangle$ = average rate of work done by the random forces $F_\xi$.
+ $2 \gamma K$ = average rate of energy dissipation

Equation ($\ref{dKdt}$) can be further solved to obtain:
$$
K(t) = \frac{c_0}{4\gamma}(1 - e^{-2 \gamma t}) \label{K(t)}
$$
$c_0$ comes from equation ($\ref{gaussn_corr}$). The diagram below describes the steady state conditions. The work done by the random noise keeps the system moving, while the dissipative contribution tries to slow down the system and bring it to halt. Equilibrium refers to the condition when these two balance each other.

| !["Equilibrium Schematic (Source: Kerson Huang)"](C:\Users\Yatharth_Nitro\AppData\Roaming\Typora\typora-user-images\image-20210609171551619.png) |
| :----------------------------------------------------------: |
|         Equilibrium Schematic (Source: Kerson Huang)         |

For $t → \infin$, equation ($\ref{K(t)}$) becomes :
$$
K(t → \infin) = \frac{c_0}{4m \gamma} \label{kinf1}
$$
Over large times, the equipartition of energy also holds, hence:
$$
K(t → \infin) = \frac{1}{2} k_B T \label{kinf2}
$$
Equating these two equations, we get:
$$
c_0 = 2m \gamma k_{B} T \label{c_0T}
$$


Using equation ($\ref{c_0T}$) and $v = dx/dt$ in equation ($\ref{main_lang}$), we get:
$$
m \frac{d^2x}{dt^2} + m \gamma \frac{dx}{dt} = F_\xi
$$
We multiply both sides by $x$ and using the ensemble average operator:
$$
\biggl<{x\frac{d^{2}x}{dt^2}} \biggr> + \gamma \cdot\biggl<{x\frac{dx}{dt}} \biggr> = 0
$$
$\langle xF_\xi \rangle = 0$, solving further:
$$
\langle x^{2} \rangle = \frac{c_{0}t}{m^{2} \gamma^{2}}
$$
