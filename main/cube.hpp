#ifndef CUBE_H
#define CUBE_H

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <glm/glm.hpp>

class Cube {
    private:
        static const GLfloat g_vertex_buffer_data[108];
        GLfloat vertices[108];
    public:
        Cube(glm::vec3 position);
};

#endif
