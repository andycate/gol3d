#include "MollerTrumbore.hpp"

bool rayIntersectsTriangle(glm::vec3 ray_origin, glm::vec3 ray_vector, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float& out_dist) {
    const float EPSILON = 0.0000001;
    glm::vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = v1 - v0;
    edge2 = v2 - v0;
    h = glm::cross(ray_vector, edge2);
    a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = ray_origin - v0;
    u = f * glm::dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = glm::cross(s, edge1);
    v = f * glm::dot(ray_vector, q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * glm::dot(edge2, q);
    if (t > EPSILON) // ray intersection
    {
        out_dist = t;
        // out_intersection = ray_origin + ray_vector * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}
