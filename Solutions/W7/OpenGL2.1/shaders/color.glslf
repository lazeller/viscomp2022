#version 120

varying vec4 color_out;

//=======================================================================
// Fragment program entry point
//=======================================================================
void main(void)
{
	gl_FragColor = color_out;
} 
