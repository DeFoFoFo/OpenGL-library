#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ComputeShader.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Boid.hpp"

#include <iostream>
#include <vector>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

mylib::Camera camera{glm::vec3(0.0f, 0.0f, 10.0f)};

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

    const float speed = 1;
    uint32_t num_of_boids{10000};
    std::vector<Boid> boids;
    boids.reserve(num_of_boids);
    for (size_t i{}; i < num_of_boids; ++i)
    {
        float yPos{10.0f/num_of_boids * i};
        yPos -= 10.0f/2;
        boids.emplace_back(glm::vec4(0.0f, yPos, 0.0f, speed));
    }
    

    GLuint SSBO[2];
    glGenBuffers(2, SSBO);
    for (int i{}; i < 2; ++i)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO[i]);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                    num_of_boids * sizeof(Boid),
                    boids.data(),
                    GL_DYNAMIC_DRAW);
    }
    
    int readIdx = 0;
    int writeIdx = 1;

    // Create VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, SSBO[readIdx]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Boid), (void*)0);

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
        std::cout << 1/dT << " " << num_of_boids << std::endl;

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO[readIdx]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO[writeIdx]);

        glUseProgram(comp.ID());
        glUniform1f(glGetUniformLocation(comp.ID(), "time"), time);
        glDispatchCompute((num_of_boids + LOCAL_SIZE - 1) / LOCAL_SIZE, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

        std::swap(readIdx, writeIdx);

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
        glBindBuffer(GL_ARRAY_BUFFER, SSBO[readIdx]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Boid), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_POINTS, 0, num_of_boids);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(2, SSBO);
    glDeleteVertexArrays(1, &VAO);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}