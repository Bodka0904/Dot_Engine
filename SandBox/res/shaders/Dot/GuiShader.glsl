#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out float v_Dist;

uniform mat4 u_ViewProjectionMatrix;
uniform vec2 u_MousePos;


void main()
{
	
	vec4 pos = u_ViewProjectionMatrix * vec4(a_Position, 0, 1.0);
	vec4 mousePos = u_ViewProjectionMatrix * vec4(u_MousePos, 0, 1.0);

	gl_Position = pos;


	v_Dist = distance(mousePos, pos)/5;
	v_TexCoord = a_TexCoord;
}


#type fragment
#version 330 core

in vec2 v_TexCoord;
in float v_Dist;

uniform sampler2D u_Texture;


////// Constants ///////
vec2 point_light_pos = vec2(0.4, 0.3);
vec3 point_light_col = vec3(0.999, 0.999, 0.999);

float point_light_intensity = 0.4;
float ambient_light_intensity = 0.5;

float PI = 3.14159265;
////////////////////////

out vec4 FragColor;


float rand(vec2 co) 
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


vec4 ColorizeBlackPixels()
{
	vec4 color = texture(u_Texture, v_TexCoord);
	
	if (color.x < 0.2f && color.y < 0.2f && color.z < 0.2f)
	{
		//float grain = rand(vec2(1, gl_FragCoord.x + sin(v_Dist)));

		vec2 p = vec2(0.5, 0.3);
		vec3 color1 = vec3(0.0, 0.3, 0.5);
		vec3 color2 = vec3(0.2, 0.3, 0.3);

		float f = 0.0;
		float g = 0.0;
		float h = 0.0;
		
		float dist = v_Dist;
		if (dist < 0.05)
			dist += 50;
			
		for (float i = 0.0; i < 30.0; i++)
		{
			float s = sin(dist + i * PI / 20.0) * 0.8;
			float c = cos(dist + i * PI / 20.0) * 0.8;
			float d = abs(p.x + c);
			float e = abs(p.y + s);
			f += 0.001 / d;
			g += 0.001 / e;
			h += 0.00003 / (d * e);
		}
		return vec4(f * color1 + g * color2 + vec3(h), 1.0);
	}
	return color;
}

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

	vec4 color = ColorizeBlackPixels();
	FragColor = color * vec4(result, 0.7);

}
