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
// varying contain interpolated values from rasterizer
//=======================================================================
varying vec4 world_pos;
varying vec4 world_normal;

//=======================================================================
// Computes the intensity of the reflected diffuse light for a point
// at position 'position' with normal vector 'normal', if the light
// source is located at 'lightPos'.
//=======================================================================
float computeDiffuseIntens(in vec3 position, in vec3 normal, in vec3 lightPos)
{
	normal = normalize(normal);
	vec3 vecToLight = normalize(lightPos.xyz - position.xyz);
	
	float diffuseIntensity = dot(normal, vecToLight);
	
	diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);
	
	return diffuseIntensity;
}


//=======================================================================
// fragment program entry point
//=======================================================================
void main(void)
{
	// Do some simple diffuse shading
	float diffuseIntensity = computeDiffuseIntens(world_pos.xyz, world_normal.xyz, w_LightPosition.xyz);
	float ambientIntensity = 0.3;
	
	gl_FragColor = color*(diffuseIntensity + ambientIntensity);
}
