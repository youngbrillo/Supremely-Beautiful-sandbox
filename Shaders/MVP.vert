#version 330 core
layout(location = 0) in vec3 vertbufferPos;
layout(location = 1) in vec2 vertbufferTexCoord;
layout(location = 2) in vec3 vertbufferNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TextureCoord;

void main()
{
	gl_Position = projection * view *  model * vec4(vertbufferPos, 1.0);
	TextureCoord = vertbufferTexCoord;
}