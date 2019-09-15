#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform vec2 u_Position;
out vec2 UV;

void main()
{

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position + u_Position, 0, 1);

	UV = a_TexCoord;
}


#type fragment
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, UV);
	if (texColor.a < 0.1)
		discard;

	color = texture(u_Texture, UV) * vec4(0.0,0,0.0,1);
}
