// png2tex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "png.h"

#pragma comment(lib,"libpng.lib")
#pragma comment(lib,"zlib.lib")

/*

  Tex File Format
  --------------------

  width = 32 bit long
  height = 32 bit long
  alpha = 8 bit byte

*/
const int PNG_BYTES_TO_CHECK = 4;
const int ERROR = -1;

int main(int argc, char* argv[])
{
	char alpha;
	printf("\t\t\t\tPNG2TEX v0.4\n\t\t\t   Programmed by burak575\n\n");
	if ( argc < 2 )
	{
		printf("Usage: png2tex input.png");
		return 0;
	}
	char *pngpath;
	pngpath = argv[1];

	int NOT_PNG = -2;
	unsigned char* header;
	bool is_png;
	header = new unsigned char [8];
	
	FILE *fp = fopen(pngpath, "rb");
    if (!fp)
    {
		printf("File IO Error!");
        return 0;
    }
	unsigned char buf[PNG_BYTES_TO_CHECK];
	if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK) return 0;

    is_png = !png_sig_cmp(&buf[0], (png_size_t)0, PNG_BYTES_TO_CHECK);
    if (!is_png)
    {
        printf("File is not PNG!");
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
	{
        printf("Error!");
        return 0;
	}

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr,(png_infopp)NULL, (png_infopp)NULL);
        printf("Error!");
        return 0;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
        printf("Error!");
        return 0;
    }
    if (setjmp(png_ptr->jmpbuf)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        printf("Error!");
        return 0;
    }

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	png_bytep* potr;
	potr = png_get_rows(png_ptr, info_ptr);
	long width,height,bit_depth,color_type,channels,rowbytes;

	width = png_get_image_width(png_ptr,info_ptr);
    height = png_get_image_height(png_ptr,info_ptr);
    bit_depth = png_get_bit_depth(png_ptr,info_ptr);
    color_type = png_get_color_type(png_ptr,info_ptr);
	channels = png_get_channels(png_ptr, info_ptr);
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	if ( channels < 3 )
	{
		printf("Channels < 3!");
		return 0;
	}
	if ( color_type == PNG_COLOR_TYPE_RGB )
	{
		alpha = 0;	
	}
	if ( color_type == PNG_COLOR_TYPE_RGB_ALPHA )
	{
		alpha = 1;
	}
	if ( argc > 2 )
	{
		if ( strcmp(argv[2],"-i") == 0) printf("w: %i h: %i bits: %i alpha: %i channels: %i rowbytes: %i",width,height,bit_depth,alpha,channels,rowbytes);
	}
	char *texpath;
	texpath = new char [strlen(pngpath)+1];
	strncpy(texpath,pngpath,strlen(pngpath)+1);
	strncpy(texpath+strlen(pngpath)-3,"tex",3);
	//strcat(texpath,"tex\0");
	FILE *fo = fopen(texpath,"wb");
	fwrite(&width,4,1,fo);
	fwrite(&height,4,1,fo);
	fwrite(&alpha,1,1,fo);
	/*for (int i=height-1;i>-1;i--)
	{
		fwrite(potr[i],rowbytes,1,fo);
	}*/
	for (int i=0;i<height;i++)
	{
		fwrite(potr[i],rowbytes,1,fo);	
	}
	fclose(fo);
	printf("Conversion is completed!");

	return 0;
}
