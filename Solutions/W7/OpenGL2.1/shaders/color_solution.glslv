#version 120

//=======================================================================
// Uniforms: Constant over all vertices
//=======================================================================
uniform vec4 lightPos;				// Position of light in model coordinates
uniform mat4 Model_mat; 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix

uniform vec3 InterestingPoint;      // The point whose neighborhod should be brighter


//=======================================================================
// Attributes: Per vertex data
//=======================================================================
attribute vec4 position;			// Vertex coordinates in model space
attribute vec4 color_in;			// Vertex color
attribute vec3 normal;				// Vertex normal


//=======================================================================
// Varying: Vertex shader output, input to rasterizer
//=======================================================================
varying vec4 color_out;				// shaded color for this vertex


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
// Vertex program entry point
//=======================================================================
void main(void)
{
	// Color used for Lighting
	vec4 c = color_in;
	vec4 white = vec4(1.0,1.0,1.0,1.0);

	// Calculate distance to point of intrest
	vec3 diff = position.xyz - InterestingPoint;
	float dist = dot(diff, diff);
	dist = clamp(dist / 400.0, 0.0, 1.0);
	
	// Linear interpolate between white and color depending on dist
	c = mix(white,c,dist);

	// Do some simple diffuse shading
	
	float diffuseIntensity = computeDiffuseIntens(position.xyz, normal, lightPos.xyz);
	float ambientIntensity = 0.3;
	
	color_out = c*(diffuseIntensity + ambientIntensity);
	
	// Project vertex coordinates to screen
    gl_Position = ProjectView_mat * Model_mat * position;
}