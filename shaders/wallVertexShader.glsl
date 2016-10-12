#version 130

//PVM
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 lightDirection;
uniform vec4 lightPosition;

in vec4 vertex;
in vec4 normal;
in vec2 inTextureCoordinates;

out vec4 normalVectorEyeSpace;
out vec4 lightVectorEyeSpace;
out vec4 lightDirectionEyeSpace;

out float distance;
out vec2 textureCoordinates;

void main(void) {
	gl_Position=P * V * M * vertex;

    distance = length(lightPosition - M * vertex);

	normalVectorEyeSpace = V * M * normal;
	lightVectorEyeSpace = V * lightPosition - V * M * vertex;
    lightDirectionEyeSpace = V * lightDirection;

    textureCoordinates = inTextureCoordinates;
}
