# Langevin Equation

The Langevin equation simulates Brownian motion of particles. The equation can be solved to simulate non-equilibrium systems. 
$$
\begin{align}
m \frac{dv}{dt} &= - m \lambda v + F_\eta(t)\\
m \frac{dv}{dt} &= - \gamma v + F_\eta(t) \label{main_lang}
\end{align}
$$

The mass $m$ is assumed to be absorbed by the constants $\lambda$ to become $\gamma$ → $m\lambda = \gamma$. Please note that $\gamma$ is not the friction coefficient in conventional sense. It has an absorbed mass factor.


1. $F_\xi(t)$ is a random force, which is a result of random impacts of implicit fluid particles much smaller than the Brownian particles the equation explicitly describes. In our simulation, we assume that the fluid is implicit.
2. The term $- \gamma v$ is the **Frictional Force (Drag)** that the fluid exerts on the particles. 

The two forces represent the two aspects of interaction of the Brownian and the fluid particles. The first describes **fluctuation** and the other **dissipation**. The two forces and this equation is a fundamental description of the atomicity (granularity) of matter. The granularity of matter results in both — the fluctuation of thermodynamic quantities and friction. <u>The two forces are thus not independent and are related by a **Fluctuation-Dissipation** law specific to that system.</u>

The force $F_\xi(t)$ can be either completely indeterministic in situations where no other external force is present. In that case, the noise profile is described by a Gaussian White Noise Profile → $\xi(t)$. Which implies that the force at time $t$ is completely uncorrelated with the force at anytime $t^{'}$ (except $t = t^{'}$). The noise is described by the equations: 
$$
\begin{align}
F(t) &= \xi(t)\\ 
\langle \xi(t) \rangle &= 0 \label{gaussn_mean}\\
\langle \xi(t_1)\xi(t_2) \rangle &= c_0\delta(t_1 - t_2) \label{gaussn_corr}
\end{align}
$$
The constant $c_0$ can be deduced by the energy balance equations related to the Langevin equations. If a steady component is present, which is a result of external forces, the random force takes the form:
$$
F_\xi = F_0 + \xi(t) \label{force}
$$
In steady state conditions, the average velocity reaches the terminal velocity $v_0$ and the $dv/dt = 0$ condition is satisfied. Substituting these in equation ($\ref{main_lang}$), we get:
$$
v_0 = \frac{F_0}{\gamma} \label{v_0}
$$
Note that the $\xi(t)$ in equation ($\ref{force}$) is zero using the relation ($\ref{gaussn_mean}$). Mobility($\eta$) is defined as:
$$
\eta = \frac{1}{\gamma} \label{mobility}
$$
For a system with no external forces, and hence $F_0 = 0$, we can conclude using equation ($\ref{v_0}$) that the terminal velocity is zero. This means that the particles have no preferred direction of movement. If a particle is initialized at the origin, the probability of the particle to be nearest to any side of a cubical box of appropriate edge after some time $t$ is equal.

## Einstein's Fluctuation-Dissipation theorem

We state without proof (will be included later):
$$
D = \frac{k_{B}T}{\gamma}, \label{diff}
$$
this is the **Einstein's Fluctuation-Dissipation theorem**, which is the suitable F-D law for our system. Using equation ($\ref{mobility}$), in terms of mobility, we get:
$$
D = k_{B}T \eta
$$



## Update Scheme



The solution of equation ($\ref{}$) yield the update scheme for our simulation:
$$
x(t + \Delta t) = x(t) - {\frac{1}{\gamma}\cdot \frac{dU}{dx}\cdot \Delta t} + \xi(t) \label{update_sch_1}
$$
The first term on the RHS is the contribution from an external force and is deterministic. This force, if non-zero, gives the particles a directional drift. The equation ($\ref{gaussn_corr}$) can be expanded and the value $c_0$ is calculated from the energy balance equations(will be included later). The second term $\xi(t)$ is the random force term, which is characterized by the relation:
$$
<\xi(t)\cdot \xi(t^`)> = \frac{2k_BT}{\gamma} \cdot \Delta t \cdot \delta_{tt^`}
$$
This equation clearly defines the dependence of the gaussian noise on the temperature of the system, hence, this noise is the **Thermal Noise** present in the system. 

 If we assume that there are no external forces, equation ($\ref{update_sch_1}$) reduces to:
