#include <ctime>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>

#define GLFW_INCLUDE_GLCOREARB
// #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "main/loadShader.hpp"

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
glm::mat4 Projection = glm::perspective(glm::radians(view->fov), (float) width / (float)height, 0.1f, 100.0f);

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

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
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

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
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwWaitEvents();
    }

    glfwTerminate();
    return 0;
}
