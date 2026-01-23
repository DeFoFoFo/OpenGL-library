#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ComputeShader.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include <iostream>
#include <vector>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

mylib::Camera camera{glm::vec3(0.0f, 0.0f, 1000.0f)};

int main()
{
    const int width = 800;
    const int height = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


    std::vector<glm::vec2> points;
    points.reserve(800*600);
    for (float i{-400}; i < 400; i += 0.25)
    {
        for (float j{-300}; j < 300; j += 0.25)
        {
            points.emplace_back(i, j);
        }
    }

    GLuint SSBO;
    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                points.size() * sizeof(glm::vec2),
                points.data(),
                GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

    // Create VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, SSBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    const GLuint LOCAL_SIZE = 256;

    mylib::ComputeShader comp{"compute.glsl"};
    mylib::Shader shader{"point_shader.vert", "point_shader.frag"};

    glEnable(GL_PROGRAM_POINT_SIZE);

    float dT{};
    float lastTime{};


    while (!glfwWindowShouldClose(window)) {
        float time = (float)glfwGetTime();
        dT = time - lastTime;
        lastTime = time;
        std::cout << 1/dT << " " << points.size() << std::endl;

        glUseProgram(comp.ID());
        glUniform1f(glGetUniformLocation(comp.ID(),"time"), time);
        glDispatchCompute((points.size() + LOCAL_SIZE - 1) / LOCAL_SIZE, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

        glClearColor(0.8, 0.5, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.ID());

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)800/600, 0.1f, 1000.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, points.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &SSBO);
    glDeleteVertexArrays(1, &VAO);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}