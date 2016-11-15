<<<<<<< HEAD
varying vec3 varyingNormal;
varying vec3 varyingPosition;
=======
varying vec4 varyingNormal;
varying vec2 varyingTexCoord;

uniform sampler2D diffuseTexture;
>>>>>>> 3Dtexture
uniform vec3 uColor;


struct Light{
	vec3 lightPosition;
	vec3 lightColor;
	vec3 specularLightColor;
};

uniform Light lights[2];

void main() {
<<<<<<< HEAD
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	for(int i=0; i< 2; i++) {
		vec3 lightDirection = - normalize(varyingPosition - lights[i].lightPosition);
		float diffuse = max(0.0, dot(varyingNormal, lightDirection));
		diffuseColor += lights[i].lightColor * diffuse;
		vec3 v = normalize(-varyingPosition);
		vec3 h = normalize(v + lightDirection);
		float specular = pow(max(0.0, dot(h, varyingNormal)), 2.0);
		specularColor += lights[i].specularLightColor * specular;
	}
	vec3 intensity = (uColor * diffuseColor) + specularColor;
	gl_FragColor = vec4(intensity.xyz, 1.0);
=======
	float diffuse = max(0.0, dot(varyingNormal, vec4(-0.5773, 0.5773, 0.5773, 0.0)));
	vec3 intensity = uColor * diffuse;
	//gl_FragColor = vec4(varyingTexCoord.x,0.0,varyingTexCoord.y, 1.0);
	gl_FragColor = texture2D(diffuseTexture,varyingTexCoord);
>>>>>>> 3Dtexture
}