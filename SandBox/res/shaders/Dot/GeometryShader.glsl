#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out VS_OUT{
	vec2 texCoords;
	vec3 normals;
} vs_out;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 u_ModelMatrix;

out vec3 v_FragPos;

void main()
{
	vs_out.texCoords = a_TexCoord;
	vs_out.normals = (ViewProjectionMatrix * u_ModelMatrix * vec4(a_Normal, 0.0)).xyz;

	gl_Position = ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);

	v_FragPos = vec3(ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0));

}

#type fragment
in vec3 v_FragPos;
in vec2 v_TexCoord;
in vec3 v_Normal;


uniform float lightStrength;
uniform float specStrength;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - v_FragPos);
	vec3 ambient = lightStrength * lightColor;
	vec3 viewDir = normalize(viewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 specular = specStrength * spec * lightColor;
	vec3 diffuse = diff * lightColor;
	vec3 result = ambient + diffuse + specular;

	color = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);

}


#type geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 texCoords;
	vec3 normals;
} gs_in[];


out vec2 v_TexCoords;
out vec3 v_Normal;

uniform float time;

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(direction, 0.0);
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	v_TexCoords = gs_in[0].texCoords;
	v_Normal = gs_in[0].normals;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	v_TexCoords = gs_in[1].texCoords;
	v_Normal = gs_in[1].normals;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	v_TexCoords = gs_in[2].texCoords;
	v_Normal = gs_in[2].normals;
	EmitVertex();
	EndPrimitive();
}