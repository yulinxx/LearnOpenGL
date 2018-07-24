#version 330 core

layout (location = 0) in vec3 aPos;		// ����
layout (location = 1) in vec3 aNormal;	// ���㷨����
layout (location = 2) in vec2 aTexCoords;	// ��������

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;   // fragment's position 

out vec3 Normal;	// ���� ����FragementShader
out vec2 TexCoords;


void main()
{
	// �˷����������
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	// ������ռ��н������еĹ��ռ���
    FragPos = vec3(model * vec4(aPos, 1.0)); // ת��������ռ�����
    Normal = aNormal;

	TexCoords = aTexCoords;	// ����Fragement Shader��
}