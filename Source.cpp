#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <Shape.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

namespace
{
    const char* const WINDOW_TITLE = "William Brandow | CS-330 Project"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 2400;
    const int WINDOW_HEIGHT = 1600;

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;

    // pencil data
    Shape pencilShaft;
    Shape pencilTip;
    Shape pencilEraserHolder;
    Shape pencilEraser;
    Shape pencilEraserTop;
    Shape pencilGraphite;

    // plane data
    Shape plane;

    // book data
    Shape bookPages;
    Shape bookTopCover;
    Shape bookBottomCover;
    Shape bookSpine;

    // orange data
    Shape orange;

    // mug data
    Shape mugOuterWall;
    Shape mugCoffee;
    Shape mugBase;
    Shape mugHandle;

    // texture data
    GLuint texture0, texture1, texture2, texture3;

    // create camera variables
    float speedVariable = 2.5f;
    glm::vec3 cameraPos = glm::vec3(1.0f, 2.0f, 8.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // variables for tracking time between frames
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // initial cursor position
    bool initialMouse = true;
    float lastX = 400;
    float lastY = 300;
    float pitch;
    float yaw;

    bool projectionToggle = true;

    // lighting
    glm::vec3 objectColor = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3(5.0, 2.0, 5.0);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

void URender(Shader myShader);
void UProcessInput(GLFWwindow* window);
bool UInitialize(int argc, char* argv[], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void createTexture(GLenum textureUnit, GLuint& texture, const char* texturePath, GLenum textureWrapping);
void destroyTexture(GLuint texture);


int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Shader program
    Shader myShader("./shader.vs", "./shader.fs");

    // use shader
    myShader.use();

    createTexture(GL_TEXTURE0, texture0, "./texture_pencilEraserHolder.jpg", GL_REPEAT);
    myShader.setInt("texture0", 0);
    createTexture(GL_TEXTURE1, texture1, "./texture_pencilTip.jpg", GL_REPEAT);
    myShader.setInt("texture1", 1);
    createTexture(GL_TEXTURE2, texture2, "./texture_plane3.jpg", GL_REPEAT);
    myShader.setInt("texture2", 2);
    createTexture(GL_TEXTURE3, texture3, "./adventuresOfBilbroSwaggins.jpg", GL_REPEAT);
    myShader.setInt("texture3", 3);

    // create plane
    plane.createPlane(plane, 10.0f, 10.0f);

    // create pencil shapes
    pencilShaft.createCylinder(pencilShaft, 0.06f, 0.06f, 1.0f, 10);
    pencilTip.createCylinder(pencilTip, 0.06f, 0.01f, 0.18f, 10);
    pencilEraserHolder.createCylinder(pencilEraserHolder, 0.06f, 0.06f, 0.15f, 10);
    pencilEraser.createCylinder(pencilEraser, 0.06f, 0.06f, 0.1f, 10);
    pencilEraserTop.createDisk(pencilEraserTop, 0.06, 10);
    pencilGraphite.createCylinder(pencilGraphite, 0.01f, 0.0f, 0.02f, 10);

    // create book shapes
    bookPages.createCube(bookPages, 1.3f, 2.0f, 0.3f);
    bookTopCover.createCube(bookTopCover, 1.31f, 2.03f, 0.01f);
    bookBottomCover.createCube(bookBottomCover, 1.31f, 2.03f, 0.01f);
    bookSpine.createCube(bookSpine, 0.01f, 2.03f, 0.3f);

    // create orange
    orange.createSphere(orange, 0.3f, 10, 5);

    // create mug
    mugOuterWall.createCylinder(mugOuterWall, 0.4f, 0.4f, 0.7f, 10);
    mugBase.createDisk(mugBase, 0.4f, 10);
    mugCoffee.createDisk(mugCoffee, 0.4f, 10);
    mugHandle.createTorus(mugHandle, 0.15f, 0.045f, 10, 10);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // calculate time between frames
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        UProcessInput(gWindow);

        // Render this frame
        URender(myShader);

        glfwPollEvents();
    }

    // release memory from pencil data
    pencilShaft.destroyShape(pencilShaft);
    pencilTip.destroyShape(pencilTip);
    pencilEraserHolder.destroyShape(pencilEraserHolder);
    pencilEraser.destroyShape(pencilEraser);
    pencilEraserTop.destroyShape(pencilEraserTop);
    pencilGraphite.destroyShape(pencilGraphite);

    // release memory from book data
    bookPages.destroyShape(bookPages);
    bookTopCover.destroyShape(bookTopCover);
    bookBottomCover.destroyShape(bookBottomCover);
    bookSpine.destroyShape(bookSpine);

    // release memory from orange data
    orange.destroyShape(orange);

    // release memory from mug
    mugOuterWall.destroyShape(mugOuterWall);
    mugBase.destroyShape(mugBase);
    mugCoffee.destroyShape(mugCoffee);
    mugHandle.destroyShape(mugHandle);

    // release memory from plane data
    plane.destroyShape(plane);

    // release texture memory
    destroyTexture(texture0);
    destroyTexture(texture1);
    destroyTexture(texture2);

    // Release shader program
    myShader.destroy();

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Functioned called to render a frame
void URender(Shader myShader)
{
    // Clear the background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. Scales the object by 2
    glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
    // 2. Rotates shape by 15 degrees in the x axis
    glm::mat4 rotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0f));
    // 3. Place object at the origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Transforms the camera
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Creates a perspective projection (field of view, aspect ratio, near plane, and far plane are the four parameters)
    glm::mat4 projection;
    if (projectionToggle == true)
    {
        projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    else
    {
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    }

    // Retrieves and passes transform matrices to the Shader program
    myShader.setMat4("model", model);
    myShader.setMat4("view", view);
    myShader.setMat4("projection", projection);

    // pass lighting uniforms
    // myShader.setFloat3("objectColor", objectColor.r, objectColor.g, objectColor.b);
    myShader.setFloat3("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);
    myShader.setFloat3("lightColor", lightColor.r, lightColor.g, lightColor.b);
    myShader.setFloat3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

    // Draws the plane
    glBindVertexArray(plane.vao);
    myShader.setInt("useTexture", 2);
    translation = glm::translate(translation, glm::vec3(0.0f, -0.14f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    // glDrawElements(GL_TRIANGLES, plane.nIndices, GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, plane.nVertices);

    // Draws the pencil - shaft
    glBindVertexArray(pencilShaft.vao);
    myShader.setFloat3("objectColor", 0.8f, 0.6f, 0.3f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(0.0f, 0.12f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    // glDrawElements(GL_TRIANGLES, pencilShaft.nIndices, GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 60);

    // Draws the cone (pencil - tip)
    glBindVertexArray(pencilTip.vao);
    myShader.setInt("useTexture", 1);
    translation = glm::translate(translation, glm::vec3(-1.18f, 0.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    // glDrawElements(GL_TRIANGLES, pencilTip.nIndices, GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 60);

    // second cylinder (pencil - silver eraser holder)
    glBindVertexArray(pencilEraserHolder.vao);
    myShader.setInt("useTexture", 0);
    translation = glm::translate(translation, glm::vec3(2.33f, 0.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    // glDrawElements(GL_TRIANGLES, pencilEraserHolder.nIndices, GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 60);

    // third cylinder (pencil - eraser)
    glBindVertexArray(pencilEraser.vao);
    myShader.setFloat3("objectColor", 0.8f, 0.3f, 0.3f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(0.25f, 0.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    // glDrawElements(GL_TRIANGLES, pencilEraser.nIndices, GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 60);

    // second cone ( pencil - graphite)
    glBindVertexArray(pencilGraphite.vao);
    myShader.setFloat3("objectColor", 0.3f, 0.3f, 0.3f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(-2.78f, 0.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    // glDrawElements(GL_TRIANGLES, pencilGraphite.nIndices, GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 60);

    // draws the eraser top
    glBindVertexArray(pencilEraserTop.vao);
    myShader.setFloat3("objectColor", 0.8f, 0.3f, 0.3f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(2.88f, 0.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    // glDrawElements(GL_TRIANGLES, pencilEraserTop.nIndices, GL_UNSIGNED_SHORT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 30);

    // draw book bottom cover
    glBindVertexArray(bookBottomCover.vao);
    myShader.setFloat3("objectColor", 0.45f, 0.45f, 0.45f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(2.0f, -0.119f, 0.0f));
    rotation = glm::rotate(glm::radians(30.0f), glm::vec3(0.0, 1.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //draw book spine
    glBindVertexArray(bookSpine.vao);
    myShader.setFloat3("objectColor", 0.45f, 0.45f, 0.45f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(-1.12585f, 0.02f, 0.65f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw book pages
    glBindVertexArray(bookPages.vao);
    myShader.setFloat3("objectColor", 0.97f, 0.97f, 0.97f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(1.12585f, 0.0f, -0.65f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw book top cover
    glBindVertexArray(bookTopCover.vao);
    myShader.setFloat3("objectColor", 0.35f, 0.35f, 0.05f);
    myShader.setInt("useTexture", 3);
    translation = glm::translate(translation, glm::vec3(0.0f, 0.6f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw orange
    glBindVertexArray(orange.vao);
    myShader.setFloat3("objectColor", 0.8f, 0.3f, 0.0f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(-4.0f, -0.05f, -2.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 600);

    // draw mug outer wall
    glBindVertexArray(mugOuterWall.vao);
    myShader.setFloat3("objectColor", 1.0f, 0.5f, 0.0f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(-2.0f, 0.13f, 3.5f));
    rotation = glm::rotate(glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 60);

    // draw mug base
    glBindVertexArray(mugBase.vao);
    myShader.setFloat3("objectColor", 0.6f, 0.3f, 0.0f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(0.0f, -0.7f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 30);

    // draw mug coffee
    glBindVertexArray(mugCoffee.vao);
    myShader.setFloat3("objectColor", 0.15f, 0.1f, 0.0f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(0.0f, 1.2f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 30);

    // draw mug handle
    glBindVertexArray(mugHandle.vao);
    myShader.setFloat3("objectColor", 0.2f, 0.1f, 0.0f);
    myShader.setInt("useTexture", 99);
    translation = glm::translate(translation, glm::vec3(-1.0f, -0.4f, 0.0f));
    rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translation * rotation * scale;
    myShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 600);


    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera speed
    float cameraSpeed = speedVariable * deltaTime;

    // W, A, S, and D keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // Q and E keys
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;

    // P key
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {

        if (projectionToggle == true)
            projectionToggle = false;
        else
            projectionToggle = true;
    }
}


bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // using GLFW 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // using GLFW 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // using OpenGL core

    // additional configuration if using MacOS
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create GLFW window
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window (*window == NULL)." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    // enable mouse as input
    glfwSetCursorPosCallback(*window, mouse_callback);
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(*window, scroll_callback);

    // display GPU OpenGL version
    std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    return true;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (initialMouse)
    {
        lastX = xpos;
        lastY = ypos;
        initialMouse = false;
    }

    // calculate mouse movement
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    // set mouse sensitivity
    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // set new pitch and yaw
    yaw += xoffset;
    pitch += yoffset;

    // limit maximum and minimum pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // update cameraFront
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // adjust speed of camera movement
    speedVariable -= (float)yoffset;

    // set minimum and maximum speed
    if (speedVariable < 0.5f)
        speedVariable = 0.5f;
    if (speedVariable > 20.0f)
        speedVariable = 20.0f;
}

void createTexture(GLenum textureUnit, GLuint& texture, const char* texturePath, GLenum textureWrapping)
{
    glGenTextures(1, &texture);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture, and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void destroyTexture(GLuint texture)
{
    glGenTextures(1, &texture);
}











