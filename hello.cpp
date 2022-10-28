#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "hello.hpp"
#include "texture.hpp"
#include "camera.hpp"

int screen_w = 800, screen_h = 600;
float lastX = screen_w / 2.0f;
float lastY = screen_h / 2.0f;
int ncubes = 2;
Camera camera(screen_w, screen_h);

float vertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1
    // (together with GL_REPEAT as texture wrapping mode). this will cause the
    // floor texture to repeat)
    5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
    0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,

    5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
    0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f};

glm::vec3 cubePositions[] = {
    glm::vec3(-1.0f, 0.001f, -1.0f), glm::vec3(2.0f, 0.001f, 0.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),  glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),   glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),    glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 lampPos = glm::vec3(1.2f, 1.0f, 2.0f);

unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3,  // 第一个三角形
    1, 2, 3   // 第二个三角形
};
char infolog[512];
const char* image_path = "marble.jpg";
// const char* image_path = "/home/ckf/tmp/background/acg.gy_14.jpg";
const char* image_path2 = "metal.png";

#define setUniformMat(id, name)                                    \
  glUniformMatrix4fv(glGetUniformLocation(id, #name), 1, GL_FALSE, \
                     glm::value_ptr(name));

/*void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods) {
  // if (action == GLFW_RELEASE){std::cout << glfwGetKeyName(key, scancode) <<
  // std::endl;};
  if (key == GLFW_KEY_W)
    cameraPos += cameraSpeed * cameraFront;
  else if (key == GLFW_KEY_S)
    cameraPos -= cameraSpeed * cameraFront;
  else if (key == GLFW_KEY_A)
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  else if (key == GLFW_KEY_D)
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}*/

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  std::cout << "[info] xpos: " << xposIn << ", ypos: " << yposIn << std::endl;
  static bool firstMouse = true;
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos;  // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
/*
void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}*/

void process_input(GLFWwindow* window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  std::cout << "[info] new width: " << width << " , height: " << height
            << std::endl;
  screen_w = width, screen_h = height;
  glViewport(0, 0, width, height);
  camera.SetScreen(screen_w, screen_h);
}

int main() {
  uint32_t VBO, VAO, EBO, floorVAO, floorVBO;
  uint32_t vertexShader;
  uint32_t fragmentShader;
  uint32_t texture;
  int width, height, nrChannels;
  int success;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
      glfwCreateWindow(screen_w, screen_h, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(
      window);  // appoint subsequent gl calls to have an effect on which window
                // for this thread, call
  // this before calling gladLoadGLLoader
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  // TODO: why cursor position update is deferred when GLFW_CURSOR_DISABLED is
  // set. glfwSetKeyCallback(window, key_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  Shader myShader("./vshader.glsl", "./fshader.glsl");
  Shader simpleShader("./vshader.glsl", "./simplefshader.glsl");
  Texture tex0(0, image_path, "texture1", GL_RGB);
  Texture tex1(0, image_path2, "texture1", GL_RGB);
  /*tex0.setPos(&myShader);
  tex1.setPos(&myShader);*/
  checkErr();

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);  // normal vector */

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);  // normal vector

  glGenBuffers(1, &floorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &floorVAO);
  glBindVertexArray(floorVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 颜色属性
  /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);*/
  checkErr();
  // the binding of EBO must be after the VAO, when binding EBO, the VAO will
  // store info of EBO. Unbinding is reversed order.
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  checkErr();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 0xff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilMask(0xff);  // behaviour: always pass stencil test, and change
                        // value into 1

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);  // default behaviour
  glClearStencil(0);   // default behaviour
  glViewport(0, 0, 800, 600);

  while (!glfwWindowShouldClose(window)) {
    float lastFrame = (float)glfwGetTime();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glm::mat4 view = camera.GetView();
    glm::mat4 projection = camera.GetProjection();

    glStencilMask(0x00);
    glBindVertexArray(floorVAO);
    myShader.use();
    tex1.texActive();

    myShader.setFMat4("view", view);
    myShader.setFMat4("projection", projection);
    glm::mat4 model(1.0f);
    myShader.setFMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xff);
    tex0.texActive();
    glBindVertexArray(VAO);  // seeing as we only have a single VAO there's no
                             // need to bind it every time, but we'll do so to
                             // keep things a bit more organized

    // result is trans * rotate. rotate is first and counter-clockwise
    // direction!
    /*glm::mat4 trans(1.0f);
    trans =
        glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(myShader.getid(), "transform"), 1,
                       GL_FALSE, glm::value_ptr(trans));*/

    myShader.setFMat4("view", view);
    myShader.setFMat4("projection", projection);

    for (int i = 0; i < ncubes; ++i) {
      glm::mat4 model(1.0f);
      model = glm::translate(model, cubePositions[i]);
      /*model = glm::rotate(model, (float)glfwGetTime() * glm::radians(5.0f *
         i), glm::vec3(1.0f, 0.3f, 0.5f));*/
      // glm::mat3 model_inverse_trans(model);  // 4x4 -> 3x3
      myShader.setFMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glDisable(GL_DEPTH_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);
    simpleShader.use();
    simpleShader.setFMat4("view", view);
    simpleShader.setFMat4("projection", projection);
    for (int i = 0; i < ncubes; ++i) {
      glm::mat4 model(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::scale(model, glm::vec3(1.05f, 1.05f, 1.05f));
      simpleShader.setFMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    process_input(window, (float)glfwGetTime() - lastFrame);
    checkErr();
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    checkErr();
    glfwSwapBuffers(window);
    glfwPollEvents();

    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);
    glEnable(GL_DEPTH_TEST);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &floorVAO);
  myShader.destroy();
  // lampShader.destroy();
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &floorVBO);
  /*tex0.texDestroy();
  tex1.texDestroy();*/

  glfwTerminate();
  return 0;
}

int shaderlog(const char* level, uint32_t shader) {
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE) {
    glGetShaderInfoLog(shader, sizeof(infolog), NULL, infolog);
    std::cout << level << infolog << std::endl;
  }
  return success;
}
int programlog(const char* level, uint32_t program) {
  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success != GL_TRUE) {
    glGetProgramInfoLog(program, sizeof(infolog), NULL, infolog);
    std::cout << level << infolog << std::endl;
  }
  return success;
}

void checkErr() {
  uint32_t status = glGetError();
  if (status != GL_NO_ERROR) {
    std::cout << status << std::endl;
  }
}