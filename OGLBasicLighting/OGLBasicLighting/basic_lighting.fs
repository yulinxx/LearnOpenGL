#version 330 core


uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;
uniform vec3 lightPos;		// ��Դ��λ��

uniform float specularStrength; 	// ����߹�ǿ�ȿ��Ʋ���

in vec3 Normal;		// ���� ��VertexShader����
in vec3 FragPos;	// �������������

out vec4 FragColor;

void main()
{

	vec3 norm = normalize(Normal);	// normalize��׼��Ϊ��λ����
	vec3 lightDir = normalize(lightPos - FragPos); // ���շ��� Ƭ��ָ���Դ

	// ---- 
	// �����Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��
	// ��������֮��ĽǶ�ԽС������������ͻ�Խ��
	// ����90�Ⱥ�,��Ϊ��,������max����Ϊ 0�͵�˽�� ���ߵ����ֵ ȷ����С��0
	float diff = max(dot(norm, lightDir), 0.0);	// ���߶�Ƭ����ɫ��Ӱ��
	vec3 diffuse = diff * lightColor;	// ������
	
	// ---- 
	// �����ɫ����һ����������, ���ղ������,��Ϊ������������Ӱ��
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;	// ������
	
	// ---- 
	// �� lightDir ���շ��������ȡ��, 
	// ��Ϊreflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�������
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 viewDir = normalize(viewPos - FragPos); // �۲ⷨ�� Ƭ��ָ��۲��

	// �ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ�
	//���32�Ǹ߹�ķ����(Shininess)��һ������ķ����Խ�ߣ�
	// ����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС��
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;	// ���淴��

	// ----  ���� ��(������ + ������ + �����) * ������ɫ
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}