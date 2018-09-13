#version 330 core  // 版本声明
out vec4 FragColor; // 输出绘制

in vec3 ourColor; // 从vs中传进 在此未使用
in vec2 TexCoord; // 从vs中传进

uniform sampler2D ourTexture;  // 纹理采样器
uniform sampler2D ourTexture2;  // 纹理采样器

void main()
{
	// 通过纹理坐标与采样器，获取纹理对象中的纹素，最终进行绘制
	//FragColor = texture(ourTexture, TexCoord);

	// 两个纹理进行混合
	FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.5);
}