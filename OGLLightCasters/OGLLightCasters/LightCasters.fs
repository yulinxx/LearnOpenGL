
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

	float constant; // ������
    float linear;		// ����һ����
    float quadratic; // ������

    vec3  direction;	// ����
    float cutOff;		// �۹�뾶
	float outerCutOff;		// ��뾶 ����ģ��
};

//  ���ʶ��� �ṹ��
struct Material {
	sampler2D diffuse;// ���������
    sampler2D specular;	// �������
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
    float diff = max(dot(norm, lightDir), 0.50);	// ������淨��������������ĵ��

	//													�������в���Ƭ�ε���������ɫֵ
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // �����
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.50), material.shininess);
    //vec3 specular = light.specular * (spec * material.specular);  
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));  

	// ˥������
	float distance    = length(light.pos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));

	// dot����һ������ֵ �Ƕ�ԽС����ֵԽ��
	float theta = dot(lightDir, normalize(-light.direction));

	// ����뾶�ļн�  �˷�Χ����Ϊ��⴦��
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    

	vec4 result;


	// û��ƽ����Ե��Ч��
	////  �ڹ��շ�Χ��
	//if (theta > light.cutOff)	// ��������ֵ�Ƚ�
	//{
	//	// ִ�й��ռ���
	//	result = vec4(ambient * attenuation + diffuse  * attenuation  + specular  * attenuation , 1.0);
	//}
	//else  // ���շ�Χ�⣬ʹ�û����⣬�ó����ھ۹�֮��ʱ��������ȫ�ڰ�
	//{
	//	result = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
	//}

	// ƽ���ı�Ե��Ч��
	result = vec4(ambient * attenuation + diffuse  * attenuation  * intensity+ specular  * attenuation * intensity, 1.0); 

	FragColor = result;
}