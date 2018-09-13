#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <glad/glad.c>

#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "shader.h"

#pragma comment (lib, "glfw3.lib") 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera 相机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.9f, 0.6f, 2.3f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	
	glEnable(GL_DEPTH_TEST);

	Shader cubeShader("cube.vs", "cube.fs");

	Shader lampShader("lamp.vs", "lamp.fs");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 		0.5f, -0.5f, -0.5f,		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,		-0.5f,  0.5f, -0.5f,		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,		0.5f, -0.5f,  0.5f,		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,		-0.5f,  0.5f,  0.5f,		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,		-0.5f,  0.5f, -0.5f,		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,		-0.5f, -0.5f,  0.5f,		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,		0.5f,  0.5f, -0.5f,		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,		0.5f, -0.5f,  0.5f,		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,		0.5f, -0.5f, -0.5f,		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,		-0.5f, -0.5f,  0.5f,		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,		0.5f,  0.5f, -0.5f,		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,		-0.5f,  0.5f,  0.5f,		-0.5f,  0.5f, -0.5f,
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 光源
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// 用同样的顶点数据vertices

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	lampShader.useShaderProgram();
	lampShader.setVec3("lampColor", 0.2f, 0.4f, 0.8f);	// 灯光颜色

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeShader.useShaderProgram();
		// ---物体材质颜色与灯光颜色进行相乘 得到物体最终颜色
		cubeShader.setVec3("objectColor", 0.3f, 0.5f, 0.31f);	// 物体颜色
		cubeShader.setVec3("lightColor", 0.2f, 0.4f, 0.8f);			// 灯光颜色

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
				(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);

		// world transformation
		glm::mat4 model;
		cubeShader.setMat4("model", model);

		// render the cube
		glBindVertexArray(cubeVAO); // VAO
		glDrawArrays(GL_TRIANGLES, 0, 36);	// 绘制经灯光照射的立方体


		// ---绘制一个和物体材质一样颜色的立方体 进行对比
		cubeShader.setVec3("objectColor", 0.3f, 0.5f, 0.31f);	// 物体颜色
		cubeShader.setVec3("lightColor", 0.3f, 0.5f, 0.31f);  // 灯光颜色 与物体一致

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.3f, 0.8f, 1.3f));
		cubeShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);  // 绘制原色的立方体
		
		// ---灯泡
		lampShader.useShaderProgram();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // 缩小立方体
		lampShader.setMat4("model", model);		// 小立方体代表灯泡

		glBindVertexArray(lightVAO);	// VAO 切换
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

