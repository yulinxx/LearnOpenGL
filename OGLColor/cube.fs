#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	// 物体材质颜色与灯光颜色进行相乘 得到物体最终颜色
    FragColor = vec4(lightColor * objectColor, 1.0);
}