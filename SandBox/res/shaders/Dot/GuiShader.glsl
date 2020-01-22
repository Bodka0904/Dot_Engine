#type vertex
#version 430 core

layout(location = 0) in vec4 a_Color;
layout(location = 1) in vec2 a_Position;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec4 v_Color;


uniform mat4 u_ViewProjectionMatrix;

void main()
{	
	vec4 pos = u_ViewProjectionMatrix * vec4(a_Position, 0, 1.0);

	gl_Position = pos;
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
}


#type fragment
#version 430 core

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;


out vec4 FragColor;


void main()
{	
	FragColor = texture(u_Texture, v_TexCoord) * vec4(v_Color.xyz,1);
}