$$
\begin{align}
x(t + \Delta t) 
&= x(t) + \xi(t)\\
&= x(t) + \sqrt{{2k_BT/\gamma}\cdot \Delta t}\cdot g(t)
\end{align}
$$
$g(t)$ is the pure Gaussian white noise which has a mean of zero and a variance of unity. By using the Einstein's Fluctuation - Dissipation theorem described in equation ($\ref{diff}$), we get:
$$
\begin{align}
\xi(t)
&= \sqrt{{2k_BT/\gamma}\cdot \Delta t}\cdot g(t)\\
&= \sqrt{2 \cdot D \cdot \Delta t} \cdot g(t) 
\end{align}
$$

This equation describes the movement of particles in terms of its **Diffusivity**. And based on equation ($\ref{diff}$), it is coupled to $\gamma$ , which is coupled to the mass $m$ and the drag coefficient of the fluid — $\lambda$. 

## Stokes Law

We first assume that our particle is spherical. We also assume that the system is in "low Reynolds Number regime" and other assumptions relevant for the *Stokes Model* to be applicable.  The Stokes law states that:
$$
F_{d} = 6 \pi \eta R v \label{stokes_law}
$$
Comparing it with the friction term: 
$$
\gamma \cdot v = 6 \pi \eta R \cdot v
$$
Hence,
$$
\gamma = 6 \pi \eta R \quad (spheres) \label{gamma_sph}
$$


For particles that are not spheres, the $\gamma$ value can be defined as:
$$
\gamma = \eta \space w
$$
$w$ represents the size of the particle. (citation required)



For aspherical particles, equation ($\ref{gamma_sph}$) becomes:
$$
\gamma = 6 \pi \eta R_{H}
$$
$R_{H}$ is called the **Stokes-Einstein radius** or the **Hydrodynamic radius** of the aspherical particle. This parameter not only describes friction in terms of the size, but also takes into account the solvent effects. Hence, $R_{H}$ may depend on ionic configuration, hydrogen bonding, etc.

# Units

The units are set and controlled via a ```class Units```.

## Need for Simulation Units

The parameters in the relations described above are real values ($\R$), and on a computer, these are represented as floating point numbers. A *normalized* floating point number can be represented as:
$$
Float \space value = (-1)^S + M \times 2^{E}
$$
$S$ is a 1-bit sign value. $E$ is the exponent and $M$ stands for mantissa which takes a value from 0.100... to 9.999.. in **normalized form (1 significant digit ahead of the decimal point)**. The value $E$ puts strict bounds on largest(or smallest) values that can be represented for a fixed size floating  point number.

​	```sizeof(float) = sizeof(1 bit) * (S+E+M)```

But if a value is given to the float, which is larger or smaller than the set normalized bounds, the float takes a *denormalized* form (e.g. 0.0000000...1). The operation of a normalized float with a denormalized float can lead to undefined behavior. In scientific computing, these cases can be a common occurrence when many complex arithmetic operations are performed(especially subtractive cancellation). It is thus advisable to work with units that are neither too small nor too large.

We define a scale called Simulation Units, where:
$$
\begin{align}
\sigma &→ 1.0 \space (length)\\
m &→ 1.0 \space (mass)\\
\epsilon &→ 1.0 \space (thermal \ energy)
\end{align}
$$

## Setting Scale

The Temperature ($T$), Viscosity ($\eta$), and Sigma ($\sigma$ → length scale) values are set. Sigma is the length scale of the simulation and is usually set equal to the diameter of the particle $\sigma = 2R$. 
$$
\begin{align}
T &= constant → \epsilon = k_BT = constant\\
\eta &= constant\\
\sigma &= constant
\end{align}
$$


Then we use $\sigma = 2R$, i.e. we set the length scale equal to the diameter of our particle:
$$
F_d = 3\pi \nu \sigma v = \gamma v \label{stokes}
$$
We say, $3\pi \eta \sigma = \gamma$  → which is the inverse of mobility. And $1/ \gamma = \Gamma$ → which will then represent the mobility. Equation ($\ref{stokes_law}$) then becomes:
$$
F_d = \frac{v}{\gamma}
$$

