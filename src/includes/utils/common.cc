#include "common.h"

float normalizeFloat(const float v, const float oldMin, const float oldMax, const float newMin, const float newMax) {
  // If both minimum ranges are the same.
  // if ((oldMin - newMin) == 0) return oldMin;

  /*
                      (value−oldMin)×(newMax−newMin)
    normalizedValue =  ------------------------------  + newMin
                                oldMax−oldMin
  */
  float oldRange = oldMax - oldMin;
  float newRange = newMax - newMin;
  return (((v - oldMin) * newRange) / oldRange) + newMin;
}