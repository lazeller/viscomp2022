/*
 *  image.h
 *  warp
 *
 *  Created by Philipp Krähenbühl on 25.09.08.
 *  Copyright 2008 ETH Zurich. All rights reserved.
 *
 */
#ifndef IMAGE_H
#define IMAGE_H

#if _WIN32
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#endif

#include <vector>
#include <string>

#include "opengl.h"

enum IMAGE_PROCESS {
	IMAGE_PROCESS_NONE = 0,
	IMAGE_PROCESS_ALPHA_DIVIDE = 1
} ;

class Image
{
private:
	std::vector<unsigned char> data_;
	int w_;
	int h_;

public:
	Image(int W, int H ) : data_( W*H*3 ), w_(W), h_(H), m_filename("") {}
	Image(int W, int H, const float *data, int components = 3, IMAGE_PROCESS mode = IMAGE_PROCESS_NONE);
	Image(int W, int H, const unsigned char *data, int components = 3,  IMAGE_PROCESS mode = IMAGE_PROCESS_NONE);

	Image(const char * png_image);

	bool fromFramebuffer(int fbo = -1);

	bool save(const char *png_image, bool flip = false);
	
	int getW() const { return w_; }
	int getH() const { return h_; }
	const unsigned char * getData() const;
	const std::string getFilename() const { return m_filename; }

private:
	std::string m_filename;

#if _WIN32
public:
	struct InitGDI {
		InitGDI();
		~InitGDI();
		
		ULONG_PTR gdiplusToken;
	};
private:
	static InitGDI gdi_init;
#endif
};

#endif
