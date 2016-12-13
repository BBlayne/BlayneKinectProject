#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout (location = 2) in vec3 Normal;   
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Max number of bones maybe.
const int MAX_BONES = 100;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 gBones[MAX_BONES];

void main(){
	

    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform += gBones[BoneIDs[3]] * Weights[3];
	
	vec4 PosL = BoneTransform * vec4(vertexPosition_modelspace, 1.0);
	
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * PosL;
	//gl_Position =  MVP * vec4(vertexPosition_modelspace, 1.0);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

