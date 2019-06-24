#pragma once
#include "stdafx.h"

class Texture
{
private:
	int m_Width;
	int m_Height;
	GLuint m_Texture;
	GLubyte* m_Texture_Bits;
	BITMAPINFO* m_Bitmap;

public:
	int const Get_Width() { return m_Width; }
	int const Get_Height() { return m_Height; }

private:
	GLubyte * Load_BMP_File(const char* image_file_path, BITMAPINFO** info)
	{
		FILE* fp; GLubyte* dest; int bitsize, infosize; BITMAPFILEHEADER header;

		// 바이너리 읽기 모드로 파일을 연다
		if (fopen_s(&fp, image_file_path, "rb") != 0) return nullptr;

		// 비트맵 파일 헤더를 읽는다.
		if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) { fclose(fp); return nullptr; }

		// 파일이 BMP 파일인지 확인한다.
		if (header.bfType != 'MB') { fclose(fp); return nullptr; }

		// BITMAPINFOHEADER 위치로 간다.
		infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);

		// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
		if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) { fclose(fp); exit(0); return nullptr; }

		// 비트맵 인포 헤더를 읽는다.
		if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) { free(*info); fclose(fp); return nullptr; }

		// 비트맵의 크기 설정
		if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0) bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8.0 *  abs((*info)->bmiHeader.biHeight);

		// 비트맵의 크기만큼 메모리를 할당한다. 
		if ((dest = (unsigned char*)malloc(bitsize)) == NULL) { free(*info); fclose(fp); return nullptr; }

		// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
		if (fread(dest, 1, bitsize, fp) < (unsigned int)bitsize) { free(*info); delete dest; fclose(fp); return nullptr; }

		fclose(fp);

		return dest;
	}

public:
	Texture() { m_Texture_Bits = new GLubyte(); }
	~Texture() { if (m_Texture_Bits != nullptr) delete m_Texture_Bits; }

	void Set_Texture(const char* image_file_path, const int& image_Width, const int& image_Height)
	{
		if ((m_Texture_Bits = Load_BMP_File(image_file_path, &m_Bitmap)) != nullptr)
		{
			glGenTextures(1, &m_Texture);
			glBindTexture(GL_TEXTURE_2D, m_Texture);
			
			glTexImage2D(GL_TEXTURE_2D, 0, 3, image_Width, image_Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_Texture_Bits);

			m_Width = image_Width; m_Height = image_Height;

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}

	void Mapping_Texture(const bool& using_Sphere_Auto_Mapping)
	{
		glEnable(GL_TEXTURE_2D);

		if (using_Sphere_Auto_Mapping)
		{
			glEnable(GL_TEXTURE_GEN_S); 
			glEnable(GL_TEXTURE_GEN_T);

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); 
			glBindTexture(GL_TEXTURE_2D, m_Texture);
		}
		else glBindTexture(GL_TEXTURE_2D, m_Texture);
	}

	void Mapping_Off(const bool& used_Auto_Mapping)
	{
		if (used_Auto_Mapping ) { glDisable(GL_TEXTURE_GEN_S); glDisable(GL_TEXTURE_GEN_T); }
		glDisable(GL_TEXTURE_2D);
	}

};