#version 330 core  // �汾����

// layout location ͨ��C++�е� glVertexAttribPointer ���д���
layout (location = 0) in vec3 aPos;  // ����λ��
layout (location = 1) in vec3 aColor; // ��ɫ
layout (location = 2) in vec2 aTexCoord; // ��������
out vec3 ourColor; // out �����fragement��
out vec2 TexCoord;

void main()
{
	// gl_Position	vec4	�������-�任��Ķ����λ�ã����ں���Ĺ̶��Ĳü��Ȳ�����
	// ���еĶ�����ɫ��������д���ֵ��
    gl_Position = vec4(aPos, 1.0); 

	// ��layout location��ֵ��fragement�����
    ourColor = aColor;	
    TexCoord = aTexCoord;
}