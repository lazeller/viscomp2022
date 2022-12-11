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
attribute vec2 texcoord;			// Vertex texcoord
attribute vec3 tangent;				// Vertex tangent
attribute vec3 bitangent;			// Vertex bitangent


//=======================================================================
// Varying: Vertex shader output, input to rasterizer
//=======================================================================
varying vec4 world_pos;
varying vec2 tex_coords;
varying vec3 world_normal;
varying vec3 world_tang;
varying vec3 world_bitang;

//=======================================================================
// Vertex program entry point
//=======================================================================
void main(void)
{

// TASK 3b)
// Compute values of:
// - world_pos
// - world_normal
// - world_tang
// - world_bitang
// - tex_coords
// ~TASK

// SOLUTION
	world_pos = m_ModelMatrix * position;
	
	// We need to use
	//    n' = (M^-1^T) * n
	// to transform the normal vector n. Considering the plane define by this normal,
	// if the points p on the plane are transformed by
	//    p' = M * p,
	// n' will be orthogonal to the transformed plane, which is exactly what we want.
	world_normal = (m_ModelNormalMatrix * vec4(normal.xyz, 1)).xyz;

	// Tangent and bitangent vectors are directions on the plane, so the model matrix M is used
	// for their transformation, however the translational part is ignored by setting the w
	// component to zero:
	world_tang = (m_ModelMatrix * vec4(tangent.xyz, 0)).xyz;
	world_bitang = (m_ModelMatrix * vec4(bitangent.xyz, 0)).xyz;

	tex_coords = texcoord;
// ~SOLUTION

   	gl_Position = m_ProjectionViewModelMatrix * position;
}
