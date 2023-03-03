#version 330 core

out vec4 FragColor;
in vec2 TextureCoord;
in vec3 Normal;
in vec3 FragPos;
in mat4 mmvp;


//object information

uniform vec4 color;
uniform sampler2D image;

//light information
uniform vec4 lightColor;
uniform float ambientStrength = 0.1;
uniform float specularStrength = 0.1;

//positions
uniform vec3 viewPos;
uniform vec3 lightPosition;


void main()
{
	vec4 objectColor = color;//* texture(image, TextureCoord);
    vec4 ambient = ambientStrength * lightColor;

    //light position
    
    vec3 norm = normalize(Normal);
    vec3 norm1 = normalize(Normal);
	//vec4 objectColor = vec4(norm.xyz, 1.0f) * texture(image, TextureCoord);
    norm1 = mat3(mmvp) * Normal;
    norm = norm1;

	//vec4 objectColor = vec4(norm1.xyz, 1.0f);
    vec3 lightDir = normalize(lightPosition - FragPos);  

    
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor; 

    vec4 result = (ambient + diffuse + specular);
    result.w = 1.0f;

    result *= objectColor;
    FragColor = objectColor;
    FragColor = result;

		
}