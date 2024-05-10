#pragma once

/**
 * Normalizes a given value from one range to another range.
 *
 * @param v Value with old range.
 * @param oldMin Old minimum range.
 * @param oldMax Old maximum range.
 * @param newMin New minimum range.
 * @param newMax New maximum range.
 *
 * @returns Old value mapped to the new range.
*/
float normalizeFloat(const float v, const float oldMin, const float oldMax, const float newMin, const float newMax);