#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	// 加载读取Shader文件
	std::string strVertexCode;
	std::string strFragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		strVertexCode = vShaderStream.str();
		strFragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = strVertexCode.c_str();
	const char * fShaderCode = strFragmentCode.c_str();

	// 2. compile shaders 编译
	unsigned int nVertex, nFragment;
	int nSuccess = 0;
	char infoLog[512];

	// vertex shader
	nVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(nVertex, 1, &vShaderCode, NULL);
	glCompileShader(nVertex);
	checkCompileErrors(nVertex, "VERTEX");

	// fragment Shader
	nFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nFragment, 1, &fShaderCode, NULL);
	glCompileShader(nFragment);
	checkCompileErrors(nFragment, "FRAGMENT");

	// shader Program
	m_ID = glCreateProgram();
	glAttachShader(m_ID, nVertex);
	glAttachShader(m_ID, nFragment);
	glLinkProgram(m_ID);
	checkCompileErrors(m_ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(nVertex);
	glDeleteShader(nFragment);
}


// activate the shader
// ------------------------------------------------------------------------
void Shader::useShaderProgram()
{
	glUseProgram(m_ID);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, glm::vec3  value) const
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
}

// ------------------------------------------------------------------------

void Shader::setVec3(const std::string &name, float valueA, float valueB, float valueC) const
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), valueA, valueB, valueC);
}


// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int nSuccess = 0;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &nSuccess);
		if (!nSuccess)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
				<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &nSuccess);
		if (!nSuccess)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
				<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}