#version 130

in vec4 color_out; 

//=======================================================================
// Fragment program entry point
//=======================================================================
void main(void)
{
	gl_FragColor = color_out;
} 
