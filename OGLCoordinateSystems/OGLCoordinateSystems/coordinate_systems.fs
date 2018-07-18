#version 330 core  // 版本声明
out vec4 FragColor; // 输出绘制

in vec2 TexCoord; // 从vs中传进

uniform sampler2D texture1;  // 纹理采样器
uniform sampler2D texture2;  // 纹理采样器

void main()
{
	// 两个纹理进行混合
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
}