#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Defines.hpp"
#include "ComputeShader.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Boid.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "Renderer.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>

constexpr uint32_t WIN_WIDTH = 1600;
constexpr uint32_t WIN_HEIGHT = 1200;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

int randomNumberWithin(int min, int max);

mylib::Camera camera{glm::vec3(0.0f, 0.0f, 25.0f)};
float lastX = WIN_WIDTH / 2;
float lastY = WIN_HEIGHT / 2;
bool firstMouse = true;

float dT{};
float lastTime{};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Test", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize glad" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(0);

    const float radius_of_neighbours = 2.0f;
    uint32_t num_of_boids{1000};
    std::vector<Boid> boids;
    boids.reserve(num_of_boids);
    for (size_t i{}; i < num_of_boids; ++i)
    {
        float xPos{(float)randomNumberWithin(-10, 10)};
        float yPos{(float)randomNumberWithin(-10, 10)};
        float zPos{(float)randomNumberWithin(-10, 10)};

        float xVelocity{(float)randomNumberWithin(-2, 2)};
        float yVelocity{(float)randomNumberWithin(-2, 2)};
        float zVelocity{(float)randomNumberWithin(-2, 2)};

        boids.emplace_back(
            glm::vec4(xPos, yPos, zPos, radius_of_neighbours),
            glm::vec4(xVelocity, yVelocity, zVelocity, 0.0f)
        );
    }

    mylib::Buffer SSBO[2];
    for (int i{}; i < 2; ++i)
    {
        SSBO[i].bindAs(mylib::BufferTarget::SSBO);
        SSBO[i].fill(mylib::BufferTarget::SSBO, num_of_boids * sizeof(Boid), boids.data(), GL_STREAM_DRAW);
    }
    
    int readIdx = 0;
    int writeIdx = 1;

    mylib::VertexBufferLayout boidLayout;
    boidLayout.push(4, GL_FLOAT);
    boidLayout.push(4, GL_FLOAT);

    mylib::VertexArray boidVAO;
    boidVAO.addBuffer(SSBO[readIdx], boidLayout);

    const GLuint LOCAL_SIZE = 256;

    mylib::ComputeShader comp_boid{"boid.glsl"};
    mylib::Shader boid_shader{"boid_shader.vert", "boid_shader.frag"};

    mylib::Shader cube_shader{"cube.vert", "cube.frag"};

    GLfloat vertices[]  = {
        .5f, .5f, .5f,  -.5f, .5f, .5f,  -.5f,-.5f, .5f,  .5f,-.5f, .5f, // v0,v1,v2,v3 (front)
        .5f, .5f, .5f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,  .5f, .5f,-.5f, // v0,v3,v4,v5 (right)
        .5f, .5f, .5f,   .5f, .5f,-.5f,  -.5f, .5f,-.5f, -.5f, .5f, .5f, // v0,v5,v6,v1 (top)
        -.5f, .5f, .5f,  -.5f, .5f,-.5f,  -.5f,-.5f,-.5f, -.5f,-.5f, .5f, // v1,v6,v7,v2 (left)
        -.5f,-.5f,-.5f,   .5f,-.5f,-.5f,   .5f,-.5f, .5f, -.5f,-.5f, .5f, // v7,v4,v3,v2 (bottom)
        .5f,-.5f,-.5f,  -.5f,-.5f,-.5f,  -.5f, .5f,-.5f,  .5f, .5f,-.5f  // v4,v7,v6,v5 (back)
    };
    GLuint indices[] = {
        0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
        4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
        8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
        12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
        16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
        20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
    };

    mylib::Buffer cubeVBO;
    cubeVBO.bindAs(mylib::BufferTarget::VBO);
    cubeVBO.fill(mylib::BufferTarget::VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    mylib::Buffer cubeEBO;
    cubeEBO.bindAs(mylib::BufferTarget::EBO);
    cubeEBO.fill(mylib::BufferTarget::EBO, sizeof(indices), indices, GL_STATIC_DRAW);

    mylib::VertexBufferLayout cubeLayout;
    cubeLayout.push(3, GL_FLOAT);

    mylib::VertexArray cubeVAO;
    cubeVAO.addBuffer(cubeVBO, cubeLayout);

    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);
    cube_shader.bind();
    glUniform4fv(glGetUniformLocation(cube_shader.getID(), "uColor"), 1, glm::value_ptr(color));

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        float time = (float)glfwGetTime();
        // If framerate is lower than 30 fps, clamp it to avoid stutters
        dT = std::min(time - lastTime, 0.033f);
        lastTime = time;
        // std::cout << 1/dT << " " << num_of_boids << std::endl;

        processInput(window);

        SSBO[readIdx].bindBase(mylib::BufferTarget::SSBO, 0);
        SSBO[writeIdx].bindBase(mylib::BufferTarget::SSBO, 1);

        comp_boid.bind();
        glUniform1f(glGetUniformLocation(comp_boid.getID(), "dT"), dT);
        comp_boid.dispatch((num_of_boids + LOCAL_SIZE - 1) / LOCAL_SIZE, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

        std::swap(readIdx, writeIdx);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 100.0f);

        boid_shader.bind();
        
        glUniformMatrix4fv(glGetUniformLocation(boid_shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(boid_shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(boid_shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(boid_shader.getID(), "viewPos"), 1, glm::value_ptr(camera.getPos()));

        boidVAO.bind();
        SSBO[readIdx].bindAs(mylib::BufferTarget::VBO);
                
        glDrawArrays(GL_POINTS, 0, num_of_boids);
        
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(20.0f));

        cube_shader.bind();

        glUniformMatrix4fv(glGetUniformLocation(cube_shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(cube_shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cube_shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        cubeVAO.bind();
        cubeEBO.bindAs(mylib::BufferTarget::EBO);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse)
    {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos; // y coordinates are reversed

	lastX = xPos;
	lastY = yPos;

	camera.processMouse(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processScroll(yOffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
		glfwSetWindowShouldClose(window, true);
	}

    using namespace mylib;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
		camera.processKeyboard(CameraMovement::FORWARD, dT);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
		camera.processKeyboard(CameraMovement::BACKWARDS, dT);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
		camera.processKeyboard(CameraMovement::LEFT, dT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
		camera.processKeyboard(CameraMovement::RIGHT, dT);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
		camera.processKeyboard(CameraMovement::UP, dT);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
		camera.processKeyboard(CameraMovement::DOWN, dT);
	}
}

int randomNumberWithin(int min, int max)
{
    if (max < min)
        std::swap(min,max);

    int range = max - min + 1;
    int rnd = rand() % range;
    return min + rnd;
}