varying vec3 varyingNormal;
varying vec2 varyingTexCoord;
varying vec3 varyingPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

struct Light {
	vec3 lightDirection;
	vec3 lightColor;
	vec3 specularLightColor;
};

uniform Light lights[1];

void main() {
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	
	
	for(int i=0;i<1;i++){
	
		float diffuse = max(0.0, dot(varyingNormal, lights[i].lightDirection));
		diffuseColor = lights[i].lightColor * diffuse;
	
		vec3 v = normalize(-varyingPosition);
		vec3 h = normalize(v + lights[i].lightDirection);
		
		float specular = pow(max(0.0, dot(h,varyingNormal)),64.0);
		specularColor = lights[i].specularLightColor * specular;
	}
	
	vec3 intensity = ((texture2D(diffuseTexture, varyingTexCoord).xyz) * diffuseColor) +((texture2D(specularTexture, varyingTexCoord).x) * specularColor);
	gl_FragColor = vec4(intensity.xyz,1.0);;
}