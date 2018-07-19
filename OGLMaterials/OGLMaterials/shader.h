#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// ����ID
	unsigned int m_ID;

	// ��������ȡ��������ɫ��
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// ʹ��/�������
	void useShaderProgram();

	// uniform���ߺ���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setVec3(const std::string &name, glm::vec3  value) const;
	void setVec3(const std::string &name,float valueA, float valueB, float valueC) const;

	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, glm::mat4 mat) const;
	
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif