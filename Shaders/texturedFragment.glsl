#version 330 core
#extension GL_ARB_bindless_texture : enable

uniform sampler2D reflectTex;
uniform sampler2D refractTex;
uniform sampler2D waterTex;
uniform sampler2D waterNormal;
uniform int useTexture;
uniform float blendFactor;
uniform float time;

uniform vec3 cameraPos;
uniform vec4 diffuseColour;
uniform vec4 specularColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex {
	vec4 clipSpace;
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} IN;

out vec4 fragColour[2];
void main(void){
	if(true){
		vec2 projTexCoord = (IN.clipSpace.xy / IN.clipSpace.w) / 2 + 0.5;
		//texCoord -= vec2(0.02, 0.02);
		
		//texCoord = clamp(texCoord,0.001, 0.999);
		//if (texCoord.x < 0) { texCoord.x = 0.0; }
		vec4 reflectColour = texture(reflectTex, vec2(projTexCoord.x,1- projTexCoord.y));
		fragColour[0] = mix(reflectColour,texture(refractTex, projTexCoord),0.5);
		fragColour[0] = mix(fragColour[0],texture(waterTex, IN.texCoord*16+time/10),0.2);
		//return;
		//fragColour[0] *= 1;
		//fragColour += vec4(-0.1, 0, 0.1, 0);
		//fragColour = vec4(texCoord + vec2(0.5,0.5), 0, 1);
		//fragColour = vec4(1, 0, 0, 1);
	}
	else{
		//fragColour = IN.colour;
	}
	//return;
	//if(IN.jointWeights.y == 0){
		//fragColour *= vec4(1,0,0,1);
	//}
	//if(IN.height > 180){
	//	fragColour = vec4(1.0,1.0,1.0,1.0) - texture(diffuseTex, IN.texCoord)/5;
	//}
	//fragColour = vec4(0,1,0,1);


	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec4 diffuse = fragColour[0];
	vec3 bumpNormal = texture(waterNormal, IN.texCoord * 16 + time / 10).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2 - 1));
	float lambert = max(dot(incident, bumpNormal), 0);
	float distance = length(lightPos - IN.worldPos);
	float attenuation = 1 - clamp(distance / lightRadius, 0, 1);
	float specFactor = clamp(dot(halfDir, bumpNormal), 0, 1);
	specFactor = pow(specFactor, 10);
	
	vec3 surface = (diffuse.rgb * diffuseColour.rgb);
	fragColour[1] = vec4(bumpNormal.xyz, 1);
	return;
	fragColour[0].rgb = surface * lambert * attenuation;
	
	fragColour[0].rgb += (specularColour.rgb * specFactor) * attenuation * 0.33;
	
	fragColour[0].rgb += surface * 0.1;
	fragColour[0].a = diffuse.a;
	fragColour[0] += diffuse * 0.5;
	
}