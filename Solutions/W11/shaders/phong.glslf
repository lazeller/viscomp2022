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
uniform float ambient;
uniform float diffuse;
uniform float specular;

//=======================================================================
// varying contain interpolated values from rasterizer
//=======================================================================
varying vec4 world_pos;
varying vec4 world_normal;

//=======================================================================
// Computes the intensity of the reflected diffuse light for a point
// at position 'position' with normal vector 'normal', if the light
// source is located at 'lightPos'.
//=======================================================================
vec2 computeDiffuseSpecIntens(in vec3 position, in vec3 normal, in vec3 cameraPos, in vec3 lightPos)
{
	normal = normalize(normal);

/*	
// TASK 1a)
// Given the position of the pixel, its normal vector, the position of the camera and the position of the light,
// (all in world coordinates), compute a 2D vector that contains in its first component the intensity of
// the diffuse light and in its second component the intensity of the specular light.

	return vec2(1, 0);	// Return dummy value
	
// ~TASK
*/

// SOLUTION
	vec3 vecToLight = normalize(lightPos - position);
	vec3 vecToCam = normalize(cameraPos - position);
	vec3 reflectedLight = -reflect(vecToLight, normal);
	
	float specPower = 50.0;
	
	vec2 DiffuseSpecIntens;
	DiffuseSpecIntens.x = clamp(dot(normal, vecToLight), 0.0, 1.0);
	DiffuseSpecIntens.y = pow(clamp(dot(reflectedLight, vecToCam), 0.0, 1.0), specPower);
	
	return DiffuseSpecIntens;
// ~SOLUTION
}

//=======================================================================
// fragment program entry point
//=======================================================================
void main(void)
{
	// Compute coefficients for diffuse and specular light:
	vec2 diffuseIntensitySpec = computeDiffuseSpecIntens(world_pos.xyz, world_normal.xyz, w_CameraPosition.xyz, w_LightPosition.xyz);
	
	gl_FragColor = color*(diffuseIntensitySpec.x * diffuse + diffuseIntensitySpec.y * specular + ambient);
}
