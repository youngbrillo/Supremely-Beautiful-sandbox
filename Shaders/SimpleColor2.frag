#version 330 core

out vec4 FragColor;
in vec2 TextureCoord;

uniform vec4 color;
uniform sampler2D image;

void main()
{
	//FragColor = color;

	FragColor = vec4(color);// * texture(image, TextureCoord);
		
	if (FragColor.a < 0.1)
		discard;
}