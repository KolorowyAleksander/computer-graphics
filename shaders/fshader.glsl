#version 130

out vec4 pixelColor; //Fragment shader output variable. Returns the (almost) final pixel color.

in vec4 vN; //interpolated normal vector in the eye space
in vec4 vV; //interpolated viewer vector in the eye space
in vec4 vL; //interpolated light vector in the eye space
in vec4 iC; //interpolated vertex color

void main(void) {
	vec4 mN=normalize(vN); //normalize interpolated normal vector in the eye space
	vec4 mL=normalize(vL); //normalize interpolated viewer vector in the eye space
	vec4 mV=normalize(vV); //normalize interpolated light vector in the eye space
	vec4 mR=reflect(-mL,mN); //compute reflected light vector in the eye space

	vec4 Ma=vec4(0,0,0,1); //Material color for ambient light
	vec4 La=vec4(0,0,0,1); //Ambient light color
	vec4 Md=iC; //Material color for diffused light
	vec4 Ld=vec4(1,1,1,1); //Diffused light color
	vec4 Ms=vec4(1,1,1,1); //Material color for specular light
	vec4 Ls=vec4(1,1,1,1); //Specular light color
	float shininess=50; //Material shininess (phong's exponent)

	float nl=max(0,dot(mN,mL)); //Compute Lambert's term
	float rv=pow(max(0,dot(mN,mL)),shininess); //Compute Phong's term

	pixelColor=Ma*La+Md*Ld*nl+Ms*Ls*rv; //Compute shading model and return as pixel color
}
