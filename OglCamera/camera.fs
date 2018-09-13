#version 330 core  // �汾����
out vec4 FragColor; // �������

in vec2 TexCoord; // ��vs�д���

uniform sampler2D texture1;  // ���������
uniform sampler2D texture2;  // ���������

void main()
{
	// ����������л��
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
}