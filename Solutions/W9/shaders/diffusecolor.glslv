//=======================================================================
// World Information
//=======================================================================
uniform mat4 w_ViewMatrix;				// The camera vie matrix
uniform mat4 w_ProjectionMatrix;		// The projection matrix
uniform mat4 w_ProjectionViewMatrix;	// multiblied matrix
		
uniform vec4 w_CameraPosition;			// camera position in world coord
uniform vec4 w_LightPosition;			// light position (first light)

uniform vec4 w_LightPositions[4];		// position of all lights

//=======================================================================
// Model Information
//=======================================================================
uniform mat4 m_ModelMatrix;
uniform mat4 m_ModelNormalMatrix;
uniform mat4 m_ViewModelMatrix;
uniform mat4 m_ProjectionViewModelMatrix;

//=======================================================================
// None Default Uniforms
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
	// do everyting in woorld coordianates
	world_pos = m_ModelMatrix * position;
	world_normal = m_ModelNormalMatrix * normal;
	
   	gl_Position = w_ProjectionViewMatrix * world_pos;
}
