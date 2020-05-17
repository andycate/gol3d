#include "LoadShader.hpp"
#include "gol.hpp"

#define GLFW_INCLUDE_GLCOREARB
// #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <boost/functional/hash.hpp>

#include <ctime>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <math.h>

/* structures */
struct Point {
    float x, y;
    Point() { x = 0.0; y = 0.0; }

    Point(float _x, float _y): x(_x), y(_y) {}
};

struct View {
    float fov, vert, horiz, transx, transy, zoom;
    View() { fov = 45.0; vert = 0.0; horiz = 0.0; transx = 0.0; transy = 0.0; zoom = 5.0; }
};


/* global variables */
int width = 800;
int height = 600;
Point* lastMousePt = new Point;
Point* delta = new Point;
int drag = 0;
View* view = new View;
GLuint vertexbuffer;
GLuint colorbuffer;
glm::mat4 Projection = glm::perspective(glm::radians(view->fov), (float) width / (float)height, 0.1f, 100.0f);

GLfloat* g_compiled_vertex_data = new GLfloat[0];
GLfloat* g_compiled_color_data = new GLfloat[0];
int compiled_length = 0;
int compiled_vertices = 0;
GOL game(std::vector<glm::vec3>({/* glm::vec3(0, 0, 0), */
                                    // glm::vec3(1, 0, 0), 
                                    // glm::vec3(0, 1, 0), 
                                    // glm::vec3(0, 0, 1),
                                    // glm::vec3(-1, 0, 0), 
                                    // glm::vec3(0, -1, 0), 
                                    // glm::vec3(0, 0, -1),
                                    // glm::vec3(1, 1, 0), 
                                    // glm::vec3(0, 1, 1), 
                                    // glm::vec3(1, 0, 1),
                                    // glm::vec3(-1, 1, 0), 
                                    // glm::vec3(0, -1, 1), 
                                    // glm::vec3(-1, 0, 1),
                                    // glm::vec3(1, -1, 0), 
                                    // glm::vec3(0, 1, -1), 
                                    // glm::vec3(1, 0, -1),
                                    // glm::vec3(-1, -1, 0), 
                                    // glm::vec3(0, -1, -1), 
                                    // glm::vec3(-1, 0, -1),
                                    // glm::vec3(1, 1, 1), 
                                    // glm::vec3(1, -1, 1), 
                                    // glm::vec3(1, 1, -1),
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(1, 0, 0),
                                    glm::vec3(1, 1, 0),
                                    glm::vec3(0, 0, 1),
                                    glm::vec3(0, 1, 1),
                                    glm::vec3(-1, 0, -1),
                                    glm::vec3(-1, 1, -1),
                                }), 5, 6, 5, 5);


void compile_cubes(std::vector<glm::vec3> cubes) {
    int vlen = sizeof(g_vertex_buffer_data)/sizeof(*g_vertex_buffer_data);
    int clen = cubes.size();
    g_compiled_vertex_data = new GLfloat[clen*vlen];
    g_compiled_color_data = new GLfloat[clen*vlen];
    compiled_length = clen*vlen*sizeof(GLfloat);
    compiled_vertices = clen*vlen;
    for(int i=0; i < clen;i++) {
        for(int v=0; v < vlen/3; v++) {
            g_compiled_vertex_data[i*vlen+v*3+0] = g_vertex_buffer_data[v*3+0]+2*cubes[i].x;
            g_compiled_vertex_data[i*vlen+v*3+1] = g_vertex_buffer_data[v*3+1]+2*cubes[i].y;
            g_compiled_vertex_data[i*vlen+v*3+2] = g_vertex_buffer_data[v*3+2]+2*cubes[i].z;
            g_compiled_color_data[i*vlen+v*3+0] = abs(cubes[i].x/3)+g_vertex_buffer_data[v*3+0]/10;
            g_compiled_color_data[i*vlen+v*3+1] = abs(cubes[i].y/3+g_vertex_buffer_data[v*3+1]/10);
            g_compiled_color_data[i*vlen+v*3+2] = abs(cubes[i].z/3)+g_vertex_buffer_data[v*3+2]/10;
        }
    }
}

