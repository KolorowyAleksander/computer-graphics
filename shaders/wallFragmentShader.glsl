#version 130

uniform float cutoffAngle;
uniform sampler2D textureMap;
uniform sampler2D textureShineMap;

in vec4 vN;
in vec4 vV;
in vec4 vL;
in vec4 lightRayWorldSpace;
in vec4 vertexPosWorldSpace;
in vec4 lightDir;
in float distance;
in vec2 textureCoordinates;

out vec4 pixelColor; //Fragment shader output variable. Returns the (almost) final pixel color.

void main(void) {

    vec4 mN=normalize(vN); //normalize interpolated normal vector in the eye space
    vec4 mL=normalize(vL); //normalize interpolated viewer vector in the eye space
    vec4 mV=normalize(vV); //normalize interpolated light vector in the eye space
    vec4 mR=reflect(-mL,mN); //compute reflected light vector in the eye space

    vec4 La=vec4(0,0,0,1);
    vec4 Ma=vec4(0,0,0,1);

    vec4 Ls=vec4(1,1,1,1); //Specular light color
    vec4 Ms=texture(textureShineMap, textureCoordinates);
    vec4 Ld=vec4(1,1,1,1);
    vec4 Md=texture(textureMap, textureCoordinates);

    float shininess=25;
    float nl=max(0,dot(mL,mN)); //Compute Lambert's term
    float rv=pow(max(0,dot(mR,mL)), shininess); //Compute Phong's term

    float theta = dot(lightRayWorldSpace, -lightDir);

    if(theta < cutoffAngle) {
        if (distance != 0) {
        pixelColor=Ma*La+0.5*1/distance*Md*Ld+1/distance*Ms*Ls*rv; //Compute shading model and return as pixel color
        }
    } else {
        pixelColor=0.01*Md*Ld;
    }
}
