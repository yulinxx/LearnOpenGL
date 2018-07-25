
#version 330 core

in vec3 Normal;		// 向量 由VertexShader传入
in vec3 FragPos;	// 顶点的世界坐标

in vec2 TexCoords;

out vec4 FragColor;

//  光源定义 结构体
struct Light {
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

//  材质定义 结构体
struct Material {
	sampler2D diffuse;// 漫反射光照
    sampler2D specular;	// 镜面光照
    float shininess;	// 反光度
}; 

uniform Light light;
uniform Material material;
uniform vec3 viewPos;


void main()
{    
    // 环境光
	//													从纹理中采样片段的环境光颜色值
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);	// 标准化为单位向量
    vec3 lightDir = normalize(light.pos - FragPos);	// 光照方向 片段指向光源
    float diff = max(dot(norm, lightDir), 0.50);	// 物体表面法向量与光照向量的点乘

	//													从纹理中采样片段的漫反射颜色值
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.50), material.shininess);
    //vec3 specular = light.specular * (spec * material.specular);  
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));  

	// 衰减计算
	float distance    = length(light.pos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));

	// dot返回一个余弦值 角度越小余弦值越大
	float theta = dot(lightDir, normalize(-light.direction));

	// 内外半径的夹角  此范围内则为柔光处理
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    

	vec4 result;


	// 没有平滑边缘的效果
	////  在光照范围内
	//if (theta > light.cutOff)	// 两个余弦值比较
	//{
	//	// 执行光照计算
	//	result = vec4(ambient * attenuation + diffuse  * attenuation  + specular  * attenuation , 1.0);
	//}
	//else  // 光照范围外，使用环境光，让场景在聚光之外时不至于完全黑暗
	//{
	//	result = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
	//}

	// 平滑的边缘的效果
	result = vec4(ambient * attenuation + diffuse  * attenuation  * intensity+ specular  * attenuation * intensity, 1.0); 

	FragColor = result;
}