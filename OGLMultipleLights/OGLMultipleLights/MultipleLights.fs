
#version 330 core

in vec3 Normal;		// 向量 由VertexShader传入
in vec3 FragPos;	// 顶点的世界坐标
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

// ------------------------------------------------------------------
//  材质定义 结构体
struct Material {
	sampler2D diffuse;// 漫反射光照
    sampler2D specular;	// 镜面光照
    float shininess;	// 反光度
}; 

uniform Material material;



// ------------------------------------------------------------------

// 平行光
struct DirLight {
    vec3 direction;	// 光照方向

	vec3 ambient;	// 环境光照
	vec3 diffuse;		// 漫反射光照
	vec3 specular;	// 镜面光照
};  
uniform DirLight dirLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);

    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
	//return vec3(0.3, 0.3, 0.3);
}

// ---------------------------------

// 点光源
struct PointLight {
	vec3 pos;	// 位置

	float constant; // 常数项
	float linear;		// 线性一次项
	float quadratic; // 二次项

	vec3 ambient;	// 环境光照
	vec3 diffuse;		// 漫反射光照
	vec3 specular;	// 镜面光照
};


#define POINT_LIGHTS 4
uniform PointLight pointLights[POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - fragPos);

	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);

	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// 衰减
	float distance = length(light.pos - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// 合并结果
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}



// 聚光光源
struct SpotLight {
	vec3 pos;	// 光源位置

	vec3 ambient;	// 环境光照
	vec3 diffuse;		// 漫反射光照
	vec3 specular;	// 镜面光照

	float constant; // 常数项
	float linear;		// 线性一次项
	float quadratic; // 二次项

	vec3  direction;	// 方向
	float cutOff;		// 聚光半径
	float outerCutOff;		// 外半径 用于模糊
};

#define SPOT_LIGHTS 6
uniform SpotLight spotLights[SPOT_LIGHTS];

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - fragPos);

	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	// 衰减
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
	// 属性
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// 第一阶段：定向光照
	vec3 result;
	result = CalcDirLight(dirLight, norm, viewDir);

	// 第二阶段：点光源
	for (int i = 0; i < POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

	// 第三阶段：聚光
	for(int i = 0; i < SPOT_LIGHTS; i++)
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}











//
//void main()
//{    
//    // 环境光
//	//													从纹理中采样片段的环境光颜色值
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//
//    // 漫反射 
//    vec3 norm = normalize(Normal);	// 标准化为单位向量
//    vec3 lightDir = normalize(light.pos - FragPos);	// 光照方向 片段指向光源
//    float diff = max(dot(norm, lightDir), 0.50);	// 物体表面法向量与光照向量的点乘
//
//	//													从纹理中采样片段的漫反射颜色值
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//
//    // 镜面光
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);  
//    float spec = pow(max(dot(viewDir, reflectDir), 0.50), material.shininess);
//    //vec3 specular = light.specular * (spec * material.specular);  
//	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));  
//
//	// 衰减计算
//	float distance    = length(light.pos - FragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * distance + 
//                light.quadratic * (distance * distance));
//
//	// 光源至片段的向量与光源的方向向量夹角的点乘
//	// dot返回一个余弦值 角度越小余弦值越大
//	float theta = dot(lightDir, normalize(-light.direction));
//
//	// 内外半径的夹角  此范围内则为柔光处理
//	float epsilon   = light.cutOff - light.outerCutOff;
//	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
//
//	vec4 result;
//
//
//	// 没有平滑边缘的效果
//	////  在光照范围内
//	//if (theta > light.cutOff)	// 两个余弦值比较
//	//{
//	//	// 执行光照计算
//	//	result = vec4(ambient * attenuation + diffuse  * attenuation  + specular  * attenuation , 1.0);
//	//}
//	//else  // 光照范围外，使用环境光，让场景在聚光之外时不至于完全黑暗
//	//{
//	//	result = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
//	//}
//
//	// 平滑的边缘的效果
//	result = vec4(ambient * attenuation + diffuse  * attenuation  * intensity+ specular  * attenuation * intensity, 1.0); 
//
//	FragColor = result;
//}