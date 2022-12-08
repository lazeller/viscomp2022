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
uniform sampler2D txtColor;
uniform samplerCube txtEnvMap;

uniform float ambient;
uniform float diffuse;
uniform float specular;

//=======================================================================
// varying contain interpolated values from rasterizer
//=======================================================================
varying vec4 world_pos;
varying vec3 world_normal;
varying vec2 tex_coords;

//=======================================================================
// Computes the intensity of the reflected diffuse light for a point
// at position 'position' with normal vector 'normal', if the light
// source is located at 'lightPos'.
//=======================================================================
vec2 computeDiffuseSpecIntens(in vec3 position, in vec3 normal, in vec3 cameraPos, in vec3 lightPos)
{
	normal = normalize(normal);

// TASK 2a)
// Copy the implementation of this function from phong.glslf

	return vec2(1, 0);	// Return dummy value
	
// ~TASK
}


//=======================================================================
// fragment program entry point
//=======================================================================
void main(void)
{
	vec3 normal = normalize(world_normal);

// TASK 2a)
// Read the texture (txtColor) at texture coordinate 'tex_coords', store
// the color in colorTex
	
	vec4 colorTex = vec4(1, 1, 1, 1);	// dummy value
	
// ~TASK

// TASK 2b)
// Reflect the 'camera to pixel' vector at the normal and use this reflected vector
// to perform a texture lookup into the cube map 'txtEnvMap', using the 'textureCube' command.
// Optionally, compute the reflection term (Fresnel term) 'reflTerm' to increase the amount of reflection
// when looking at the surface at an oblique angle.

	float reflTerm = 0.5;
	vec4 colorCube = vec4(0.8, 0.8, 1, 1);	// dummy value

// ~TASK
	
	// Do some simple diffuse shading
	vec2 diffuseIntensitySpec = computeDiffuseSpecIntens(world_pos.xyz, normal.xyz, w_CameraPosition.xyz, w_LightPosition.xyz);
	
	vec4 colObject = (1.0 - reflTerm) * colorTex*(diffuseIntensitySpec.x * diffuse + diffuseIntensitySpec.y * specular + ambient);
	vec4 colReflected = colorCube * reflTerm;
	
	gl_FragColor = colObject + colReflected;
	gl_FragColor.a = 0.5;
}
