#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Mylib.hpp"

#include "Boid.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>

constexpr uint32_t WIN_WIDTH = 1600;
constexpr uint32_t WIN_HEIGHT = 1200;
constexpr uint32_t boxSize = 200;

void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

int randomNumberWithin(int min, int max);

mylib::Camera camera{glm::vec3(0.0f, 0.0f, 10.0f)};
float lastX = WIN_WIDTH / 2;
float lastY = WIN_HEIGHT / 2;
bool firstMouse = true;

float dT{};
float lastTime{};

int main()
{
    glfwInit();
    mylib::Window window{WIN_WIDTH, WIN_HEIGHT, "Boids"};
    
    glfwSetCursorPosCallback(window.getHandle(), mouseCallback);
    glfwSetScrollCallback(window.getHandle(), scrollCallback);

    glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(0);

    mylib::Sampler sampler2D{mylib::TextureDimension::DIM2};
    sampler2D.addWrapParameter(mylib::Wrap::WRAP_R, mylib::WrapParam::REPEAT);
    sampler2D.addWrapParameter(mylib::Wrap::WRAP_S, mylib::WrapParam::REPEAT);
    sampler2D.addMagParameter(mylib::MinMagFilter::MAG, mylib::MinMagFilterParam::NEAREST);
    sampler2D.addMagParameter(mylib::MinMagFilter::MIN, mylib::MinMagFilterParam::NEAREST);
    GLint maxUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
    std::cout << "Max texture units: " << maxUnits << std::endl;
    for (GLint i{}; i < maxUnits; ++i)
    {
        sampler2D.bind(i);
    }

    camera.setSpeed(20.0f);
    
    mylib::ComputeShader compBoid{"src/shaders/boid.comp"};
    mylib::Shader boidShader{"src/shaders/fish.vert", "src/shaders/boid.frag"};

    mylib::Shader cubeShader{"src/shaders/cube.vert", "src/shaders/cube.frag"};

    mylib::Model bird{"assets/Clown_fish.glb"};

    const float radiusOfInfluence = 15.0f;
    const float radiusOfSeparation = 5.0f;
    const float maxSpeed = 15.0f;
    const float minSpeed = 3.0f;
    const float maxForce = 10.0f;

    uint32_t numBoids{5000};
    std::vector<Boid> boids;
    boids.reserve(numBoids);
    for (size_t i{}; i < numBoids; ++i)
    {
        float xPos{(float)randomNumberWithin(-boxSize/2, boxSize/2)};
        float yPos{(float)randomNumberWithin(-boxSize/2, boxSize/2)};
        float zPos{(float)randomNumberWithin(-boxSize/2, boxSize/2)};

        float xVelocity{(float)randomNumberWithin(-maxSpeed, maxSpeed)};
        float yVelocity{(float)randomNumberWithin(-maxSpeed, maxSpeed)};
        float zVelocity{(float)randomNumberWithin(-maxSpeed, maxSpeed)};

        boids.emplace_back(
            glm::vec3(xPos, yPos, zPos),
            glm::vec3(xVelocity, yVelocity, zVelocity)
        );
    }

    mylib::Buffer SSBO[2];
    for (int i{}; i < 2; ++i)
    {
        SSBO[i].bindAs(mylib::BufferTarget::SSBO);
        SSBO[i].fill(mylib::BufferTarget::SSBO, numBoids * sizeof(Boid), boids.data(), GL_STREAM_DRAW);
    }

    int readIdx = 0;
    int writeIdx = 1;

    compBoid.setUniform(radiusOfInfluence, "radiusOfInfluence");
    compBoid.setUniform(radiusOfSeparation, "radiusOfSeparation");
    compBoid.setUniform(maxSpeed, "maxSpeed");
    compBoid.setUniform(minSpeed, "minSpeed");
    compBoid.setUniform(maxForce, "maxForce");
    compBoid.setUniform(boxSize, "boxSize");

    const GLuint LOCAL_SIZE = 256;

    GLfloat vertices[]  = {
        1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,  1.0f,-1.0f, 1.0f, // v0,v1,v2,v3 (front)
        1.0f, 1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f,-1.0f,-1.0f,  1.0f, 1.0f,-1.0f, // v0,v3,v4,v5 (right)
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f,-1.0f,  -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f, // v0,v5,v6,v1 (top)
        -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, // v1,v6,v7,v2 (left)
        -1.0f,-1.0f,-1.0f,   1.0f,-1.0f,-1.0f,  1.0f,-1.0f, 1.0f, -1.0f,-1.0f, 1.0f, // v7,v4,v3,v2 (bottom)
        1.0f,-1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,  1.0f, 1.0f,-1.0f  // v4,v7,v6,v5 (back)
    };
    GLuint indices[] = {
        0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
        4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
        8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
        12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
        16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
        20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
    };

    mylib::VertexArray cubeVAO;

    mylib::VertexBufferLayout cubeLayout;
    cubeLayout.push(3, GL_FLOAT);

    mylib::Buffer cubeVBO;
    cubeVBO.bindAs(mylib::BufferTarget::VBO);
    cubeVBO.fill(mylib::BufferTarget::VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
    cubeVAO.addBuffer(cubeVBO, cubeLayout);

    mylib::Buffer cubeEBO;
    cubeEBO.bindAs(mylib::BufferTarget::EBO);
    cubeEBO.fill(mylib::BufferTarget::EBO, sizeof(indices), indices, GL_STATIC_DRAW);

    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);
    cubeShader.setUniform(color, "uColor");
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mylib::Renderer renderer;

    while (!glfwWindowShouldClose(window.getHandle()))
    {
        float time = (float)glfwGetTime();
        // If framerate is lower than 5 fps, clamp it to avoid stutters
        dT = std::min(time - lastTime, 0.2f);
        lastTime = time;
        // std::cout << 1/dT << std::endl;

        processInput(window.getHandle());

        SSBO[readIdx].bindBase(mylib::BufferTarget::SSBO, 0);
        SSBO[writeIdx].bindBase(mylib::BufferTarget::SSBO, 1);

        compBoid.bind();
        compBoid.setUniform(dT, "dT");
        compBoid.dispatch((numBoids + LOCAL_SIZE - 1) / LOCAL_SIZE, 1, 1);
        compBoid.barrier(mylib::MemoryBarrier::SSBO);

        std::swap(readIdx, writeIdx);

        renderer.backgroundColor(0.1f, 0.1f, 0.1f, 1.0f);
        renderer.clear();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.0f);
        
        // Model is computed inside compute shader
        boidShader.setUniform(view, "view");
        boidShader.setUniform(projection, "projection");
        boidShader.setUniform(time, "time");

        const std::vector<mylib::Mesh>& birdMeshes = bird.getMeshes();
        
        for (const auto& mesh : birdMeshes)
        {
            mesh.getVAO();
            SSBO[readIdx].bindAs(mylib::BufferTarget::SSBO);
            SSBO[readIdx].bindBase(mylib::BufferTarget::SSBO, 0);
        }

        renderer.drawInstanced(bird, numBoids, boidShader);
        
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(boxSize/2));

        cubeShader.setUniform(model, "model");
        cubeShader.setUniform(view, "view");
        cubeShader.setUniform(projection, "projection");

        cubeVAO.bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window.getHandle());
        glfwPollEvents();
    }

    glfwDestroyWindow(window.getHandle());
    glfwTerminate();
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