#version 330 core

layout (location = 0) in vec3 aPos;		// 顶点
layout (location = 1) in vec3 aNormal;	// 顶点法向量
layout (location = 2) in vec2 aTexCoords;	// 纹理坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;   // fragment's position 

out vec3 Normal;	// 向量 传至FragementShader
out vec2 TexCoords;


void main()
{
	// 乘法从右向左读
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	// 在世界空间中进行所有的光照计算
    FragPos = vec3(model * vec4(aPos, 1.0)); // 转换成世界空间坐标
    Normal = aNormal;

	TexCoords = aTexCoords;	// 传至Fragement Shader中
}