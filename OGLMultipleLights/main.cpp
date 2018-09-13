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


void framebuffer_size_callback(GLFWwindow* pWnd, int width, int height);
void mouse_callback(GLFWwindow* pWnd, double xpos, double ypos);
void scroll_callback(GLFWwindow* pWnd, double xoffset, double yoffset);
void processInput(GLFWwindow *pWnd);
unsigned int loadTexture(char const * path);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera 相机
Camera camera(glm::vec3(0.0f, 0.0f, 2.5f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 0.0f, 3.5f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	glfwSetInputMode(pWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);


	Shader cubeShader("MultipleLights.vs", "MultipleLights.fs");
	Shader lampShader("lamp.vs", "lamp.fs");

	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 cubePos[] = { // 立方体位置
		glm::vec3(0.0f,  0.0f,  0.0f),			glm::vec3(2.0f,  5.0f, -5.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),		glm::vec3(-3.8f, -2.0f, -6.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -0.1f),			glm::vec3(-1.3f,  1.0f, 0.0f)
	};

	glm::vec3 pointLightPos[] = { // 点光源位置
		glm::vec3(0.7f,  0.7f,  2.0f),
		glm::vec3(2.3f, -1.3f, 4.0f),
		glm::vec3(-3.0f,  2.0f, -6.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 spotLightPos[] = { // 点光源位置
		glm::vec3(0.7f,  0.9f,  2.5f),
		glm::vec3(2.3f, -3.3f, 1.2f),
		glm::vec3(-2.5f,  3.0f, -4.0f),
		glm::vec3(-2.3f,  -3.4f, -3.0f)
	};


	// 立方体
	unsigned int cubeVAO, cubeVBO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// 光源
	unsigned int lampVAO, lampVBO;
	glGenVertexArrays(1, &lampVAO);
	glGenBuffers(1, &lampVBO);

	glBindVertexArray(lampVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// 纹理
	unsigned int diffuseMap = loadTexture("../res/container.png");	
	unsigned int specularMap = loadTexture("../res/container_specular.png");

	cubeShader.useShaderProgram();
	cubeShader.setInt("material.diffuse", 0);
	cubeShader.setInt("material.specular", 1);	

	while (!glfwWindowShouldClose(pWnd))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(pWnd);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ---物体1
		cubeShader.useShaderProgram();
		cubeShader.setVec3("light.pos", lightPos);
		cubeShader.setVec3("viewPos", camera.Position);

		// 光源定义
		// 平行光
		cubeShader.setVec3("dirLight.direction", camera.Front);

		cubeShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		cubeShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		// 点光源
		// 1
		cubeShader.setVec3("pointLights[0].pos", pointLightPos[0]);

		cubeShader.setFloat("pointLights[0].constant", 1.0f);
		cubeShader.setFloat("pointLights[0].linear", 0.1f);
		cubeShader.setFloat("pointLights[0].quadratic", 0.032f);

		cubeShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);

		// 2
		cubeShader.setVec3("pointLights[1].pos", pointLightPos[1]);

		cubeShader.setFloat("pointLights[1].constant", 1.0f);
		cubeShader.setFloat("pointLights[1].linear", 0.1f);
		cubeShader.setFloat("pointLights[1].quadratic", 0.032f);

		cubeShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);

		// 3
		cubeShader.setVec3("pointLights[2].pos", pointLightPos[2]);

		cubeShader.setFloat("pointLights[2].constant", 1.0f);
		cubeShader.setFloat("pointLights[2].linear", 0.1f);
		cubeShader.setFloat("pointLights[2].quadratic", 0.032f);

		cubeShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);

		// 4
		cubeShader.setVec3("pointLights[3].pos", pointLightPos[3]);

		cubeShader.setFloat("pointLights[3].constant", 1.0f);
		cubeShader.setFloat("pointLights[3].linear", 0.1f);
		cubeShader.setFloat("pointLights[3].quadratic", 0.032f);

		cubeShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);

		// 聚光光源 射灯
		// 1
		cubeShader.setVec3("spotLights[0].pos", spotLightPos[0]);

		cubeShader.setVec3("spotLights[0].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("spotLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);

		cubeShader.setFloat("spotLights[0].constant", 1.0f);
		cubeShader.setFloat("spotLights[0].linear", 0.1f);
		cubeShader.setFloat("spotLights[0].quadratic", 0.032f);

		cubeShader.setVec3("spotLights[0].direction", camera.Front);
		cubeShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(17.5f)));

		// 2
		cubeShader.setVec3("spotLights[1].pos", spotLightPos[1]);

		cubeShader.setVec3("spotLights[1].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("spotLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("spotLights[1].specular", 1.0f, 1.0f, 1.0f);

		cubeShader.setFloat("spotLights[1].constant", 1.0f);
		cubeShader.setFloat("spotLights[1].linear", 0.1f);
		cubeShader.setFloat("spotLights[1].quadratic", 0.032f);

		cubeShader.setVec3("spotLights[1].direction", camera.Front);
		cubeShader.setFloat("spotLights[1].cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(17.5f)));
		
		// 3
		cubeShader.setVec3("spotLights[2].pos", spotLightPos[2]);

		cubeShader.setVec3("spotLights[2].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("spotLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("spotLights[2].specular", 1.0f, 1.0f, 1.0f);

		cubeShader.setFloat("spotLights[2].constant", 1.0f);
		cubeShader.setFloat("spotLights[2].linear", 0.1f);
		cubeShader.setFloat("spotLights[2].quadratic", 0.032f);

		cubeShader.setVec3("spotLights[2].direction", camera.Front);
		cubeShader.setFloat("spotLights[2].cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLights[2].outerCutOff", glm::cos(glm::radians(17.5f)));

		// 4
		cubeShader.setVec3("spotLights[3].pos", spotLightPos[3]);

		cubeShader.setVec3("spotLights[3].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("spotLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("spotLights[3].specular", 1.0f, 1.0f, 1.0f);

		cubeShader.setFloat("spotLights[3].constant", 1.0f);
		cubeShader.setFloat("spotLights[3].linear", 0.1f);
		cubeShader.setFloat("spotLights[3].quadratic", 0.032f);

		cubeShader.setVec3("spotLights[3].direction", camera.Front);
		cubeShader.setFloat("spotLights[3].cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLights[3].outerCutOff", glm::cos(glm::radians(17.5f)));

		// 5
		cubeShader.setVec3("spotLights[4].pos", spotLightPos[4]);

		cubeShader.setVec3("spotLights[4].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("spotLights[4].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("spotLights[4].specular", 1.0f, 1.0f, 1.0f);

		cubeShader.setFloat("spotLights[4].constant", 1.0f);
		cubeShader.setFloat("spotLights[4].linear", 0.1f);
		cubeShader.setFloat("spotLights[4].quadratic", 0.032f);

		cubeShader.setVec3("spotLights[4].direction", camera.Front);
		cubeShader.setFloat("spotLights[4].cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLights[4].outerCutOff", glm::cos(glm::radians(17.5f)));
		
		// 6
		cubeShader.setVec3("spotLights[5].pos", spotLightPos[5]);

		cubeShader.setVec3("spotLights[5].ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("spotLights[5].diffuse", 0.8f, 0.8f, 0.8f);
		cubeShader.setVec3("spotLights[5].specular", 1.0f, 1.0f, 1.0f);

		cubeShader.setFloat("spotLights[5].constant", 1.0f);
		cubeShader.setFloat("spotLights[5].linear", 0.1f);
		cubeShader.setFloat("spotLights[5].quadratic", 0.032f);

		cubeShader.setVec3("spotLights[5].direction", camera.Front);
		cubeShader.setFloat("spotLights[5].cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLights[5].outerCutOff", glm::cos(glm::radians(17.5f)));

		// 灯照范围 半径
		// radians 弧度 12.5度转为弧度, 然后cos求值 约0.976
		//float dTemp = glm::cos(glm::radians(12.5f));
		
		// 材质定义
		cubeShader.setFloat("material.shininess", 64.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);

		glm::mat4 model = glm::mat4();
		cubeShader.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(cubeVAO);			// VAO
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

		// 绘制十个立方体
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePos[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// ---灯泡
		lampShader.useShaderProgram();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // 缩小立方体
		lampShader.setMat4("model", model);		  // 小立方体代表灯泡

		glBindVertexArray(lampVAO);	// VAO 切换
		glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPos[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(pWnd);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteBuffers(1, &lampVBO);

	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow *pWnd)
{
	if (glfwGetKey(pWnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(pWnd, true);

	if (glfwGetKey(pWnd, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}


void framebuffer_size_callback(GLFWwindow* pWnd, int width, int height)
{
	glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* pWnd, double xpos, double ypos)
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


void scroll_callback(GLFWwindow* pWnd, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

