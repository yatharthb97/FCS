# Correlators

Libray that defines and implements three distinct autocorrelator modules:
1. Linear Correlator (implemnted)
2. Logarihmic Correlator (implementation pending)
3. Multi-Tau Correlator (implementation pending)


This library contains the following modules:

1. `lincorr_base.hpp` — Linear Correlator Base
2. `lincorr.hpp.hpp` — Linear Correlator Modules. Features include {normalisation, baseline subtraction}.
    * `LinCorr_RT` - Real-time LC with no baseline subtraction and no normalisation.
    * `LinCorr_RTBaseline` - Real-time LC with baseline subtraction but no normalisation.
    * `LinCorr_RTBaselineNorm` - Real time LC with baseline subtraction and also performs normalisation using calculated mean.
    * `LinCorr_RTNorm` - Real time LC with no baseline subtraction but performs normalisation using calculated mean.
3. 'logcorr.hpp' — Logarithmic Correlator
4. `multitau_corr_base.hpp` — Multi-tau correlators template base.
5. `multitau_corr.hpp` — Several Multi-tau correlator definations.

Auxillary files:

6. `types.hpp` — defines the type definations and macros used in the library.
7. `_indev.hpp` — rough code under development.
