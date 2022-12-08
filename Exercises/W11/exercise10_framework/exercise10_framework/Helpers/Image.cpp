/*
 *  image.cpp
 *  warp
 *
 *  Created by Philipp Krähenbühl on 25.09.08.
 *  Copyright 2008 ETH Zurich. All rights reserved.
 *
 */

#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include <gdiplus.h>

#undef min
#undef max
#include <algorithm>
#else
#include <png.h>
#endif

#include "Image.h"


#ifdef _WIN32
static Image::InitGDI gdi_init;

Image::InitGDI::InitGDI()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

Image::InitGDI::~InitGDI()
{
	if(gdiplusToken) {
		Gdiplus::GdiplusShutdown(gdiplusToken);
		gdiplusToken = 0;
	}
}

#endif

Image::Image(const char * png_image ):data_(1),w_(0),h_(0), m_filename(png_image)
{
#ifdef _WIN32
    ULONG len1;
    WCHAR *wname;
    Gdiplus::Status status;

    len1 = MultiByteToWideChar(CP_ACP, 0, png_image, -1, NULL, 0);
    if(!len1) return;
    wname = new WCHAR[len1];
    MultiByteToWideChar(CP_ACP, 0, png_image, -1, wname, len1);

    Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(wname);
    Gdiplus::BitmapData* bitmapData = new Gdiplus::BitmapData;
    w_ = bitmap->GetWidth();
    h_ = bitmap->GetHeight();

    Gdiplus::Rect rect(0,0,w_, h_);

    // Lock a rectangular portion of the bitmap for reading.
    // convert to 24RGB format
   status = bitmap->LockBits(
                            &rect,
                            Gdiplus::ImageLockModeRead,
                            PixelFormat24bppRGB,
                            bitmapData
                            );
   if(status != Gdiplus::Ok) {
       delete bitmapData;
       delete bitmap;
       delete [] wname;
	   w_ = 0;
	   h_ = 0;
	   return;
   }

    // Copy the pixel data
    unsigned char* pixels = (unsigned char*)bitmapData->Scan0;
	int stride = bitmapData->Stride;
	int bytepp = (0xFF&(bitmapData->PixelFormat >> 8)) / 8;
	assert((0xFF&(bitmapData->PixelFormat >> 8)) % 8 == 0); //we have a 4bit or 1 bit per pixel picture ihhh
	stride = stride <0?-stride:stride;
	data_.resize( w_ * h_ * 3 );

    //memcpy(data_.get(),pixels, w_ * h_ * 3);
    //windows lies its BGR not RGB as askes therefore we have to convert
    //make faster if needed
    unsigned char * p = &data_[0];
	for(int y = 0; y < h_; y++) {
		pixels = (unsigned char*)bitmapData->Scan0 + y*stride;
		for(int x = 0; x < w_; x++) {
			p[0] = pixels[2];
			p[1] = pixels[1];
			p[2] = pixels[0];
			p+=3; pixels+=bytepp;
		}
	}

    bitmap->UnlockBits(bitmapData);

    delete bitmapData;
    delete bitmap;
    delete [] wname;

#else
	FILE * in = fopen( png_image, "rb" );
	if (!in) return;

	char sig[8];
	fread(sig, 1, 8, in );
	if (!png_check_sig((unsigned char*)sig, 8)){ fclose(in); return; }

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr){ fclose(in); return; }

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr){png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL); fclose(in); return; }

	// Now comes the uglyness of libpng!
	if (setjmp(png_jmpbuf(png_ptr))) {	png_destroy_read_struct(&png_ptr, &info_ptr, NULL); fclose(in); return; }

	png_init_io(png_ptr, in);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);
	int bit_depth, color_type;
	png_uint_32 w,h;
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth,
				 &color_type, NULL, NULL, NULL);
	w_ = w;
	h_ = h;
	if (color_type & PNG_COLOR_MASK_ALPHA) png_set_strip_alpha(png_ptr);
	if (bit_depth > 8) png_set_strip_16(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	unsigned int rb = png_get_rowbytes(png_ptr, info_ptr);
	data_.resize(rb * h );

	png_bytepp row_pointers = new png_bytep[h];

	if (setjmp(png_jmpbuf(png_ptr))) {
		delete[] row_pointers;
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(in);
		return;
	}

	for (unsigned int i=0; i<h; i++) row_pointers[i] = &data_[0] + i*rb;
	png_read_image(png_ptr, row_pointers);

	delete[] row_pointers;
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose( in );
#endif
}

Image::Image(int W, int H, const float *data, int components /*= 3 */, IMAGE_PROCESS mode /*= IMAGE_PROCESS_NONE*/)
:	data_( W*H*3 ),
	w_(W),
	h_(H),
	m_filename("")
{
	assert(data);
	assert(components > 0 && components <= 4);

	switch(mode) {
		case IMAGE_PROCESS_NONE:
			for(int i = 0, j = 0; i < w_*h_*3; i += 3, j += components) {
				data_[i + 0] = (unsigned char)(data[j + 0] * 255.0f);
				data_[i + 1] = (unsigned char)(data[j + 1] * 255.0f);
				data_[i + 2] = (unsigned char)(data[j + 2] * 255.0f);
			}
			break;
		case IMAGE_PROCESS_ALPHA_DIVIDE:
			assert(components == 4);
			for(int i = 0, j = 0; i < w_*h_*3; i += 3, j += components) {
				data_[i + 0] = (unsigned char)(std::min(1.0f,data[j + 0]/data[j + 3]) * 255.0f);
				data_[i + 1] = (unsigned char)(std::min(1.0f,data[j + 1]/data[j + 3]) * 255.0f);
				data_[i + 2] = (unsigned char)(std::min(1.0f,data[j + 2]/data[j + 3]) * 255.0f);
			}
			break;
		default:
			assert(!"not implemented");
	}

}

