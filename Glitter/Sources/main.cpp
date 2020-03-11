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

int main(int argc, char *argv[]) {

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
  GLShader glShader("./vertexShaderSource.vert", "./fragmentShaderSource.frag",
                    &success);
  if (0 == success) {
    fprintf(stderr, "Failed to Create OpenGL Shader\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  // 三角形
  //    float vertices[] = {
  //        // position          // color
  //        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
  //         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
  //         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
  //    };
  //
  //     unsigned int indices[] = { // 注意索引从0开始!
  //         0, 1, 2
  //     };
  //
  //    float texCoords[] = {
  //        0.0f, 0.0f,
  //        1.0f, 0.0f,
  //        0.5f, 1.0f
  //    };

  // 矩形
  float vertices[] = {
      //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
  };

  unsigned int indices[] = {
      // 注意索引从0开始!
      0, 1, 3, // 第一个三角形
      1, 2, 3  // 第二个三角形
  };

  // VAO
  unsigned int VAO = 0;
  glGenVertexArrays(1, &VAO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  // VBO
  unsigned int VBO = 0;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) /*bytes*/, vertices,
               GL_STATIC_DRAW);
  // link VBO to Vertex shader
  // ---------------------------
  // link position vertex data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float) /*stride*/,
                        (void *)0);
  glEnableVertexAttribArray(0);
  // link color vertex data
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float) /*stride*/,
                        (void *)(3 * sizeof(float)) /*offset*/);
  glEnableVertexAttribArray(1);
  // link 2d-texture vertex data
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float) /*stride*/,
                        (void *)(6 * sizeof(float)) /*offset*/);
  glEnableVertexAttribArray(2);

  // EBO
  unsigned int EBO = 0;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Some 2D-TO
  // ----------
  // first 2d-texture
  unsigned int texture0;
  glGenTextures(1, &texture0);
  glBindTexture(GL_TEXTURE_2D, texture0);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 加载并生成纹理
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  if (nullptr != data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture0" << std::endl;
  }
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0); // unbind

  // second 2d-texture
  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 加载并生成纹理
  data =
      stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
  if (nullptr != data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture1" << std::endl;
  }
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0); // unbind

  // unbind 2D-TO, EBO, VBO and VAO after linked VBO to Vertex shader
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
    glBindVertexArray(VAO);                     // bind VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind EBO

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    glShader.use();
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    glShader.setFloat("myColor", 0.0f, greenValue, 0.0f);
      
    glShader.setInt("ourTexture0", GL_TEXTURE0 - GL_TEXTURE0 /*GL_TEXTURE0*/);
    glShader.setInt("ourTexture1", GL_TEXTURE1 - GL_TEXTURE0 /*GL_TEXTURE1*/);
      
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
      
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
    glBindVertexArray(0);                       // unbind VAO

    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture0);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return EXIT_SUCCESS;
}

void framebuffer_size_changed(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
