varying vec4 varyingNormal;
varying vec2 varyingTexCoord;
varying vec4 projectedTexCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D proejctedTexture;

void main() {

	vec2 tex2;
	tex2.x = projectedTexCoord.x/projectedTexCoord.w;
	tex2.y = projectedTexCoord.y/projectedTexCoord.w;
	
	vec4 projectorColor = vec4(0.0,0.0,0.0,0.0);
	if(tex2.x < 1.0 && tex2.y < 1.0 && tex2.x > 0.0 && tex2.y > 0.0){
		projectorColor = texture2D(proejctedTexture,tex2);
	}
	
	float diffuse = max(0.0, dot(varyingNormal, vec4(-0.5773, 0.5773, 0.5773, 0.0)));
	gl_FragColor = texture2D(diffuseTexture,varyingTexCoord);
}