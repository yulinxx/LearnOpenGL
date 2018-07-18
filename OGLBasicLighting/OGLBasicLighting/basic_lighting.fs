#version 330 core


uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;
uniform vec3 lightPos;		// 光源的位置

uniform float specularStrength; 	// 镜面高光强度控制参数

in vec3 Normal;		// 向量 由VertexShader传入
in vec3 FragPos;	// 顶点的世界坐标

out vec4 FragColor;

void main()
{

	vec3 norm = normalize(Normal);	// normalize标准化为单位向量
	vec3 lightDir = normalize(lightPos - FragPos); // 光照方向 片段指向光源

	// ---- 
	// 计算光源对当前片段实际的漫发射影响
	// 两个向量之间的角度越小，漫反射分量就会越大
	// 大于90度后,则为负,所以用max限制为 0和点乘结果 两者的最大值 确保不小于0
	float diff = max(dot(norm, lightDir), 0.0);	// 光线对片段颜色的影响
	vec3 diffuse = diff * lightColor;	// 漫反射
	
	// ---- 
	// 光的颜色乘以一个常量因子, 最终参与计算,作为环境光对物体的影响
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;	// 环境光
	
	// ---- 
	// 对 lightDir 光照方向进行了取反, 
	// 因为reflect函数要求第一个向量是从光源指向片段位置的向量，
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 viewDir = normalize(viewPos - FragPos); // 观测法线 片段指向观察点

	// 先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。
	//这个32是高光的反光度(Shininess)。一个物体的反光度越高，
	// 反射光的能力越强，散射得越少，高光点就会越小。
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;	// 镜面反射

	// ----  最终 　(环境光 + 漫反射 + 镜面光) * 物体颜色
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}