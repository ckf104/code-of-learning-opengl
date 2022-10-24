#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "hello.hpp"
#include "texture.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  std::cout << "[info] new width: " << width << " , height: " << height
            << std::endl;
  glViewport(0, 0, width, height);
}

float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
};

unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3,  // 第一个三角形
    1, 2, 3   // 第二个三角形
};
char infolog[512];
//const char* image_path = "container.jpg";
const char* image_path = "/home/ckf/tmp/background/acg.gy_14.jpg";
const char* image_path2 = "smile.png";

int main() {
  uint32_t VBO, VAO, EBO;
  uint32_t vertexShader;
  uint32_t fragmentShader;
  uint32_t shaderProgram;
  uint32_t texture;
  int width, height, nrChannels;
  int success;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  Shader myShader("./vshader.glsl", "./fshader.glsl");
  Texture tex0(0, image_path, "texture1", GL_RGB);
  Texture tex1(1, image_path2, "texture2", GL_RGBA);
  tex0.setPos(&myShader);
  tex1.setPos(&myShader);
  checkErr();  

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // 颜色属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  checkErr();
  // the binding of EBO must be after the VAO, when binding EBO, the VAO will
  // store info of EBO. Unbinding is reversed order.
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  checkErr();

  // glDrawArrays(GL_TRIANGLES, 0, 3);

  glViewport(0, 0, 800, 600);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    myShader.use();
    tex0.texActive();
    tex1.texActive();
    glBindVertexArray(VAO);  // seeing as we only have a single VAO there's no
                             // need to bind it every time, but we'll do so to
                             // keep things a bit more organized
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    checkErr();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    checkErr();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  myShader.destroy();
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  tex0.texDestroy();

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