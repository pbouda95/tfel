set(IEEE754_ROUNDING_MODES )
if(NOT TFEL_BROKEN_LIBMATH)
  list(APPEND IEEE754_ROUNDING_MODES "UpWard")
  list(APPEND IEEE754_ROUNDING_MODES "DownWard")
  list(APPEND IEEE754_ROUNDING_MODES "ToNearest")
  list(APPEND IEEE754_ROUNDING_MODES "TowardZero")
  list(APPEND IEEE754_ROUNDING_MODES "Random")
else(NOT TFEL_BROKEN_LIBMATH)
  list(APPEND IEEE754_ROUNDING_MODES "ToNearest")
endif(NOT TFEL_BROKEN_LIBMATH)
