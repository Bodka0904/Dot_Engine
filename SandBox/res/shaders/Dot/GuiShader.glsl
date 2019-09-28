#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec2 a_Transformation;

out vec2 v_TexCoord;
out float v_Dist;

uniform mat4 u_ViewProjectionMatrix;
uniform vec2 u_MousePos;


void main()
{
	vec4 pos = u_ViewProjectionMatrix * vec4(a_Position + a_Transformation, 0, 1.0);
	vec4 mousePos = u_ViewProjectionMatrix * vec4(u_MousePos, 0, 1.0);


	gl_Position = pos;
	

	float dist = distance(mousePos, pos);

	v_Dist = dist;
	v_TexCoord = a_TexCoord;
}


#type fragment
#version 330 core

in vec2 v_TexCoord;
in float v_Dist;

out vec4 FragColor;

uniform float u_TexOffset;
uniform sampler2D u_Texture;


vec2 point_light_pos = vec2(0.4, 0.3);
vec3 point_light_col = vec3(0.999, 0.999, 0.999);

float point_light_intensity = 0.4;
float ambient_light_intensity = 0.5;


void main()
{
	float diff = 0;
	if (v_Dist <= point_light_intensity)
	{
		diff = 1.0 - abs(v_Dist / point_light_intensity);
	}

	vec3 diffuse = diff * point_light_col;
	vec3 ambient = point_light_col * ambient_light_intensity;

	vec3 result = ambient + diffuse;
	FragColor = texture(u_Texture, vec2(v_TexCoord.x + (u_TexOffset / 2), v_TexCoord.y)) * vec4(result, 0.7);
	
}
