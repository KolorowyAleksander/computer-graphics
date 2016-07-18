#version 130

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 lightPos0; //Light coordinates in world space

//Attributes
in vec4 vertex; //vertex coordinates in model space
in vec4 color;  //vertex color
in vec4 normal; //vertex normal

//Zmienne interpolowane
out vec4 vN; //interpolated normal vector in the eye space
out vec4 vV; //interpolated viewer vector in the eye space
out vec4 vL; //interpolated light vector in the eye space
out vec4 iC; //interpolated vertex color

void main(void) {
	gl_Position=P*V*M*vertex; //Compute vertex coordinates in the clip space

	vN=normalize(V*M*normal); //Compute and interpolate normal vector in the eye space
	vL=normalize(V*lightPos0-V*M*vertex); //Compute and interpolate viewer vector in the eye space
	vV=normalize(vec4(0,0,0,1)-V*M*vertex); //Compute and interpolate light vector in the eye space

	iC=color; //Interpolate vertex color
}
