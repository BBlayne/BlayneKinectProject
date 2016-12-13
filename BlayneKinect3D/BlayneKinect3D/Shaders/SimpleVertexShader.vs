#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;    

out vec2 UV;                                                               
out vec3 Normal0;                                                                   
out vec3 WorldPos0;  

uniform mat4 MVP;

void main(){

    gl_Position =  MVP * vec4(Position, 1.0);
	// UV of the vertex. No special space for this one.
	UV = TexCoord;
	Normal0     = (MVP * vec4(Normal, 0.0)).xyz;          
	WorldPos0   = (MVP * vec4(Position, 1.0)).xyz;   
}

