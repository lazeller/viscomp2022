#version 130

in vec4 screenPos_out;

uniform vec2 center;
uniform float zoom;

vec3 floatToColor(float x)
{
	if(x == 1.0)
		return vec3(0, 0, 0);

	if(x < 0.5)
	{
		float t = x * 2.0;
		return vec3(1.0, t, 0.0);
	}
	else
	{
		float t = (x - 0.5) * 2.0;
		return vec3(1.0 - t, 1.0, 0.0);
	}
}

// Computes c = a*b where a, b, and c are complex numbers.
// I.e. computes c such that:
// c.x + i*c.y = (a.x + i*a.y) * (b.x + i*b.y)
// where i = sqrt(-1)
vec2 complexProduct(vec2 a, vec2 b)
{
	// EXERCISE 1
	return vec2(a.x * b.x - a.y * b.y, a.y * b.x + a.x * b.y);
}

void main()
{
	int maxIter = 100;
	
	vec2 scale = vec2(4.0/3.0, 1.0) * zoom;
	
	vec2 c = (screenPos_out.xy - vec2(0.5, 0.5)) * scale - center;

	// EXERCISE 1:
	// implement mandelbrot fractal
	vec2 z = c;
	int i = 0;
	for (i=0; i<maxIter; ++i) {
		z = complexProduct(z, z) + c;
		if (dot(z, z) > 4.0)
			break;
	}
	gl_FragColor.rgb = floatToColor(float(i) / float(maxIter));
	
	// EXERCISE 3
	// change alpha channel to add transparancy
	float r = distance(screenPos_out.xy, vec2(0.5, 0.5));
	gl_FragColor.a = clamp(r*r*16.0 - 1.0, 0.0, 1.0);
}
