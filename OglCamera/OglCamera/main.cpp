#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <glad/glad.c>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "shader.h"

using namespace std;
#pragma comment (lib, "glfw3.lib") 

void framebuffer_size_callback(GLFWwindow* pWnd, int width, int height);
void processInput(GLFWwindow *pWnd);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 相机参数
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  // 位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 前方向向量
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // 上方向向量

bool firstMouse = true;

// 俯仰角(Pitch)、偏航角(Yaw)和滚转角(Roll)
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

//视野(fov Field of View)可以看到场景中的范围
float fov = 45.0f;


float deltaTime = 0.0f;	//当前帧与最后帧的时间差
float lastFrame = 0.0f;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWnd = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Camera", NULL, NULL);
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);	// 开启深度测试

	Shader ourShader("camera.vs", "camera.fs");

	float vertices[] = { // 模型位置坐标3　纹理坐标3
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// 为每个立方体定义一个位移向量来指定它在世界空间的位置
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute  -> layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute  -> layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 图像1
	unsigned int texture1;
	glGenTextures(1, &texture1); // 创建一个空的纹理对象
	glBindTexture(GL_TEXTURE_2D, texture1);// 绑定后，接下来对纹理的操作均在此ID上进行

	// set the texture wrapping parameters　设置纹理环绕方式：均为重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters　过滤方式：缩小放大均为线性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	// 将纹理图片数据加载到内存中
	//stbi_set_flip_vertically_on_load(true); // 翻转图像
	unsigned char *data = stbi_load("../res/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// 用纹理图片数据生成一个2D纹理，即将空的纹理对象填入数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// 图像2
	unsigned int texture2;
	glGenTextures(1, &texture2); // 创建一个空的纹理对象
	glBindTexture(GL_TEXTURE_2D, texture2);// 绑定后，接下来对纹理的操作均在此ID上进行

	 // set the texture wrapping parameters　设置纹理环绕方式：均为重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters　过滤方式：缩小放大均为线性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("../res/Sloth.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data); // 释放图片资源

	// 纹理分别对应不同的采样器
	ourShader.useShaderProgram();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	while (!glfwWindowShouldClose(pWnd))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame; // 计算帧的时间差
		lastFrame = currentFrame;

		processInput(pWnd); // 输入处理

		// 清理窗口 及 深度缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.useShaderProgram();

		// 定义并将 透视投影矩阵 传至Shader中
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation 相机/视图 移动
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(pWnd);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

// WSAD 控制移动 ESC退出
void processInput(GLFWwindow *pWnd)
{
	if (glfwGetKey(pWnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(pWnd, true);

	float cameraSpeed = 0.5 * deltaTime;
	if (glfwGetKey(pWnd, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(pWnd, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(pWnd, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(pWnd, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// 窗口大小
void framebuffer_size_callback(GLFWwindow* pWnd, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 鼠标移动控制
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

