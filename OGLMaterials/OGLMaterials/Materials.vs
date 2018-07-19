#version 330 core

layout (location = 0) in vec3 aPos;		// 顶点
layout (location = 1) in vec3 aNormal;	// 顶点法向量

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    // 注意乘法要从右向左读
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	// 世界空间中进行所有的光照计算
    FragPos = vec3(model * vec4(aPos, 1.0)); // 转换成世界空间坐标

	Normal = aNormal;
}
