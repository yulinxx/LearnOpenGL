#version 330 core  // 版本声明

// layout location 通过C++中的 glVertexAttribPointer 进行传递
layout (location = 0) in vec3 aPos;  // 顶点位置
layout (location = 1) in vec3 aColor; // 颜色
layout (location = 2) in vec2 aTexCoord; // 纹理坐标
out vec3 ourColor; // out 输出至fragement中
out vec2 TexCoord;

void main()
{
	// gl_Position	vec4	输出属性-变换后的顶点的位置，用于后面的固定的裁剪等操作。
	// 所有的顶点着色器都必须写这个值。
    gl_Position = vec4(aPos, 1.0); 

	// 将layout location的值向fragement中输出
    ourColor = aColor;	
    TexCoord = aTexCoord;
}