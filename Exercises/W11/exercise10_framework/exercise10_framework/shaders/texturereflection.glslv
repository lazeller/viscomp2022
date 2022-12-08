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

//=======================================================================
// Attributes: Per vertex data
//=======================================================================
attribute vec4 position;			// Vertex coordinates in model space
attribute vec4 normal;				// Vertex normal
attribute vec2 texcoord;			// Vertex texcoord
attribute vec3 tangent;				// Vertex tangent
attribute vec3 bitangent;			// Vertex bitangent


//=======================================================================
// Varying: Vertex shader output, input to rasterizer
//=======================================================================
varying vec4 world_pos;
varying vec3 world_normal;
varying vec2 tex_coords;

//=======================================================================
// Vertex program entry point
//=======================================================================
void main(void)
{
	world_pos = m_ModelMatrix * position;
	
	world_normal = (m_ModelNormalMatrix * vec4(normal.xyz, 1)).xyz;

	tex_coords = texcoord;

   	gl_Position = w_ProjectionViewMatrix * world_pos;
}
