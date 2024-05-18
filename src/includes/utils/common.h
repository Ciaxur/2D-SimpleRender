#pragma once

#include <glm/glm.hpp>
#include <vector>

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
double normalizeFloat(const double v, const double oldMin, const double oldMax, const double newMin, const double newMax);


/**
 * Finds the mindpoint of any given polygon shape, given an array of vertices.
 *
 * @param vertices Array of vertices.
*/
glm::vec2 findMidpoint(std::vector<glm::vec2>);
