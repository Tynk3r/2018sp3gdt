
#include <iostream>
#include <fstream>
#include <GL\glew.h>

#include "LoadTGA.h"

#include "Vector3.h"

GLuint LoadTGA(const char *file_path)				// load TGA file to memory
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if(!fileStream.is_open()) {
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return 0;
	}

	GLubyte		header[ 18 ];									// first 6 useful header bytes
	GLuint		bytesPerPixel;								    // number of bytes per pixel in TGA gile
	GLuint		imageSize;									    // for setting memory
	GLubyte *	data;
	GLuint		texture = 0;
	unsigned	width, height;

	fileStream.read((char*)header, 18);
	width = header[12] + header[13] * 256;
	height = header[14] + header[15] * 256;

 	if(	width <= 0 ||								// is width <= 0
		height <= 0 ||								// is height <=0
		(header[16] != 24 && header[16] != 32))		// is TGA 24 or 32 Bit
	{
		fileStream.close();							// close file on failure
		std::cout << "File header error.\n";
		return 0;										
	}

	bytesPerPixel	= header[16] / 8;						//divide by 8 to get bytes per pixel
	imageSize		= width * height * bytesPerPixel;	// calculate memory required for TGA data
	
	data = new GLubyte[ imageSize ];
	fileStream.seekg(18, std::ios::beg);
	fileStream.read((char *)data, imageSize);
	fileStream.close();	

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if(bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	else //bytesPerPixel == 4
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	float maxAnisotropy = 1.f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenerateMipmap( GL_TEXTURE_2D );
	
	delete []data;

	return texture;						
}

GLuint PaintTGA(GLuint texture, float x, float y, Vector3 color, float alpha, float tgaLength)
{
	GLuint		bytesPerPixel;								    // number of bytes per pixel in TGA gile
	GLuint		imageSize;									    // for setting memory
	GLubyte *	data;
	unsigned	width, height;

	width = tgaLength;
	height = tgaLength;

	//Note : Each pixel contains the 4 variables RGBA
	bytesPerPixel = 4;					//divide by 8 to get bytes per pixel
	imageSize = width * height * bytesPerPixel;	// calculate memory required for TGA data

	data = new GLubyte[imageSize]; //'data' is a temporary storage of the texture info

	//target position of the texture to edit
	int targetX = (int)(x * width);
	int targetY = (int)(y * height);

	//keeping the target position within the texture ID size
	if (targetX >= width) targetX = width - 1;
	if (targetX < 0) targetX = 0;
	if (targetY >= height) targetY = height - 1;
	if (targetY < 0) targetY = 0;

	glBindTexture(GL_TEXTURE_2D, texture);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	data[targetY * (int)sqrt(imageSize) * 2 + targetX * 4] = color.z * 255;  //BLUE
	data[targetY * (int)sqrt(imageSize) * 2 + targetX * 4 + 1] = color.y * 255; //GREEN
	data[targetY * (int)sqrt(imageSize) * 2 + targetX * 4 + 2] = color.x * 255; ///RED
	data[targetY * (int)sqrt(imageSize) * 2 + targetX * 4 + 3] = alpha * 255; //ALPHA

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	float maxAnisotropy = 1.f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[]data;

	return texture;
}

GLuint PaintTGABurst(GLuint texture, float x, float y, Vector3 color, float alpha, float tgaLength, int spread)
{
	GLuint		bytesPerPixel;								    // number of bytes per pixel in TGA gile
	GLuint		imageSize;									    // for setting memory
	GLubyte *	data;
	unsigned	width, height;

	width = tgaLength;
	height = tgaLength;

	//Note : Each pixel contains the 4 variables RGBA
	bytesPerPixel = 4;					//divide by 8 to get bytes per pixel
	imageSize = width * height * bytesPerPixel;	// calculate memory required for TGA data

	data = new GLubyte[imageSize]; //'data' is a temporary storage of the texture info

								   //target position of the texture to edit
	int targetX = (int)(x * width);
	int targetY = (int)(y * height);

	//keeping the target position within the texture ID size
	if (targetX >= width) targetX = width - 1;
	if (targetX < 0) targetX = 0;
	if (targetY >= height) targetY = height - 1;
	if (targetY < 0) targetY = 0;

	glBindTexture(GL_TEXTURE_2D, texture);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	for (int tempY = -spread; tempY <= spread; ++tempY)
	{
		for (int tempX = -spread + abs(tempY); tempX <= spread - abs(tempY); ++tempX)
		{
			if (targetX + tempX < width && targetX + tempX >= 0 && targetY + tempY < height && targetY + tempY >= 0 )
			{
				data[(targetY + tempY) * (int)sqrt(imageSize) * 2 + (targetX + tempX) * 4] = color.z * 255;  //BLUE
				data[(targetY + tempY) * (int)sqrt(imageSize) * 2 + (targetX + tempX) * 4 + 1] = color.y * 255; //GREEN
				data[(targetY + tempY) * (int)sqrt(imageSize) * 2 + (targetX + tempX) * 4 + 2] = color.x * 255; ///RED
				data[(targetY + tempY) * (int)sqrt(imageSize) * 2 + (targetX + tempX) * 4 + 3] = alpha * 255; //ALPHA
			}
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	float maxAnisotropy = 1.f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[]data;

	return texture;
}

GLuint NewTGA(int size)
{
	GLuint		texture = 0;
	GLuint		bytesPerPixel;								    // number of bytes per pixel in TGA gile
	GLuint		imageSize;									    // for setting memory
	GLubyte *	data;
	unsigned	width, height;

	if (size <= 1) size = 1;
	else if (size <= 2) size = 2;
	else if (size <= 4) size = 4;
	else if (size <= 8) size = 8;
	else if (size <= 16) size = 16;
	else if (size <= 32) size = 32;
	else if (size <= 64) size = 64;
	else if (size <= 128) size = 128;
	else if (size <= 256) size = 256;
	else if (size <= 512) size = 512;
	else if (size <= 1024) size = 1024;
	else if (size <= 2048) size = 2048;
	else size = 4096;

	width = size;
	height = size;

	//Note : Each pixel contains the 4 variables RGBA
	bytesPerPixel = 4;					//divide by 8 to get bytes per pixel
	imageSize = width * height * bytesPerPixel;	// calculate memory required for TGA data

	data = new GLubyte[imageSize]; //'data' is a temporary storage of the texture info

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			data[y * (int)sqrt(imageSize) * 2 + x * 4] = 0;  //BLUE
			data[y * (int)sqrt(imageSize) * 2 + x * 4 + 1] = 0; //GREEN
			data[y * (int)sqrt(imageSize) * 2 + x * 4 + 2] = 0; ///RED
			data[y * (int)sqrt(imageSize) * 2 + x * 4 + 3] = 0; //ALPHA		
		}
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	float maxAnisotropy = 1.f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[]data;

	return texture;
}