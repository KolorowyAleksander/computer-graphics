#version 130

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 lightPosition;
uniform vec4 lightDirection;

in vec4 vertex;
in vec4 color;
in vec4 normal;
in vec2 inTextureCoordinates;


out vec4 vN;
out vec4 vV;
out vec4 vL;
out vec4 lightRayWorldSpace;
out vec4 vertexPosWorldSpace;
out vec4 lightDir;
out float distance;
out vec2 textureCoordinates;

void main(void) {
	gl_Position=P*V*M*vertex; //Compute vertex coordinates in the clip space

    distance = length(lightPosition - M * vertex);
    lightRayWorldSpace = normalize(lightPosition - M*vertex);
    lightDir = normalize(lightDirection - lightPosition);

	vN=normalize(V*M*normal);
	vL=normalize(V*lightPosition-V*M*vertex); //pointing from light to fragment
	vV=normalize(V*M*vertex - V*lightPosition); //pointing from fragment to light

    textureCoordinates = inTextureCoordinates;
}
