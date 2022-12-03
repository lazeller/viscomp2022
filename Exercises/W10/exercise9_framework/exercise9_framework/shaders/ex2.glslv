#version 130

in vec4 pos;

out vec4 screenPos_out;

// EXERCISE 2:
// new position vecotr
// in, out or inout???
vec3 posObject; 

void main()
{
	vec4 posVtx = pos;
	
	// EXERCISE 2:
	// Ripple vertices in x and y direction:

	// EXERCISE 2:
	// Scale to 30% of original size:

	// EXERCISE 2:
	// Translate:
	
	// Set w and z so w-division will result in: (x/z, y/z, -0.1/z, 1)
	// This gives perspective distortion and depth-values in [-1, 1]
	if(posVtx.z != 0.0) {
		posVtx.w = posVtx.z;
		posVtx.z = -0.1;
	}
		
	gl_Position = posVtx;
	
	screenPos_out = pos * vec4(0.5, 0.5, 0, 0) + vec4(0.5, 0.5, 0, 0);
}
