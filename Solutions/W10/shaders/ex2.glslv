#version 130

in vec4 pos;

out vec4 screenPos_out;

// EXERCISE 2:
uniform vec3 posObject;
uniform float time;

void main()
{
	vec4 posVtx = pos;
	
	// EXERCISE 2:
	// Ripple vertices in x and y direction:
    posVtx.x += pow(cos(posVtx.y * 8.0 + time * 10.0), 2.0) * 0.1;
	posVtx.y += pow(sin(posVtx.x * 8.0 + time * 10.0), 2.0) * 0.1;

	// EXERCISE 2:
	// Scale to 30% of original size:
    posVtx.xy *= 0.3;

	// EXERCISE 2:
	// Translate:
    posVtx.xyz += posObject;
	
	// Set w and z so w-division will result in: (x/z, y/z, -0.1/z, 1)
	// This gives perspective distortion and depth-values in [-1, 1]
	if(posVtx.z != 0.0) {
		posVtx.w = posVtx.z;
		posVtx.z = -0.1;
	}
		
	gl_Position = posVtx;
	
	screenPos_out = pos * vec4(0.5, 0.5, 0, 0) + vec4(0.5, 0.5, 0, 0);
}
