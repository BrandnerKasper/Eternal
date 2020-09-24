//2 in 1 Shader for either a simple color or a texture with texture scaling and tint :)

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexID;


uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexID = a_TexID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;

uniform sampler2D u_Textures[32];

void main()
{
//TODO Tiling! and Tint!!
	color = texture(u_Textures[int(v_TexID)], v_TexCoord) * v_Color;
}