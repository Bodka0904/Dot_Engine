#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out float v_Dist;

uniform mat4 u_ViewProjectionMatrix;


void main()
{
	vec4 pos = u_ViewProjectionMatrix * vec4(a_Position, 0, 1.0);

	gl_Position = pos;
	v_TexCoord = a_TexCoord;
}


#type fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform float u_Color;
uniform sampler2D u_Texture;

void main()
{	
	vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;
	if (texColor.a < 0.1)
		discard;

	FragColor = texColor;
}
