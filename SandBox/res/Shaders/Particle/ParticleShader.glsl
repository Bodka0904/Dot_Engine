#type vertex
#version 430 core

layout(location = 0) in vec4 a_Offset;
layout(location = 1) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out float v_Transparent;

layout(std140,binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};
uniform mat4 u_ModelMatrix;


float c_Stages = 64;
float c_Row = 8;

void main()
{
	//vec2 texCoord = a_TexCoord;
	//
	//float atlasProgression = a_Offset.w * c_Stages;
	//int index = int(floor(atlasProgression));
	//int column = int(mod(index, c_Row));
	//int row = int(index / c_Row);
	//
	//texCoord.x += float(column)/c_Row;
	//texCoord.y += float(row)/c_Row;

	v_TexCoord = a_TexCoord;
	v_Transparent = a_Offset.w;

	gl_Position = ViewProjectionMatrix * u_ModelMatrix * vec4( (a_Position * a_Offset.w) + a_Offset.xyz, 1);
}


#type fragment
#version 430 core

in vec2 v_TexCoord;
in float v_Transparent;

out vec4 FragColor;

uniform float u_Time;
uniform vec3 u_Color;
uniform sampler2D t_Texture;


void main()
{	
	vec4 color = vec4(v_Transparent,v_Transparent,v_Transparent,1);
	vec4 texColor = texture(t_Texture, v_TexCoord) * vec4(0.7,0.3,0,v_Transparent) * color;
	
	if(texColor.a == 0.0)
		discard;
	gl_FragColor = texColor;
}

