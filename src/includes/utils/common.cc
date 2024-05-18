#include "common.h"

double normalizeFloat(const double v, const double oldMin, const double oldMax, const double newMin, const double newMax) {
  // If both minimum ranges are the same.
  // if ((oldMin - newMin) == 0) return oldMin;

  /*
                      (value−oldMin)×(newMax−newMin)
    normalizedValue =  ------------------------------  + newMin
                                oldMax−oldMin
  */
  double oldRange = oldMax - oldMin;
  double newRange = newMax - newMin;
  return (((v - oldMin) * newRange) / oldRange) + newMin;
}

glm::vec2 findMidpoint(std::vector<glm::vec2> vertices) {
  double sumX = 0.0;
  double sumY = 0.0;
  size_t numVertices = vertices.size();

  // Calculate the sum of x and y coordinates
  for (const glm::vec2 &vertex: vertices) {
    sumX += vertex.x;
    sumY += vertex.y;
  }

  // Calculate the centroid
  double centroidX = sumX / numVertices;
  double centroidY = sumY / numVertices;

  return glm::vec2(centroidX, centroidY);
}