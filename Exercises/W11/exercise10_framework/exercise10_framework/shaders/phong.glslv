#version 130

//=======================================================================
// World Information
//=======================================================================
uniform mat4 w_ViewMatrix;				// M_view
uniform mat4 w_ProjectionMatrix;		// M_proj
uniform mat4 w_ProjectionViewMatrix;	// M_proj * m_view
		
uniform vec4 w_CameraPosition;			// camera position in world coord
uniform vec4 w_LightPosition;			// light position (first light)

uniform vec4 w_LightPositions[4];		// position of all lights

//=======================================================================
// Model Information
//=======================================================================
uniform mat4 m_ModelMatrix;					// M_model
uniform mat4 m_ModelNormalMatrix;			// M_model^-T
uniform mat4 m_ViewModelMatrix;				// M_view * M_model
uniform mat4 m_ProjectionViewModelMatrix;	// M_proj * M_view * M_model

//=======================================================================
// Default Uniforms
//=======================================================================
uniform vec4 color;

//=======================================================================
// Attributes: Per vertex data
//=======================================================================
attribute vec4 position;			// Vertex coordinates in model space
attribute vec4 normal;				// Vertex normal
attribute vec2 texcoord;			// Vertex normal


//=======================================================================
// Varying: Vertex shader output, input to rasterizer
//=======================================================================
varying vec4 world_pos;
varying vec4 world_normal;

//=======================================================================
// Vertex program entry point
//=======================================================================
void main(void)
{
	// Transform position to world coordinates:
	world_pos = m_ModelMatrix * position;
	
	// Trasnform normal to world coordinates:
	world_normal = m_ModelNormalMatrix * normal;
	
   	gl_Position = w_ProjectionViewMatrix * world_pos;
}
