#include "gol.hpp"

glm::vec3 GOL::transforms[26] = {glm::vec3(-1, -1, -1),
                    glm::vec3(-1, -1, 0),
                    glm::vec3(-1, -1, 1),
                    glm::vec3(-1, 0, -1),
                    glm::vec3(-1, 0, 0),
                    glm::vec3(-1, 0, 1),
                    glm::vec3(-1, 1, -1),
                    glm::vec3(-1, 1, 0),
                    glm::vec3(-1, 1, 1),
                    glm::vec3(0, -1, -1),
                    glm::vec3(0, -1, 0),
                    glm::vec3(0, -1, 1),
                    glm::vec3(0, 0, -1),
                    glm::vec3(0, 0, 1),
                    glm::vec3(0, 1, -1),
                    glm::vec3(0, 1, 0),
                    glm::vec3(0, 1, 1),
                    glm::vec3(1, -1, -1),
                    glm::vec3(1, -1, 0),
                    glm::vec3(1, -1, 1),
                    glm::vec3(1, 0, -1),
                    glm::vec3(1, 0, 0),
                    glm::vec3(1, 0, 1),
                    glm::vec3(1, 1, -1),
                    glm::vec3(1, 1, 0),
                    glm::vec3(1, 1, 1)
                };

GOL::GOL(int e_l, int e_u, int f_l, int f_u) : cubes(), r(Rule(e_l, e_u, f_l, f_u)) {}
GOL::GOL(std::vector<glm::vec3> _cubes, int e_l, int e_u, int f_l, int f_u) : cubes(), r(Rule(e_l, e_u, f_l, f_u)) {
    for(glm::vec3 v : _cubes) {
        cubes.insert({v, v});
    }
}

bool GOL::add_cube(glm::vec3 cube) {
    return cubes.insert({cube, cube}).second;
}

bool GOL::remove_cube(glm::vec3 cube) {
    return cubes.erase(cube) > 0;
}

void GOL::iterate() {
    Cubes cubes_copy(cubes);
    Cubes cubes_new;
    std::unordered_map<glm::vec3, int, Vec3Key, Vec3Key> counts;

    for(std::pair<glm::vec3, glm::vec3> c : cubes_copy) {
        for(glm::vec3 t : GOL::transforms) {
            glm::vec3 cell = c.second + t;
            if(counts.find(cell) != counts.end()) {
                int count = counts[cell] + 1;
                if(count >= r.f_l && count <= r.f_u) {
                    cubes_new[cell] = cell;
                } else if(count > r.f_u) {
                    cubes_new.erase(cell);
                }
                counts[cell] = count;
            } else {
                counts[cell] = 1;
            }
        }
    }
    for(std::pair<glm::vec3, glm::vec3> c : cubes_copy) {
        glm::vec3 cell = c.second;
        if(counts.find(cell) != counts.end()) {
            if(counts[cell] > r.e_u || counts[cell] < r.e_l) {
                cubes_new.erase(cell);
            }
        }
    }
    cubes = cubes_new;
}

std::vector<glm::vec3> GOL::get_cubes() {
    std::vector<glm::vec3> result;
    for(std::pair<glm::vec3, glm::vec3> c : cubes) {
        result.push_back(c.second);
    }
    return result;
}
