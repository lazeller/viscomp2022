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
	// my changes here
	double vec_comp_x = (a.x * b.x) - (a.y * b.y);
	double vec_comp_y = (a.x * b.y) + (a.y * b.x);
	// return vec2(0.0, 0.0);
	return vec2(vec_comp_x, vec_comp_y);
}

void main()
{
	int maxIter = 100;
	
	vec2 scale = vec2(4.0/3.0, 1.0) * zoom;
	
	vec2 c = (screenPos_out.xy - vec2(0.5, 0.5)) * scale - center;

	// EXERCISE 1:
	// implement mandelbrot fractal
	// my stuff
	vec2 z = c;
	int i = 0; 
	do {
		z = complexProduct(z,z)+c;
		i = i + 1;
	} while(((z.x*z.x)+(z.y*z.y)) <= 4.0 && i < maxIter);
	//  The function floatToColor outputs a vec3... I made color a vec3 as well
	// How is color usually defined???
	// or should color be gl_FragColor???
	vec3 color = floatToColor(i / maxIter);

	// EXERCISE 3
	// change alpha channel to add transparancy
    gl_FragColor.a = 1.0;
}
