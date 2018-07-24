
#version 330 core

in vec3 Normal;		// ���� ��VertexShader����
in vec3 FragPos;	// �������������

in vec2 TexCoords;

out vec4 FragColor;

//  ��Դ���� �ṹ��
struct Light {
    vec3 pos;	// ��Դλ��

    vec3 ambient;	// ��������
    vec3 diffuse;		// ���������
    vec3 specular;	// �������
};

//  ���ʶ��� �ṹ��
struct Material {
    //vec3 ambient;	// ��������
	
	sampler2D diffuse;
    //vec3 diffuse;		// ���������

    vec3 specular;	// �������

    float shininess;	// �����
}; 

uniform Light light;
uniform Material material;

uniform vec3 viewPos;


void main()
{    
    // ������
	//													�������в���Ƭ�εĻ�������ɫֵ
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // ������ 
    vec3 norm = normalize(Normal);	// ��׼��Ϊ��λ����
    vec3 lightDir = normalize(light.pos - FragPos);	// ���շ��� Ƭ��ָ���Դ
    float diff = max(dot(norm, lightDir), 0.0);	// ������淨��������������ĵ��

	//													�������в���Ƭ�ε���������ɫֵ
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // �����
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}