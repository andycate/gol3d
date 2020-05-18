#include "cube.hpp"

const GLfloat Cube::g_vertex_buffer_data[108] = {
            -0.5f,-0.5f,-0.5f, // triangle 1 : begin
            -0.5f,-0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f, // triangle 1 : end
            -0.5f,-0.5f,-0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f,-0.5f,
            0.5f, 0.5f,-0.5f, // triangle 2 : begin
            -0.5f,-0.5f,-0.5f,
            -0.5f, 0.5f,-0.5f, // triangle 2 : end
            0.5f, 0.5f,-0.5f,
            0.5f,-0.5f,-0.5f,
            -0.5f,-0.5f,-0.5f,
            0.5f,-0.5f, 0.5f,
            -0.5f,-0.5f,-0.5f,
            0.5f,-0.5f,-0.5f,
            0.5f,-0.5f, 0.5f,
            -0.5f,-0.5f, 0.5f,
            -0.5f,-0.5f,-0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f,-0.5f, 0.5f,
            0.5f,-0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f,-0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f,-0.5f,-0.5f,
            0.5f, 0.5f,-0.5f,
            0.5f,-0.5f,-0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f,-0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f,-0.5f,
            -0.5f, 0.5f,-0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f,-0.5f,
            -0.5f, 0.5f, 0.5f,
        };
const glm::vec3 Cube::face_transforms[6] = {
            glm::vec3(-1, 0, 0),
            glm::vec3(0, 0, -1),
            glm::vec3(0, -1, 0),
            glm::vec3(0, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
        };
const size_t Cube::vlen = 108;
const size_t Cube::clen = 108;
const Cube::ColorMode Cube::cmode(0.2, 0.075);

Cube::Cube(glm::vec3 position): current_intersection(-1), current_highlight(-1) {
    pose = position;
    for(int i=0;i<36;i++) {
        vertices[i*3+0] = Cube::g_vertex_buffer_data[i*3+0] + position.x;
        vertices[i*3+1] = Cube::g_vertex_buffer_data[i*3+1] + position.y;
        vertices[i*3+2] = Cube::g_vertex_buffer_data[i*3+2] + position.z;
        colors[i*3+0] = abs(pose.x) * Cube::cmode.c_pos + Cube::g_vertex_buffer_data[i*3+0] * Cube::cmode.c_vert;
        colors[i*3+1] = abs(pose.y) * Cube::cmode.c_pos + Cube::g_vertex_buffer_data[i*3+1] * Cube::cmode.c_vert;
        colors[i*3+2] = abs(pose.z) * Cube::cmode.c_pos + Cube::g_vertex_buffer_data[i*3+2] * Cube::cmode.c_vert;
    }
}

glm::vec3 Cube::get_position() {
    return pose;
}

void Cube::append_vertices(GLfloat* varr, size_t pos) {
    for(int i = 0; i < 36; i++) {
        varr[pos+i*3+0] = vertices[i*3+0];
        varr[pos+i*3+1] = vertices[i*3+1];
        varr[pos+i*3+2] = vertices[i*3+2];
    }
}

void Cube::append_colors(GLfloat* carr, size_t pos) {
    for(int i = 0; i < 36; i++) {
        carr[pos+i*3+0] = colors[i*3+0];
        carr[pos+i*3+1] = colors[i*3+1];
        carr[pos+i*3+2] = colors[i*3+2];
    }
}

bool Cube::clear_highlight() {
    if(current_highlight < 0) {
        return false;
    }
    colors[current_highlight*2*3*3+0] = abs(pose.x) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+0] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+1] = abs(pose.y) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+1] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+2] = abs(pose.z) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+2] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+3] = abs(pose.x) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+3] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+4] = abs(pose.y) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+4] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+5] = abs(pose.z) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+5] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+6] = abs(pose.x) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+6] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+7] = abs(pose.y) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+7] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+8] = abs(pose.z) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+8] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+0+9] = abs(pose.x) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+0+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+1+9] = abs(pose.y) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+1+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+2+9] = abs(pose.z) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+2+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+3+9] = abs(pose.x) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+3+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+4+9] = abs(pose.y) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+4+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+5+9] = abs(pose.z) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+5+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+6+9] = abs(pose.x) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+6+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+7+9] = abs(pose.y) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+7+9] * Cube::cmode.c_vert;
    colors[current_highlight*2*3*3+8+9] = abs(pose.z) * Cube::cmode.c_pos + 
            Cube::g_vertex_buffer_data[current_highlight*2*3*3+8+9] * Cube::cmode.c_vert;
    current_highlight = -1;
    return true;
}

void Cube::highlight_face(int face) {
    if(face == current_highlight) return;
    clear_highlight();
    colors[face*2*3*3+0] = 1;
    colors[face*2*3*3+1] = 1;
    colors[face*2*3*3+2] = 1;
    colors[face*2*3*3+3] = 1;
    colors[face*2*3*3+4] = 1;
    colors[face*2*3*3+5] = 1;
    colors[face*2*3*3+6] = 1;
    colors[face*2*3*3+7] = 1;
    colors[face*2*3*3+8] = 1;
    colors[face*2*3*3+0+9] = 1;
    colors[face*2*3*3+1+9] = 1;
    colors[face*2*3*3+2+9] = 1;
    colors[face*2*3*3+3+9] = 1;
    colors[face*2*3*3+4+9] = 1;
    colors[face*2*3*3+5+9] = 1;
    colors[face*2*3*3+6+9] = 1;
    colors[face*2*3*3+7+9] = 1;
    colors[face*2*3*3+8+9] = 1;
    current_highlight = face;
}

std::pair<int, float> Cube::update_intersection(glm::vec3 origin, glm::vec3 ray) {
    // if there is a current intersection, check that first
    //   if still intersecting, return true and exit
    //   else, clear current intersection and continue
    // for each triangle:
    //   if intersection
    //     if outward
    //       set intersection number to current
    //       return true
    // return false

    if(current_intersection > -1) {
        glm::vec3 v0(vertices[current_intersection*3*3+0],
                        vertices[current_intersection*3*3+1],
                        vertices[current_intersection*3*3+2]);
        glm::vec3 v1(vertices[current_intersection*3*3+3],
                        vertices[current_intersection*3*3+4],
                        vertices[current_intersection*3*3+5]);
        glm::vec3 v2(vertices[current_intersection*3*3+6],
                        vertices[current_intersection*3*3+7],
                        vertices[current_intersection*3*3+8]);
        float _dist;
        if(rayIntersectsTriangle(origin, ray, v0, v1, v2, _dist)) {
            return std::pair<int, float>(current_intersection/2, _dist);
        }
    }
    for(int i = 0; i < 12; i++) {
        glm::vec3 v0(vertices[i*3*3+0],
                        vertices[i*3*3+1],
                        vertices[i*3*3+2]);
        glm::vec3 v1(vertices[i*3*3+3],
                        vertices[i*3*3+4],
                        vertices[i*3*3+5]);
        glm::vec3 v2(vertices[i*3*3+6],
                        vertices[i*3*3+7],
                        vertices[i*3*3+8]);
        float dist;
        if(rayIntersectsTriangle(origin, ray, v0, v1, v2, dist)) {
            if(glm::dot(glm::cross(v1 - v0, v2 - v1), ray) < 0) {
                current_intersection = i;
                return std::pair<int, float>(i/2, dist);
            }
        }
    }
    return std::pair<int, float>(-1, 0.0);
}