/* input callbacks */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_PRESS) {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            drag = 1;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastMousePt->x = xpos;
            lastMousePt->y = ypos;
        } else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
            drag = 2;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastMousePt->x = xpos;
            lastMousePt->y = ypos;
        }
    } else {
        drag = 0;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    delta->x = xpos - lastMousePt->x;
    delta->y = ypos - lastMousePt->y;
    lastMousePt->x = xpos;
    lastMousePt->y = ypos;
    double xnorm = 2.0 * xpos / width;
    double ynorm = 2.0 * ypos / height;
    glm::vec4 ray_clip = glm::vec4(xnorm, ynorm, -1.0, 1.0);
    glm::vec4 ray_eye = glm::inverse(Projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    glm::vec3 cam = glm::vec3(glm::rotate(glm::mat4(1.0f), view->horiz, glm::vec3(0, 1, 0)) * 
                                glm::rotate(glm::mat4(1.0f), view->vert, glm::vec3(1, 0, 0)) * 
                                glm::scale(glm::mat4(1.0f), glm::vec3(view->zoom, view->zoom, view->zoom)) * 
                                glm::vec4(0, 0, 1, 1));
    glm::mat4 View = glm::lookAt(
        cam, // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    glm::vec3 ray_wor = glm::vec3(glm::inverse(View) * ray_eye);
    if(drag == 1) {
        view->horiz -= delta->x*0.01;
        view->vert -= delta->y*0.01;
    } else if(drag == 2) {
        view->transx += delta->x*0.01;
        view->transy += delta->y*0.01;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    view->zoom *= pow(1.01, yoffset);
    view->fov += xoffset*0.1;
    Projection = glm::perspective(glm::radians(view->fov), (float) width / (float)height, 0.1f, 100.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_I && action == GLFW_PRESS) {
        game.iterate();
        compile_cubes(game.get_cubes());
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, compiled_length, g_compiled_vertex_data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, compiled_length, g_compiled_color_data, GL_STATIC_DRAW);
    }
}

void window_size_callback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
    Projection = glm::perspective(glm::radians(view->fov), (float) width / (float)height, 0.1f, 100.0f);
}


glm::mat4 get_mvp(float zoom, float y_angle, float vert_angle, float transx, float transy) {
    
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    glm::vec3 cam = glm::vec3(glm::rotate(glm::mat4(1.0f), y_angle, glm::vec3(0, 1, 0)) * 
                                glm::rotate(glm::mat4(1.0f), vert_angle, glm::vec3(1, 0, 0)) * 
                                glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, zoom)) * 
                                glm::vec4(0, 0, 1, 1));
    glm::vec3 look = /*glm::cross(cam, glm::vec3(0, 1, 0)) * transx + */glm::vec3(0, 0, 0);
    
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        cam, // Camera is at (4,3,3), in World Space
        look, // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
    return mvp;
}



int main(int argc, char** argv) {
    std::cout << glm::to_string(Projection) << std::endl;

    GLFWwindow* window;

    /* Initialize the library */
    if ( !glfwInit() )
    {
        return -1;
    }

    #ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on OS X */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow( 800, 600, "3D GOL", NULL, NULL );
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Setup input handling */
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    // configure for display
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    compile_cubes(game.get_cubes());
    glBufferData(GL_ARRAY_BUFFER, compiled_length, g_compiled_vertex_data, GL_STATIC_DRAW);
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, compiled_length, g_compiled_color_data, GL_STATIC_DRAW);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "main/SimpleVertexShader.vertexshader", "main/SimpleFragmentShader.fragmentshader" );
    glUseProgram(programID);

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glm::mat4 mvp = get_mvp(view->zoom, view->horiz, view->vert, view->transx, view->transy);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, compiled_vertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwWaitEvents();
    }

    glfwTerminate();
    return 0;
}
