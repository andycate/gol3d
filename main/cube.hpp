#ifndef CUBE_H
#define CUBE_H

#include "MollerTrumbore.hpp"

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <glm/glm.hpp>

#include <iostream>
#include <math.h>

class Cube {
    struct ColorMode {
        float c_pos, c_vert;
        ColorMode(float _c_pos, float _c_vert): c_pos(_c_pos), c_vert(_c_vert) {}
    };
    private:
        static const GLfloat g_vertex_buffer_data[108];
        static const ColorMode cmode;
        glm::vec3 pose;
        GLfloat vertices[108];
        GLfloat colors[108];
        int current_intersection;
        int current_highlight;
    public:
        static const glm::vec3 face_transforms[6];
        static const size_t vlen;
        static const size_t clen;

        Cube(glm::vec3 position);

        void append_vertices(GLfloat* varr, size_t pos);
        void append_colors(GLfloat* carr, size_t pos);
        std::pair<int, float> update_intersection(glm::vec3 origin, glm::vec3 ray);
        bool clear_highlight();
        void highlight_face(int face);
        glm::vec3 get_position();
};

#endif
