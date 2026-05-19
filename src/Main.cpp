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
constexpr uint32_t BOX_SIZE = 500;

void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

void processInput(mylib::InputManager* inputManager);

mylib::ShaderProgram boidShader;

mylib::Camera camera{ glm::vec3(0.0f, 0.0f, 10.0f) };
float lastX = WIN_WIDTH / 2;
float lastY = WIN_HEIGHT / 2;
bool firstMouse = true;

float dT{};
float lastTime{};

int main()
{
    mylib::Application app;
    mylib::Window& window = app.getWindow();
    window.createWindow(WIN_WIDTH, WIN_HEIGHT, "Boids");
    glfwSetWindowPos(app.getWindow().getHandle(), 0, 50);
    mylib::InputManager& inputManager = app.getInputManager();
    inputManager.connectWindow(&window);

    glfwSetCursorPosCallback(window.getHandle(), mouseCallback);
    glfwSetScrollCallback(window.getHandle(), scrollCallback);

    glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1);

    camera.setSpeed(20.0f);

    mylib::ComputeShader compBoid{ "src/shaders/boid.comp" };
    boidShader.assign("src/shaders/fish.vert", "src/shaders/boid.frag");

    mylib::ShaderProgram cubeShader{ "src/shaders/cube.vert", "src/shaders/cube.frag" };
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);
    cubeShader.bind();
    cubeShader.setUniform(color, "uColor");

    mylib::Model boidModel{ "assets/Pacific_blue_tang.glb" };

    const float radiusOfInfluence = 15.0f;
    const float radiusOfSeparation = 10.0f;
    const float maxSpeed = 15.0f;
    const float minSpeed = 3.0f;
    const float maxForce = 10.0f;

    uint32_t numBoids{ 20000 };
    std::vector<Boid> boids;
    boids.reserve(numBoids);
    for (size_t i{}; i < numBoids; ++i)
    {
        float xPos{ (float)mylib::randomNumberWithin(-BOX_SIZE / 2, BOX_SIZE / 2) };
        float yPos{ (float)mylib::randomNumberWithin(-BOX_SIZE / 2, BOX_SIZE / 2) };
        float zPos{ (float)mylib::randomNumberWithin(-BOX_SIZE / 2, BOX_SIZE / 2) };

        float xVelocity{ (float)mylib::randomNumberWithin(-maxSpeed, maxSpeed) };
        float yVelocity{ (float)mylib::randomNumberWithin(-maxSpeed, maxSpeed) };
        float zVelocity{ (float)mylib::randomNumberWithin(-maxSpeed, maxSpeed) };

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

    compBoid.bind();
    compBoid.setUniform(radiusOfInfluence, "radiusOfInfluence");
    compBoid.setUniform(radiusOfSeparation, "radiusOfSeparation");
    compBoid.setUniform(maxSpeed, "maxSpeed");
    compBoid.setUniform(minSpeed, "minSpeed");
    compBoid.setUniform(maxForce, "maxForce");
    compBoid.setUniform(BOX_SIZE, "boxSize");

    const GLuint LOCAL_SIZE = 256;

    GLfloat vertices[] = {
         1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   // v0,v1,v2,v3 (front)
         1.0f, 1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f,-1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   // v0,v3,v4,v5 (right)
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,  -1.0f, 1.0f, 1.0f,   // v0,v5,v6,v1 (top)
        -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,  -1.0f,-1.0f, 1.0f,   // v1,v6,v7,v2 (left)
        -1.0f,-1.0f,-1.0f,   1.0f,-1.0f,-1.0f,   1.0f,-1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,   // v7,v4,v3,v2 (bottom)
         1.0f,-1.0f,-1.0f,  -1.0f,-1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,   1.0f, 1.0f,-1.0f    // v4,v7,v6,v5 (back)
    };
    GLuint indices[] = {
         0,  1,  2,  2,  3,  0,   // v0-v1-v2, v2-v3-v0 (front)
         4,  5,  6,  6,  7,  4,   // v0-v3-v4, v4-v5-v0 (right)
         8,  9, 10, 10, 11,  8,   // v0-v5-v6, v6-v1-v0 (top)
        12, 13, 14, 14, 15, 12,   // v1-v6-v7, v7-v2-v1 (left)
        16, 17, 18, 18, 19, 16,   // v7-v4-v3, v3-v2-v7 (bottom)
        20, 21, 22, 22, 23, 20    // v4-v7-v6, v6-v5-v4 (back)
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mylib::Renderer renderer;

    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window.getHandle()))
    {
        float time = (float)glfwGetTime();
        // If framerate is lower than 5 fps, clamp it to avoid stutters
        dT = std::min(time - lastTime, 0.2f);
        lastTime = time;
        // std::cout << 1/dT << " " << dT * 1000 << std::endl;
        glfwSetWindowTitle(window.getHandle(), std::string{window.getName() + " " + std::to_string(1/dT)}.c_str());

        processInput(&inputManager);

        glCullFace(GL_BACK);

        SSBO[readIdx].bindBase(mylib::BufferTarget::SSBO, 0);
        SSBO[writeIdx].bindBase(mylib::BufferTarget::SSBO, 1);

        compBoid.bind();
        compBoid.setUniform(dT, "dT");
        compBoid.dispatch((numBoids + LOCAL_SIZE - 1) / LOCAL_SIZE, 1, 1);
        compBoid.barrier(mylib::MemoryBarrier::SSBO);

        std::swap(readIdx, writeIdx);

        renderer.backgroundColor(0.2f, 0.8f, 0.5f, 1.0f);
        renderer.clear(mylib::BufferBit::COLOR, mylib::BufferBit::DEPTH);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);

        // Model is computed inside compute shader
        boidShader.bind();
        boidShader.setUniform(view, "view");
        boidShader.setUniform(projection, "projection");
        boidShader.setUniform(time, "time");

        const std::vector<mylib::Mesh>& boidModelMeshes = boidModel.getMeshes();

        for (const auto& mesh : boidModelMeshes)
        {
            mesh.getVAO();
            SSBO[readIdx].bindAs(mylib::BufferTarget::SSBO);
            SSBO[readIdx].bindBase(mylib::BufferTarget::SSBO, 0);
        }

        renderer.drawInstanced(boidModel, numBoids, boidShader);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(BOX_SIZE / 2));

        cubeShader.bind();
        cubeShader.setUniform(model, "model");
        cubeShader.setUniform(view, "view");
        cubeShader.setUniform(projection, "projection");

        if (abs(camera.getPos().x) < static_cast<float>(BOX_SIZE) / 2 ||
            abs(camera.getPos().y) < static_cast<float>(BOX_SIZE) / 2 ||
            abs(camera.getPos().z) < static_cast<float>(BOX_SIZE) / 2)  // Inside the cube
            glCullFace(GL_FRONT);
        else  // Outside the cube
            glCullFace(GL_BACK);

        renderer.drawIndexed(cubeVAO, 36, cubeShader);

        glfwSwapBuffers(window.getHandle());
        glfwPollEvents();
    }
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

void processInput(mylib::InputManager* inputManager)
{
    using namespace mylib;

    if (inputManager->isJustPressed(Key::ESCAPE))
    {
        glfwSetWindowShouldClose(inputManager->getWindow()->getHandle(), true);
    }

    if (inputManager->isPressed(Key::W))
    {
        camera.processKeyboard(CameraMovement::FORWARD, dT);
    }
    if (inputManager->isPressed(Key::S))
    {
        camera.processKeyboard(CameraMovement::BACKWARD, dT);
    }
    if (inputManager->isPressed(Key::A))
    {
        camera.processKeyboard(CameraMovement::LEFT, dT);
    }
    if (inputManager->isPressed(Key::D))
    {
        camera.processKeyboard(CameraMovement::RIGHT, dT);
    }
    if (inputManager->isPressed(Key::SPACE))
    {
        camera.processKeyboard(CameraMovement::UP, dT);
    }
    if (inputManager->isPressed(Key::LEFT_SHIFT))
    {
        camera.processKeyboard(CameraMovement::DOWN, dT);
    }

    if (inputManager->isJustPressed(Key::F5))
        boidShader.recompile();
}