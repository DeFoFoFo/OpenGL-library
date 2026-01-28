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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Test", NULL, NULL);
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
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1);

    const float radius_of_neighbours = 0.5f;
    uint32_t num_of_boids{100000};
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

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, SSBO[readIdx]);

    glBindVertexArray(0);

    const GLuint LOCAL_SIZE = 256;

    mylib::ComputeShader comp_boid{"boid.glsl"};
    mylib::Shader boid_shader{"boid_shader.vert", "boid_shader.frag"};

    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        float time = (float)glfwGetTime();
        // If framerate is lower than 30 fps, clamp it to avoid stutters
        dT = std::min(time - lastTime, 0.033f);
        lastTime = time;
        std::cout << 1/dT << " " << num_of_boids << std::endl;

        processInput(window);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO[readIdx]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO[writeIdx]);

        glUseProgram(comp_boid.ID());
        glUniform1f(glGetUniformLocation(comp_boid.ID(), "dT"), dT);
        glDispatchCompute((num_of_boids + LOCAL_SIZE - 1) / LOCAL_SIZE, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

        std::swap(readIdx, writeIdx);

        glClearColor(0.8, 0.5, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(boid_shader.ID());

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(boid_shader.ID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(boid_shader.ID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(boid_shader.ID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

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
    if (max <= min)
    {
        std::cerr << "min need to be smaller than max" << std::endl;
        return 0;
    }

    int range = max - min + 1;
    int rnd = rand() % range;
    return min + rnd;
}