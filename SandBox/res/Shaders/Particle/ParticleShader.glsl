#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;

out VS_OUT{
	 vec4 position;
} vs_out;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 u_ModelMatrix;

void main()
{
	vec4 pos = ViewProjectionMatrix * u_ModelMatrix* vec4(a_Position.xyz, 1);
	gl_Position = pos;
}


#type fragment
#version 330 core

in vec2 v_TexCoord;
in vec4 v_Position;
uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord) * vec4(0.8, 0.3, 0, 0.2);
	if (texColor.w < 0.1)
		discard;

	FragColor = texColor ;
}


#type geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in VS_OUT{
	 vec4 position;
} gs_in[];

out vec2 v_TexCoord;
out vec4 v_Position;

uniform float u_Size;

void generate_quad(vec4 position)
{
	gl_Position = position + vec4(-u_Size, -u_Size, 0.0, 0.0);    // 1:bottom-left
	v_TexCoord = vec2(0, 0);
	v_Position = gl_Position;
	EmitVertex();
	gl_Position = position + vec4(u_Size, -u_Size, 0.0, 0.0);    // 2:bottom-right
	v_TexCoord = vec2(1, 0);
	v_Position = gl_Position;
	EmitVertex();
	gl_Position = position + vec4(-u_Size, u_Size*2, 0.0, 0.0);    // 3:top-left
	v_TexCoord = vec2(0, 1);
	v_Position = gl_Position;
	EmitVertex();
	gl_Position = position + vec4(u_Size, u_Size*2, 0.0, 0.0);    // 4:top-right
	v_TexCoord = vec2(1, 1);
	v_Position = gl_Position;
	EmitVertex();
	EndPrimitive();
}

void main() 
{
	generate_quad(gl_in[0].gl_Position);
}