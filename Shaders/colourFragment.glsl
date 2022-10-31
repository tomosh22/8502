#version 330 core

in Vertex{
	vec4 colour;
} IN;

out vec4 fragColour;
void main(void){
	//if(IN.colour.x < 0.5f && IN.colour.y < 0.5f && IN.colour.z < 0.5f)discard;
	float val = 0.05f;
	if(IN.colour.x < val || IN.colour.y < val || IN.colour.z < val){
		fragColour = IN.colour;
	}
	else{
		discard;
	}
	
}