#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 highlightColor;
uniform vec3 textColor;

uniform float percentToHighlight;
uniform float alpha;

void main()
{    
	
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r * alpha);
	
	if(percentToHighlight > TexCoords.x || percentToHighlight == -1){
	   color = vec4(highlightColor, 1.0) * sampled;
	}
	else{
	    color = vec4(textColor, 1.0) * sampled;

	}
	
}