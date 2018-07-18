#version 330 core  // �汾����
out vec4 FragColor; // �������

in vec3 ourColor; // ��vs�д��� �ڴ�δʹ��
in vec2 TexCoord; // ��vs�д���

uniform sampler2D ourTexture;  // ���������
uniform sampler2D ourTexture2;  // ���������

void main()
{
	// ͨ���������������������ȡ��������е����أ����ս��л���
	//FragColor = texture(ourTexture, TexCoord);

	// ����������л��
	FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.5);
}