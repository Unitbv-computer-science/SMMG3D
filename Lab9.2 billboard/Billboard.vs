#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;

// Output data ; will be interpolated for each fragment.
out vec2 TexCoords;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRightWorldspace;
uniform vec3 CameraUpWorldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	vec3 particleCenterWordspace = BillboardPos;
	
	vec3 vertexPositionWorldspace = 
		particleCenterWordspace
		+ CameraRightWorldspace * squareVertices.x * BillboardSize.x
		+ CameraUpWorldspace * squareVertices.y * BillboardSize.y;

	// Output position of the vertex
	gl_Position = VP * vec4(vertexPositionWorldspace, 1.0f);

	// TexCoords of the vertex. No special space for this one.
	TexCoords = squareVertices.xy + vec2(0.5, 0.5);
}

