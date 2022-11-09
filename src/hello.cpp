#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "hello.hpp"
#include "texture.hpp"
#include "game.hpp"
#include "resource_manager.hpp"

Game Breakout(SCREEN_W, SCREEN_H);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  std::cout << "[info] new width: " << width << " , height: " << height
            << std::endl;
  glViewport(0, 0, width, height);
  Breakout.SetScreen(width, height);
}

GLFWwindow* window_init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_RESIZABLE, false);

  GLFWwindow* window =
      glfwCreateWindow(SCREEN_W, SCREEN_H, "Breakout", NULL, NULL);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(
      window);  // appoint subsequent gl calls to have an effect on which window
                // for this thread, call
  // this before calling gladLoadGLLoader
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  /*glfwSetScrollCallback(window, scroll_callback);
  glfwSetCursorPosCallback(window, mouse_callback);*/
  // TODO: why cursor position update is deferred when GLFW_CURSOR_DISABLED is
  // set. glfwSetKeyCallback(window, key_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }
  return window;
}

int main() {
  uint32_t VBO, VAO, EBO, lampVAO;
  uint32_t vertexShader;
  uint32_t fragmentShader;
  uint32_t texture;
  int width, height, nrChannels;
  int success;
  GLFWwindow* window = window_init();
  Breakout.Init();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glViewport(0, 0, 800, 600);

  while (!glfwWindowShouldClose(window)) {
    float lastFrame = (float)glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT);
    Breakout.Render();

    float dt = (float)glfwGetTime() - lastFrame;
    Breakout.Update(dt, window);
    checkErr();
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    checkErr();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  ResourceManager::Clear();
  
  glfwTerminate();
  return 0;
}

void checkErr() {
  uint32_t status = glGetError();
  if (status != GL_NO_ERROR) {
    std::cout << status << std::endl;
  }
}