#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoords;

uniform sampler2D terrainTexture;

out vec4 fragColor;

void main() {
    vec3 normal = normalize(fragNormal);
    vec4 textureColor = texture(terrainTexture, fragTexCoords);
    fragColor = vec4(textureColor.rgb, 1.0);
}
