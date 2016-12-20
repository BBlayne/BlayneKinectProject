#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;   

out vec2 UV;          

void main(){

    gl_Position =  vec4(Position, 1.0);
	// UV of the vertex. No special space for this one.
	UV = TexCoord;
}

