#pragma once

#include <cstdint>


typedef uint_fast64_t counter_t; //! Data type received from the pulse counter. It is the fundamental type used for representing series data.

typedef uint_fast16_t index_t; //! It is used as the array indices and thus determine the maximum size of the Channel_array and the Series_array.

#define LC_DSep ','

#define LC_SAFETY_CHECKS 1