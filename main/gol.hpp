#ifndef GOL_H
#define GOL_H

#include <glm/glm.hpp>
#include <boost/functional/hash.hpp>

#include <vector>
#include <iostream>
#include <unordered_map>

class GOL {
    struct Rule {
        int e_l, e_u, f_l, f_u;
        Rule() { e_l = 0; e_u = 0; f_l = 0; f_u = 0; }
        Rule(int _e_l, int _e_u, int _f_l, int _f_u): e_l(_e_l), e_u(_e_u), f_l(_f_l), f_u(_f_u) {}
    };
    struct Vec3Key {
        size_t operator()(const glm::vec3& k)const {
            size_t seed = 0;
            boost::hash_combine(seed, k.x);
            boost::hash_combine(seed, k.y);
            boost::hash_combine(seed, k.z);
            return seed;
        }
        bool operator()(const glm::vec3& a, const glm::vec3& b)const {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }
    };
    typedef std::unordered_map<glm::vec3,glm::vec3,Vec3Key,Vec3Key> Cubes;
    private:
        static const glm::vec3 transforms[26];
        Cubes cubes;
        Rule r;
    public:
        GOL(int e_l, int e_u, int f_l, int f_u);
        GOL(std::vector<glm::vec3> _cubes, int e_l, int e_u, int f_l, int f_u);

        void iterate();
        bool add_cube(glm::vec3 cube);
        bool remove_cube(glm::vec3 cube);
        std::vector<glm::vec3> get_cubes();
};

#endif
