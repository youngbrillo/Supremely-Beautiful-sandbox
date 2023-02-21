#version 330 core
layout(location = 0) in vec3 vertbufferPos;
layout(location = 1) in vec2 vertbufferTexCoord;
layout(location = 2) in vec3 vertbufferNormals;

out vec2 TextureCoord;

uniform mat4 projection = mat4(1.0f);
uniform mat4 model = mat4(1.0f);

void main()
{
    gl_Position = projection * model * vec4(vertbufferPos, 1.0);
    //TexCoords = vertbufferPos.zw;
    TextureCoord = vec2(vertbufferTexCoord.x, vertbufferTexCoord.y);
}  