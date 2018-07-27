
#version 330 core

in vec3 Normal;		// ���� ��VertexShader����
in vec3 FragPos;	// �������������
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

// ------------------------------------------------------------------
//  ���ʶ��� �ṹ��
struct Material {
	sampler2D diffuse;// ���������
    sampler2D specular;	// �������
    float shininess;	// �����
}; 

uniform Material material;



// ------------------------------------------------------------------

// ƽ�й�
struct DirLight {
    vec3 direction;	// ���շ���

	vec3 ambient;	// ��������
	vec3 diffuse;		// ���������
	vec3 specular;	// �������
};  
uniform DirLight dirLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);

    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // �ϲ����
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
	//return vec3(0.3, 0.3, 0.3);
}

// ---------------------------------

// ���Դ
struct PointLight {
	vec3 pos;	// λ��

	float constant; // ������
	float linear;		// ����һ����
	float quadratic; // ������

	vec3 ambient;	// ��������
	vec3 diffuse;		// ���������
	vec3 specular;	// �������
};


#define POINT_LIGHTS 4
uniform PointLight pointLights[POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - fragPos);

	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);

	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// ˥��
	float distance = length(light.pos - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// �ϲ����
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}



// �۹��Դ
struct SpotLight {
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

#define SPOT_LIGHTS 6
uniform SpotLight spotLights[SPOT_LIGHTS];

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - fragPos);

	// ������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	// ˥��
	float distance = length(light.pos - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));


	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


	return (ambient * attenuation + diffuse * attenuation * intensity
				+ specular * attenuation * intensity);
}


// ------------------------------------------------------------------


void main()
{
	// ����
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// ��һ�׶Σ��������
	vec3 result;
	result = CalcDirLight(dirLight, norm, viewDir);

	// �ڶ��׶Σ����Դ
	for (int i = 0; i < POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

	// �����׶Σ��۹�
	for(int i = 0; i < SPOT_LIGHTS; i++)
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}











//
//void main()
//{    
//    // ������
//	//													�������в���Ƭ�εĻ�������ɫֵ
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//
//    // ������ 
//    vec3 norm = normalize(Normal);	// ��׼��Ϊ��λ����
//    vec3 lightDir = normalize(light.pos - FragPos);	// ���շ��� Ƭ��ָ���Դ
//    float diff = max(dot(norm, lightDir), 0.50);	// ������淨��������������ĵ��
//
//	//													�������в���Ƭ�ε���������ɫֵ
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//
//    // �����
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);  
//    float spec = pow(max(dot(viewDir, reflectDir), 0.50), material.shininess);
//    //vec3 specular = light.specular * (spec * material.specular);  
//	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));  
//
//	// ˥������
//	float distance    = length(light.pos - FragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * distance + 
//                light.quadratic * (distance * distance));
//
//	// ��Դ��Ƭ�ε��������Դ�ķ��������нǵĵ��
//	// dot����һ������ֵ �Ƕ�ԽС����ֵԽ��
//	float theta = dot(lightDir, normalize(-light.direction));
//
//	// ����뾶�ļн�  �˷�Χ����Ϊ��⴦��
//	float epsilon   = light.cutOff - light.outerCutOff;
//	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
//
//	vec4 result;
//
//
//	// û��ƽ����Ե��Ч��
//	////  �ڹ��շ�Χ��
//	//if (theta > light.cutOff)	// ��������ֵ�Ƚ�
//	//{
//	//	// ִ�й��ռ���
//	//	result = vec4(ambient * attenuation + diffuse  * attenuation  + specular  * attenuation , 1.0);
//	//}
//	//else  // ���շ�Χ�⣬ʹ�û����⣬�ó����ھ۹�֮��ʱ��������ȫ�ڰ�
//	//{
//	//	result = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
//	//}
//
//	// ƽ���ı�Ե��Ч��
//	result = vec4(ambient * attenuation + diffuse  * attenuation  * intensity+ specular  * attenuation * intensity, 1.0); 
//
//	FragColor = result;
//}