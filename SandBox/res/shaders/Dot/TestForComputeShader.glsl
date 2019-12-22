#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;


out vec2 v_Position;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 u_ModelMatrix;



void main()
{
	vec4 pos = ViewProjectionMatrix * vec4(a_Position.xyz, 1.0);
	gl_Position = pos;
	v_Position = pos.xy;
}


#type fragment
#version 330 core

in vec2 v_Position;

out vec4 FragColor;

float PI = 3.14159265;

void main()
{
	vec2 p = vec2(0.5, 0.3);
	vec3 color1 = vec3(1, 0.3, 0.2);
	vec3 color2 = vec3(1, 0.3, 0.3);
	
	float f = 0.0;
	float g = 0.0;
	float h = 0.0;
	for (float i = 0.0; i < 30.0; i++)
	{
		float s = sin(v_Position.x + i * PI / 20.0) * 0.8;
		float c = cos(v_Position.y + i * PI / 20.0) * 0.8;
		float d = abs(p.x + c);
		float e = abs(p.y + s);
		f += 0.001 / d;
		g += 0.001 / e;
		h += 0.00003 / (d * e);
	}
	
	FragColor = vec4(f * color1 + g * color2 + vec3(h), 1.0);
}
