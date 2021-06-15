![image-20210612133242892](C:\Users\Yatharth_Nitro\AppData\Roaming\Typora\typora-user-images\image-20210612133242892.png)



How does a probability distribution  starts with $\delta(v - v_0)$ at $t = t_0$ and ends as a gaussian $\rho(V, t → \infin | v_{0})$  ?



We know that :
$$
\rho_{eq}^{(v)} = \Big(\frac{m}{2 \pi k_{B}T} \Big)^{1/2} \cdot e^{\frac{-mv^{2}}{2k_{B}T}}
$$

$$
v^{2}(t) = v^{2}_{0}(t) 

+ \frac{2v_{0}}{m} \int_{0}^{t}dt \cdot \overline{\eta^{'}(t)} 

+ \frac{1}{m^{2}} \int_{0}^{t} dt_{1} \int_{0}^{t}dt_{2} \space \overline{\eta(t_{1}) \eta({t_{2}})} \label{v_long}
$$
By using equations () and () for the gaussian noise, the term $\frac{2v_{0}}{m} \int_{0}^{t}dt \cdot \overline{\eta^{'}(t)} = 0$.

We recap the expression of the *delta-correlated white noise*, which has a flat power spectrum.
$$
\langle \eta(t_{1}) \eta({t_{2}})\rangle = \Gamma \delta(t_{1}-t_{2})
$$


 ![image-20210612142000001](C:\Users\Yatharth_Nitro\AppData\Roaming\Typora\typora-user-images\image-20210612142000001.png)

In the $[t_{1} , t_{2}]$ space, the integral (3rd term of RHS in equation($\ref{v_long}$)) is only non-zero for $t_{1} = t_{2}$ because of the delta function. Hence, we can replace $t_{2}$ with $t_{1}$. Then, $\langle \eta^{2} (t_{1}) \rangle = 1$ — completely correlated with itself.
$$
\begin{align}
v^{2}(t) &= v^{2}_{0}(t)  + \frac{\Gamma}{m^{2}} \int_{0}^{t} dt_{1}  \label{} \\
v^{2}(t) &= v_{0}^{2} + \frac{\Gamma \space t}{m^{2}} 
\end{align}
$$
Now,
$$
\begin{align}
\langle v^{2} \rangle &= \frac{k_{B}T}{m} + 
\frac{\Gamma t}{m^{2}}
\end{align}
$$
This system, absorbs energy from the fluid solvent molecules and gains energy. This system cannot exist.



We revisit our assumptions:

1. We say there is no such thing as a perfectly delta-correlated noise. There is some finite correlation time.
   $$
   \overline{\eta(t_1)\eta(t_2)} = K e^{-|t_1 - t_2|/\tau }
   $$
   When we solve the integral ($\ref{v_long}$) with this correction, the velocity still increases with time. Hence, our problem is not solved.

2. There is a part of the random force $F_{\eta}$, which depends on the velocity itself. The force is stronger on the side of the direction of movement.
   $$
   \begin{align}
   m\dot{v} &= F_{systematic} + F_{random}\\
   m\dot{v} &= -m\lambda v(t) + \eta(t)\\
   m\dot{v} &= -\gamma v(t) + \eta(t)\\
   \end{align}
   $$



This is the simplest model for a frictional force. We assume that the Viscous drag (the specific frictional force in our system) is related to the constant $\gamma$ in our equation above.

The solution is now:
$$
v(t) = v_{0} e^{-\gamma t} + \frac{1}{m} \int_{0}^{t} dt^{'} e^{-\gamma (t-t^{'})} \space \eta(t^{'})
$$
Now,
$$
\overline{v(t)} = v_{0} e^{-\gamma t}
$$
The initial condition vanishes exponentially with time.
$$
\overline{v^{2}(t)} = v_{0}^{2} 
e^{-\gamma t}
+ \frac{1}{m^2} \int_{0}^{t} dt_1 \int_{0}^{t} dt_2 \overline{\eta(t_1)\eta(t_2)} \cdot
e^{-\gamma (t-t_1) -\gamma (t-t_1)}
$$



$$
\overline{\eta(t_1)\eta(t_2)} = \langle \eta(t_1)\eta(t_2) \rangle 
= \Gamma \delta(t_1 -  t_2)
$$
But why ?? TK 

Then, 
$$
\begin{align}
\overline{v^{2}(t)} &= v_{0}^{2} 
e^{-2\gamma t}
+ \frac{\Gamma}{m^2} \int_{0}^{t} dt_1 
\cdot
e^{-2\gamma (t-t_1)}\\


&= v_{0}^{2} e^{-2\gamma t}
+ \frac{\Gamma}{2\gamma m^2}
e^{-2\gamma t}(e^{2\gamma t} - 1)\\

&= v_{0}^{2} e^{-2\gamma t}
+ \frac{\Gamma}{2\gamma m^2}
(1 - e^{2\gamma t})\\


&= \Big( v_{0}^{2} - \frac{\Gamma}{2\gamma m^2} \Big) 
e^{-2\gamma t}
+ 
\frac{\Gamma}{2\gamma m^2}
\end{align}
$$
For $t → \infin$, $\overline{v^{2}} → \langle v^2 \rangle_{eq}$, then $\Gamma/2m^2 \gamma = \langle v^2 \rangle_{eq}$.
$$
\begin{align}
\frac{\Gamma}{2m^2 \gamma} &= \frac{k_{B}T}{m}\\

\Gamma &= 2m \gamma k_{B} T \label{fluct_relaxation}\\
\end{align}
$$
Equation ($\ref{fluct_relaxation}$) is known as the **fluctuation-dissipation theorem**.

<u>OR</u>
$$
\langle v^2(t) \rangle = \int_{-\infin}^{\infin} \rho_{eq}(v_{0}) \space
\overline{v^{2}(t)} \space 
dv_{0}
$$


