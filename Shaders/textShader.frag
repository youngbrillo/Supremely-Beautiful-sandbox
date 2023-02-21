#version 330 core
in vec2 TexCoords;
out vec4 Fragolor;

uniform sampler2D text;
uniform vec4 color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    Fragolor = color * sampled;

   //color = fontColor * texture(text, TexCoords);
}  