Image::Image(int W, int H, const unsigned char *data, int components /*= 3 */,  IMAGE_PROCESS mode /*= IMAGE_PROCESS_NONE*/)
:	data_( W*H*3 ),
	w_(W),
	h_(H),
	m_filename("")
{
	assert(data);
	assert(components > 0 && components <= 4);

	switch(mode) {
		case IMAGE_PROCESS_NONE:
			for(int i = 0, j = 0; i < w_*h_*3; i += 3, j += components) {
				data_[i + 0] = (unsigned char)(data[j + 0]);
				data_[i + 1] = (unsigned char)(data[j + 1]);
				data_[i + 2] = (unsigned char)(data[j + 2]);
			}
			break;
		case IMAGE_PROCESS_ALPHA_DIVIDE:
			assert(components == 4);
			for(int i = 0, j = 0; i < w_*h_*3; i += 3, j += components) {
				data_[i + 0] = (unsigned char)(data[j + 0]/data[j + 3]);
				data_[i + 1] = (unsigned char)(data[j + 1]/data[j + 3]);
				data_[i + 2] = (unsigned char)(data[j + 2]/data[j + 3]);
			}
			break;
		default:
			assert(!"not implemented");
	}
}

const unsigned char * Image::getData() const
{
	return &data_[0];
}

#ifdef _WIN32
int GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	static CLSID ret;
	static WCHAR oldf[30] = {0};
	////speed up
	//if(wcscmp(oldf, format) == 0) {
	//	*pClsid = ret;
	//	return 1;
	//}
	wcscpy_s(oldf, 30*sizeof(WCHAR), format);

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			ret = pImageCodecInfo[j].Clsid;
			*pClsid = ret;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}
#endif

bool Image::save(const char *png_image, bool flip)
{
#ifdef _WIN32
	/*convert filename */
	ULONG len1;
	WCHAR *wname;
	Gdiplus::Status status;

	len1 = MultiByteToWideChar(CP_ACP, 0, png_image, -1, NULL, 0);
	if(!len1) return false;
	wname = new WCHAR[len1];
	MultiByteToWideChar(CP_ACP, 0, png_image, -1, wname, len1);

	int stride = ((3*sizeof(unsigned char) * w_ + 3) & -4);

	unsigned char *base = new unsigned char[stride*h_];
	unsigned char * pixels = &data_[0];
	unsigned char *p;

	int y, yend, ystep;
	if(flip) {
		y = h_ -1;
		yend = 0;
		ystep = -1;
	} else {
		y = 0;
		yend = h_- 1;
		ystep = 1;
	}
	for(; y != yend; y+=ystep) {
		p = (unsigned char*)base + y*stride;
		for(int x = 0; x < w_; x++) {
			p[0] = pixels[2];
			p[1] = pixels[1];
			p[2] = pixels[0];
			p+=3; pixels+=3;
		}
	}

	Gdiplus::Bitmap *bitmap = new Gdiplus::Bitmap(w_, h_, stride, PixelFormat24bppRGB, base );

	 CLSID pngClsid;
	 GetEncoderClsid(L"image/png", &pngClsid);
	 status = bitmap->Save(wname, &pngClsid);

	 if(status != Gdiplus::Ok) {
		 // Retrieve the system error message for the last-error code

		 LPTSTR pszMessage;
		 DWORD dwLastError = GetLastError();

		 FormatMessage(
			 FORMAT_MESSAGE_ALLOCATE_BUFFER |
			 FORMAT_MESSAGE_FROM_SYSTEM |
			 FORMAT_MESSAGE_IGNORE_INSERTS,
			 NULL,
			 dwLastError,
			 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			 (LPTSTR)&pszMessage,
			 0, NULL );

		 // Display the error message and exit the process

		 wprintf(L"failed with error %d: %s\n", dwLastError, pszMessage);

		 LocalFree(pszMessage);
	 }

	 delete[] wname;
	 delete bitmap;
	 delete[] base;

	 return status == Gdiplus::Ok;

#else
	FILE *fp;
	png_struct *png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
		return (1);

	png_infop info_ptr = png_create_info_struct (png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_write_struct (&png_ptr, (png_infopp)NULL);
		return (1);
	}
	png_bytepp bitmap = new png_bytep[h_];
	for (int i = 0; i < h_; i++)
		bitmap[i] = (png_bytep)&data_[0] + (i*3*w_);

	fp = fopen(png_image,"w");
	if (fp==NULL)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		delete[] bitmap;
		return -1;
	}
	png_init_io(png_ptr, fp);


	png_set_IHDR(png_ptr, info_ptr, w_, h_, 8, PNG_COLOR_TYPE_RGB , PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, bitmap);
	png_write_end(png_ptr, NULL);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	delete[] bitmap;


	fclose(fp);
	return (0);
#endif
}

bool Image::fromFramebuffer(int fbo /*= -1*/)
{
	int currentfbo = -1;
	//remove errors;
	glGetError();
	if(fbo != -1) {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentfbo);
		if(currentfbo == fbo)
			currentfbo  = -1;
		else
			glBindBuffer(GL_FRAMEBUFFER, fbo);
	}

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0,0,w_,h_, GL_RGB, GL_UNSIGNED_BYTE, &data_[0]);

	if(currentfbo != -1)
		glBindBuffer(GL_FRAMEBUFFER, currentfbo);

	return glGetError() == GL_NO_ERROR;
}
