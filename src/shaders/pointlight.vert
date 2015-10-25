#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

in vec3 VertexNormal;
in vec4 VertexPosition;
in vec4 texCoord;

out vec3 Normal;
out vec4 Position;
out vec3 EyeDirection;
out vec2 tex_coord;

void main () {
	Normal = normalize(m_normal * VertexNormal);
	Position = m_viewModel * VertexPosition;
	EyeDirection = normalize(vec3(-Position));
	gl_Position = m_pvm * VertexPosition;
	tex_coord = texCoord.st;
};