/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: main.
 */


#include <Windows.h>
// Include standard headers
#include <cstdio>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#define GLM_ENABLE_EXPERIMENTAL

// Local / project headers
#include <iostream>

#include "Camera.hpp"
#include "Graphic.h"
#include "Shader.h"

#include "AnimationModel.h"
#include "imgui_impl_glfw.h"

#include "Imgui/imgui.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
#include "PhysicsSimulation.h"
#include "SimpleBox.h"

//////////////////////////////////////////////////////////////////////


int windowWidth = 1024;
int windowHeight = 768;
int displayIndex = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
bool camLock = true;
Graphic* graphic;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        camLock = !camLock;
}
int deltaTime(int previous, int offset)
{
    return (clock() - previous) + offset;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    GLFWwindow* window;


    // Setting up OpenGL properties
    glfwWindowHint(GLFW_SAMPLES, 1); // change for anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(windowWidth, windowHeight, // window dimensions
                              "Sample 1 - Simple scene (Quad) with Scene Class", // window title
                              nullptr, // which monitor (if full-screen mode)
                              nullptr); // if sharing context with another window
    if (window == nullptr)
    {
        fprintf(stderr,
                "Failed to open GLFW window. Check if your GPU is 4.0 compatible.\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // OpenGL resource model - "glfwCreateWindow" creates the necessary data storage for the OpenGL
    // context but does NOT make the created context "current". We MUST make it current with the following
    // call
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = static_cast<GLboolean>(true); // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, &key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();


    graphic = new Graphic();
    graphic->window = window;

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;
    int pTime = clock();
    int offset = 0;
    int tickRate = 120;

    do
    {
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;

        if(timeDiff >= 1.0 / 30.0)
        {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle = "Animation - " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, newTitle.c_str());
            prevTime = crntTime;
            counter = 0;
        }
        
        int delta = deltaTime(pTime, offset);
        pTime = clock();
        float dt = static_cast<float>(delta) / 1000.f;

        offset = delta % (1000 / tickRate);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("CS460 Project4");

        if (ImGui::TreeNode("Box"))
        {
            ImGui::SliderFloat3("BoxPos", &graphic->simpleBox->pos.x, -10.f, 10.f);
            ImGui::SliderFloat3("BoxScale", &graphic->simpleBox->scale.x, 0.f, 10.f);
            ImGui::TreePop();
        }
        
        if(ImGui::TreeNode("AnchorPositions"))
        {
            ImGui::SliderFloat3("frontRightPos", &graphic->frontRight->pos.x, -20.f, 20.f);
            ImGui::SliderFloat3("backRightPos", &graphic->backRight->pos.x, -20.f, 20.f);
            ImGui::SliderFloat3("frontLeftPos", &graphic->frontLeft->pos.x, -20.f, 20.f);
            ImGui::SliderFloat3("backLeftPos", &graphic->backLeft->pos.x, -20.f, 20.f);
        	ImGui::TreePop();
        }

        if(ImGui::Button("Reset"))
        {
            graphic->ReInitSimulation();
        }
        

    	ImGui::Text("Space  : Toggle cam moving");
        ImGui::Text("W/A/S/D: Cam movement");
        ImGui::Text("Mouse  : Cam rotation");

        ImGui::End();


        float currentFrame = glfwGetTime();
        graphic->deltaTime = currentFrame - graphic->lastFrame;
        graphic->lastFrame = currentFrame;

        graphic->ProcessInput();
        graphic->Draw(dt);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();



        

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    delete graphic;

    return 0;
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if(!camLock)
    {
	    const float xpos = static_cast<float>(xposIn);
	    const float ypos = static_cast<float>(yposIn);

	    if (firstMouse)
	    {
	        lastX = xpos;
	        lastY = ypos;
	        firstMouse = false;
	    }

	    const float xoffset = xpos - lastX;
	    const float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        graphic->cam->ProcessMouseMovement(xoffset, yoffset);

    	lastX = xpos;
	    lastY = ypos;
    }
    else
    {
        lastX = static_cast<float>(xposIn);
        lastY = static_cast<float>(yposIn);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= static_cast<float>(yoffset);
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
