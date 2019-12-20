#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;


out VS_OUT{
	vec3 normal;
} vs_out;


void main()
{	
	vs_out.normal = a_Normal;
	gl_Position = vec4(a_Position, 1);;
}

#type fragment
#version 330 core
in vec2 v_TexCoord;


uniform sampler2D u_Texture;
out vec4 color;

void main()
{
	color = texture(u_Texture, v_TexCoord);
}

#type geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 v_TexCoord;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

in VS_OUT{
	vec3 normal;
} gs_in[];

const float c_Size = 0.8;

void GenerateQuad()
{
	gl_Position = ViewProjectionMatrix * (gl_in[0].gl_Position + vec4(0, 0, 0.0, 0.0));    // 1:bottom-left
	v_TexCoord = vec2(0, 0);
	EmitVertex();


	gl_Position = ViewProjectionMatrix * (gl_in[0].gl_Position + vec4(c_Size, 0, 0.0, 0.0));    // 2:bottom-right
	v_TexCoord = vec2(1, 0);
	EmitVertex();

	
	gl_Position = ViewProjectionMatrix * (gl_in[0].gl_Position + vec4(0, c_Size, 0.0, 0.0));    // 3:top-left
	v_TexCoord = vec2(0, 1);
	EmitVertex();

	gl_Position = ViewProjectionMatrix * (gl_in[0].gl_Position + vec4(c_Size, c_Size, 0.0, 0.0));    // 4:top-right
	v_TexCoord = vec2(1, 1);
	EmitVertex();
	EndPrimitive();
}
void main()
{
	GenerateQuad();
}