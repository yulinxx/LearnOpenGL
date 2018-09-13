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

// 0/1 在镜面贴图/漫反射贴图 间切换
#define USER_DIFFUSE_MAP  0

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

#if USER_DIFFUSE_MAP
	Shader cubeShader("DiffuseMap.vs", "DiffuseMap.fs");
#else
	Shader cubeShader("SpecularMap.vs", "SpecularMap.fs");
#endif

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


	unsigned int cubeVAO, cubeVBO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute 向量
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

	unsigned int diffuseMap = loadTexture("../res/container.png");	 // 纹理

#if USER_DIFFUSE_MAP
#else
	unsigned int specularMap = loadTexture("../res/container_specular.png");
#endif

	cubeShader.useShaderProgram();
	cubeShader.setInt("material.diffuse", 0);

#if USER_DIFFUSE_MAP
#else
	cubeShader.setInt("material.specular", 1);	
#endif

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
		cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// 材质定义
#if USER_DIFFUSE_MAP
#else
		cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
#endif

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

#if USER_DIFFUSE_MAP
#else
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
#endif

		glBindVertexArray(cubeVAO);			// VAO
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glDrawArrays(GL_TRIANGLES, 0, 36);	// 绘制经灯光照射的立方体

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
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
