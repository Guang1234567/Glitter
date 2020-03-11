// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

const char* contentsOfShaderSource(const char* filePath);

void framebuffer_size_changed(GLFWwindow* window, int width, int height);

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
     #endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    if (0 == gladLoadGL()) {
      fprintf(stderr, "Failed to initialize GLAD\n");
      glfwTerminate();
      return EXIT_FAILURE;
    }
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_changed);
    
    // build and compile our shader program
    // ------------------------------------
    int success = 0;
    GLShader glShader("./vertexShaderSource.vert", "./fragmentShaderSource.frag", &success);
    if (0 == success) {
        fprintf(stderr, "Failed to Create OpenGL Shader\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
     // 三角形
    float vertices[] = {
        // position          // color
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
    };
     
     unsigned int indices[] = { // 注意索引从0开始!
         0, 1, 2
     };
    
    // 矩形
//    float vertices[] = {
//        0.5f, 0.5f, 0.0f,   // 右上角
//        0.5f, -0.5f, 0.0f,  // 右下角
//        -0.5f, -0.5f, 0.0f, // 左下角
//        -0.5f, 0.5f, 0.0f   // 左上角
//    };
//
//    unsigned int indices[] = { // 注意索引从0开始!
//        0, 1, 3, // 第一个三角形
//        1, 2, 3  // 第二个三角形
//    };
    
    // VAO
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    // VBO
    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) /*bytes*/, vertices, GL_STATIC_DRAW);
    // link VBO to Vertex shader
    // ---------------------------
    // link position vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float) /*stride*/, (void *)0);
    glEnableVertexAttribArray(0);
    // link color vertex data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float) /*stride*/,
                          (void *)(3 * sizeof(float)) /*offset*/);
    glEnableVertexAttribArray(1);
    
    //EBO
    unsigned int EBO = 0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // unbind EBO, VBO and VAO after linked VBO to Vertex shader
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(mWindow, true);
        }

        // Background Fill Color
        // glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glShader.use();
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        glShader.setFloat("myColor", 0.0f, greenValue, 0.0f);
        
        glBindVertexArray(VAO); // bind VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind EBO
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // unbind EBO
        glBindVertexArray(0); // unbind VAO

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_changed(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
