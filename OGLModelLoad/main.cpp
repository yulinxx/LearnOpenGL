#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <glad/glad.c>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Camera.h"
#include "model.h"

#include <iostream>

#pragma comment (lib, "glfw3.lib") 


void framebuffer_size_callback(GLFWwindow* pWnd, int width, int height);
void mouse_callback(GLFWwindow* pWnd, double xpos, double ypos);
void scroll_callback(GLFWwindow* pWnd, double xoffset, double yoffset);
void processInput(GLFWwindow *pWnd);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float dLastX = SCR_WIDTH / 2.0f;
float dLastY = SCR_HEIGHT / 2.0f;
bool dFirstMouse = true;

// timing
float dDeltaTime = 0.0f;
float dLastFrame = 0.0f;


int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	// uncomment this statement to fix compilation on OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

	// glfw pWnd creation
	GLFWwindow* pWnd = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (pWnd == NULL)
	{
		std::cout << "Failed to create GLFW pWnd" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWnd);
	glfwSetFramebufferSizeCallback(pWnd, framebuffer_size_callback);
	glfwSetCursorPosCallback(pWnd, mouse_callback);
	glfwSetScrollCallback(pWnd, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(pWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// -------------------------
	Shader ourShader("model_loading.vs", "model_loading.fs");
	Shader lampShader("lamp.vs", "lamp.fs");

	// load models
	Model ourModel("../res/nanosuit/nanosuit.obj");


	glm::vec3 ptLightPos[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};


	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// -----------
	while (!glfwWindowShouldClose(pWnd))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		dDeltaTime = currentFrame - dLastFrame;
		dLastFrame = currentFrame;

		// input
		processInput(pWnd);

		// render
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.useShaderProgram();

		// view/projection transformations
		glm::mat4 matProj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 matView = camera.GetViewMatrix();
		ourShader.setMat4("projection", matProj);
		ourShader.setMat4("view", matView);

		// render the loaded model
		glm::mat4 matModel;
		matModel = glm::translate(matModel, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		matModel = glm::scale(matModel, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", matModel);
		//ourModel.Draw(ourShader);


		// Draw the lamps
		lampShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(lampShader.m_ID, "projection"), 1, GL_FALSE, glm::value_ptr(matProj));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.m_ID, "view"), 1, GL_FALSE, glm::value_ptr(matView));
		//for (GLuint i = 0; i < 2; i++)
		{
			//matModel = glm::mat4();
			//matModel = glm::translate(matModel, ptLightPos[i]);
			//matModel = glm::scale(matModel, glm::vec3(0.3f, 0.3f, 0.3f)); // Downscale lamp object (a bit too large)
			glUniformMatrix4fv(glGetUniformLocation(lampShader.m_ID, "model"), 1, GL_FALSE, glm::value_ptr(matModel));
			ourModel.Draw(lampShader);
		}


		glfwSwapBuffers(pWnd);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



void processInput(GLFWwindow *pWnd)
{
	if (glfwGetKey(pWnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(pWnd, true);

	if (glfwGetKey(pWnd, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, dDeltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, dDeltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, dDeltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, dDeltaTime);
}


void framebuffer_size_callback(GLFWwindow* pWnd, int width, int height)
{
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* pWnd, double xpos, double ypos)
{
	if (dFirstMouse)
	{
		dLastX = xpos;
		dLastY = ypos;
		dFirstMouse = false;
	}

	float xoffset = xpos - dLastX;
	float yoffset = dLastY - ypos; // reversed since y-coordinates go from bottom to top

	dLastX = xpos;
	dLastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* pWnd, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}