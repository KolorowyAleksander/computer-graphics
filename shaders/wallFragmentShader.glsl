#version 130

uniform float umbra;
uniform float penumbra;

uniform sampler2D textureMap;
uniform sampler2D textureShineMap;

in vec4 normalVectorEyeSpace;
in vec4 lightVectorEyeSpace;
in vec4 lightDirectionEyeSpace;

in float distance;
in vec2 textureCoordinates;

out vec4 pixelColor;

void main(void) {

    vec4 normalVector = normalize(normalVectorEyeSpace);
    vec4 lightVector = normalize(lightVectorEyeSpace);

    float attenuation = 1/(distance*distance);

    vec4 lightDiffuse = vec4(1, 1, 1, 1);
    vec4 diffuse = attenuation * texture(textureMap, textureCoordinates);

    vec4 lightAmbient=vec4(0, 0, 0, 1);
    vec4 ambient=vec4(0.3, 0.3, 0.3, 1) * diffuse;


    vec4 reflection = reflect(-lightVector, normalVector);

    vec4 specular =
        attenuation
        * texture(textureShineMap, textureCoordinates)
        * pow(clamp(dot(lightVector, reflection), 0, 1), 10);

    //consine of angle which cuts off the light around
    float theta = dot(-lightVector, normalize(lightDirectionEyeSpace));

    if(theta > umbra) {
        pixelColor = ambient * lightAmbient + diffuse * lightDiffuse + specular;
    } else if (theta > penumbra){
        float epsilon = umbra - penumbra;
        float intensity = clamp((theta - penumbra) / epsilon, 0.0, 1.0);
        pixelColor = intensity * (diffuse * lightDiffuse + ambient * lightAmbient + specular);
    } else {
        pixelColor = ambient * lightAmbient;
    }
}
