
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
};

//  材质定义 结构体
struct Material {
    //vec3 ambient;	// 环境光照
	
	sampler2D diffuse;
    //vec3 diffuse;		// 漫反射光照

    vec3 specular;	// 镜面光照

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
    float diff = max(dot(norm, lightDir), 0.0);	// 物体表面法向量与光照向量的点乘

	//													从纹理中采样片段的漫反射颜色值
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}