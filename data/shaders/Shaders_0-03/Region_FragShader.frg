#version 460 core

uniform sampler2D uniform_Texture;
in vec2 TexCoord;
in float DifColor;
out vec4 FragColor;
void main() {
    FragColor = texture(uniform_Texture, TexCoord) * DifColor + 0.07;
}