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
uniform sampler2D txtColor;
uniform sampler2D txtNormal;

uniform float ambient;
uniform float diffuse;
uniform float specular;

//=======================================================================
// varying contain interpolated values from rasterizer
//=======================================================================
varying vec4 world_pos;
varying vec3 world_normal;
varying vec3 world_tang;
varying vec3 world_bitang;
varying vec2 tex_coords;

//=======================================================================
// Computes the intensity of the reflected diffuse light for a point
// at position 'position' with normal vector 'normal', if the light
// source is located at 'lightPos'.
//=======================================================================
vec2 computeDiffuseSpecIntens(in vec3 position, in vec3 normal, in vec3 cameraPos, in vec3 lightPos)
{
	normal = normalize(normal);

// TASK 3b)
// Copy the implementation of this function from phong.glslf

	return vec2(1, 0);	// Return dummy value
	
// ~TASK

}


//=======================================================================
// fragment program entry point
//=======================================================================
void main(void)
{
// TASK 3b)
// Compute perturbed normal from world_normal, world_tang, world_bitang and

	vec3 normal = normalize(world_normal);	// Dummy code: Use unperturbed normal
// ~TASK

	// Do some simple diffuse shading
	vec2 diffuseIntensitySpec = computeDiffuseSpecIntens(world_pos.xyz, normal.xyz, w_CameraPosition.xyz, w_LightPosition.xyz);
	vec4 color = texture2D(txtColor, tex_coords);
	
	gl_FragColor = color*(diffuseIntensitySpec.x * diffuse + diffuseIntensitySpec.y * specular + ambient);
}
