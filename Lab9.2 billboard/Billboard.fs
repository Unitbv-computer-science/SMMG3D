#version 330 core

// Interpolated values from the vertex shaders
in vec2 TexCoords;

// Ouput data
out vec4 color;

uniform sampler2D billboardTextureSampler;

void main(){
	// Output color = color of the texture at the specified TexCoords
	color = texture( billboardTextureSampler, TexCoords );
}