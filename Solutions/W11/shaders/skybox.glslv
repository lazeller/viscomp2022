#version 130

//=======================================================================
// Default Uniforms
//=======================================================================
uniform mat4 backprojectMatrix;

//=======================================================================
// Attributes: Per vertex data
//=======================================================================
attribute vec4 position;			// Vertex coordinates in model space


//=======================================================================
// Varying: Vertex shader output, input to rasterizer
//=======================================================================
varying vec4 world_pos;

//=======================================================================
// Vertex program entry point
//=======================================================================
void main(void)
{
	world_pos = backprojectMatrix * position;
	
   	gl_Position = position;
}
