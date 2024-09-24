#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_proj;

out vec3 fragNormal;
out vec2 fragTexCoords;

void main() {
    gl_Position = m_proj * m_view * m_model * vec4(inPosition, 1.0);
    fragNormal = mat3(transpose(inverse(m_model))) * inNormal;
    fragTexCoords = inTexCoords;
}
