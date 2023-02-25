#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//object information


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform vec4 color;

//light information
uniform vec4 lightColor;
uniform float ambientStrength = 0.1;
uniform float specularStrength = 0.1;

//positions
uniform vec3 viewPos;
uniform vec3 lightPosition;


void main()
{
	vec4 objectColor = color* texture(texture_diffuse1, TexCoords);
    vec4 ambient = ambientStrength * lightColor;

    //light position
    
    vec3 norm = normalize(Normal);
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