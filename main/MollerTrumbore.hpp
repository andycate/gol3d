#ifndef MOLLER_TRUMBORE_H
#define MOLLER_TRUMBORE_H

#include <glm/glm.hpp>

bool rayIntersectsTriangle(glm::vec3 ray_origin, glm::vec3 ray_vector, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3& out_intersection);

#endif
