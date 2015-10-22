#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

in vec3 VertexNormal;
in vec4 VertexPosition;

out vec3 Normal;
out vec4 Position;
out vec3 EyeDirection;

void main () {
	Normal = normalize(m_normal * VertexNormal);
	Position = m_viewModel * VertexPosition;
	EyeDirection = vec3(-Position);
	gl_Position = m_pvm * VertexPosition;	
}