By using dimensional analysis,
$$
\begin{align}
\gamma \quad [=]  \quad \eta\sigma \quad &[=] \quad [M][T]^{-1}\\
k_{B}T \quad [=] \quad [E] \quad &[=] \quad [M][L]^2[T]^{-2}\\
\sigma \quad &[=] \quad [L]\\
\end{align}
$$
All other units can be expressed using the units described above. We now try to form the basis for some other basic units using dimensional analysis to do basic unit conversions,
$$
\begin{align}
[T] &= [\Gamma] \space [K_{B}T]^{-1} \space [L]^{2}\\
[M] &= [\Gamma]^{2} \space [K_{B}T]^{-1} \space [L]^{2}\\
[F] &= [K_{B}T] \space [L]^{-1}\\
\end{align}
$$


The above dimensional equations yield:
$$
\begin{align}
1.0 \space t_{sim} &= \frac{\gamma \sigma^{2}}{k_{B}T} \space t_{SI} \quad (time)\\
1.0 \space m_{sim} &= \frac{\gamma^{2}\sigma^{2}}{k_{B}T} \space m_{SI} \quad (mass) \label{mass_conv}\\ 
1.0 \space F_{sim} &= \frac{k_{B}T}{\sigma} \space F_{SI} \quad (force)
\end{align}
$$
We introduce a representation:

> $\eta$   →  real units (SI)
>
> $\eta^*$  → simulation units (dimensionless)



Using equation ($\ref{stokes}$) we calculate the conversion for *Dynamic Viscosity* ($\eta$),
$$
\begin{align}
\eta &= \frac{\gamma}{3\pi \sigma}\\
\eta &=  \Big(\frac{\gamma}{3\pi \sigma}\Big)\space \eta^*\\
\end{align}
$$
Also note that, 
$$
\begin{align}
\eta^* &= \frac{\eta}{\Big(\frac{\gamma}{3\pi \sigma}\Big)} = 1\\
[\eta^*] &= [1] \quad (dimensionless)
\end{align}
$$
<u>Note: All the simulation units are dimensionless.</u>

Using equation ($\ref{diff}$) and ($\ref{gamma_sph}$) ,
$$
D = \Big(\frac{k_{B}T}{\gamma} \Big) \space D^*
$$
Using equation ($\ref{}$) we get the conversion for the diffusion timescale, 
$$
\tau = \Big(\frac{\sigma^{2}}{D}\Big) \space \tau^* = \Big(\frac{\gamma \sigma^{2}}{k_{B}T}\Big) \space \tau^*
$$


Calculation for density of the particle: 
$$
V = \frac{4}{3}\pi (\sigma/2)^{3} = \frac{\pi \sigma^{3}}{6}
$$


$$
\begin{align}
\rho_{m}
&= m/V\\
&= \frac{6m}{\pi \sigma^{3}}\\
&= \frac{6(M_{r} \space m^{*})}{\pi \sigma^{3}}\\
\end{align}
$$
$M_{r}$ is the multiplicative constant on the RHS of equation ($\ref{mass_conv}$) which converts the simulation unit mass to real unit mass (SI).
$$
\begin{align}
\rho_m &= \Big(\frac{M_r}{\sigma^3}\Big) \cdot \Big(\frac{6m^*}{\pi} \Big) \cdot \rho^*\\
\rho_m^* &= \rho_m \Big(\frac{M_r}{\sigma^3}\Big)^{-1} \label{mden_conv}\\
\rho_m^* &= \Big(6m^*/\pi \Big)
\end{align}
$$
We arrive at a fixed mass density for monomer in simulation units which is proportional to the reduced mass $m^*$, but the conversion is different. For a particle of mass $m^*$, the mass density of the monomer (not the solvent), is $\rho_m^* = \Big(6m^*/\pi \Big) $ → which is fixed and the conversion to real mass units is done via equation ($\ref{mden_conv}$).



Based on this, the value of $D^*$ and $m^*$ can be set appropriately. If we assume that the mass density of the monomer, $\rho_m$ is approximately same as the fluid, then the mass $m^*$ is also set base on equation (52).



