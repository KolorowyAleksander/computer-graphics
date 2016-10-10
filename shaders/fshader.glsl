#version 130

uniform sampler2D textureMap;
uniform sampler2D textureShineMap;

in vec4 vN; //interpolated normal vector in the eye space
in vec4 vV; //interpolated viewer vector in the eye space
in vec4 vL; //interpolated light vector in the eye space
in vec2 textureCoordinates;

out vec4 pixelColor; //Fragment shader output variable. Returns the (almost) final pixel color.

void main(void) {
	vec4 mN=normalize(vN); //normalize interpolated normal vector in the eye space
    vec4 mL=normalize(vL); //normalize interpolated viewer vector in the eye space
    vec4 mV=normalize(vV); //normalize interpolated light vector in the eye space
    vec4 mR=reflect(-mL,mN); //compute reflected light vector in the eye space

    vec4 La=vec4(0,0,0,1);
    vec4 Ma=vec4(0,0,0,1);

    vec4 Ld=vec4(1,1,1,1);
    vec4 Md=texture(textureMap, textureCoordinates);

    vec4 Ls=vec4(0,0,0,1); //Specular light color
    vec4 Ms=texture(textureShineMap, textureCoordinates);

    float shininess=50;
    float nl=max(0,dot(mL,mN)); //Compute Lambert's term
    float rv=pow(max(0,dot(mR,mL)), shininess); //Compute Phong's term

    pixelColor=Ma*La+clamp(Md*Ld*nl, 0.0, 1.0)+Ms*Ls*rv; //Compute shading model and return as pixel color
}
