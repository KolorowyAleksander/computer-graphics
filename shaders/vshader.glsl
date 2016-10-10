#version 130

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 lightPosition;

in vec4 vertex;
in vec4 color;
in vec4 normal;
in vec2 inTextureCoordinates;


out vec4 vN;
out vec4 vV;
out vec4 vL;
out vec2 textureCoordinates;

void main(void) {
	gl_Position=P*V*M*vertex; //Compute vertex coordinates in the clip space

	vN=normalize(V*M*normal); //Compute and interpolate normal vector in the eye space
	vL=normalize(V*lightPosition-V*M*vertex); //Compute and interpolate viewer vector in the eye space
	vV=normalize(vec4(0,0,0,1)-V*M*vertex); //Compute and interpolate light vector in the eye space

    textureCoordinates = inTextureCoordinates;
}
