#include "Texture.h"

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;

	int bitsize;
	int infosize;
	BITMAPFILEHEADER header;
	if (fopen_s(&fp, filename, "rb") != NULL) {   // 비트맵 파일을 바이너리 모드로 연다. (return_value == NULL) 일 경우 파일열기 성공.
		return NULL;
	}

	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)   // 비트맵의 헤더 정보를 읽는다.
	{
		fclose(fp);
		return NULL;
	}

	if (header.bfType != 'MB')      // 만일 비트맵이 아니라면 실행을 중지한다.
	{
		fclose(fp);
		return NULL;
	}

	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);      // BITMAPINFO의 위치를 계산한다.

	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL)
	{
		fclose(fp);
		return NULL;
	}

	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize)      // BITMAPINFO를 읽는다.
	{
		free(*info);
		fclose(fp);
		return NULL;
	}

	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)      // 비트맵 이미지의 크기를 계산한다.
	{
		bitsize = ((*info)->bmiHeader.biWidth *
			(*info)->bmiHeader.biBitCount + 7) / 8 *
			abs((*info)->bmiHeader.biHeight);
	}

	if ((bits = (unsigned char*)malloc(bitsize)) == NULL)      // 비트맵 데이터를 읽고 파일을 닫는다.
	{
		free(*info);
		fclose(fp);
		return NULL;
	}

	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize)
	{
		free(*info);
		free(bits);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return bits;
}

void InitiateTexture(GLuint * textureID)
{
	BITMAPINFO * bitInfo;   // 비트맵 헤더 저장할 변수
	GLubyte * bitmap;		// 비트맵 데이터를 가리킬 포인터

	glGenTextures(11, textureID);
	glBindTexture(GL_TEXTURE_2D, textureID[0]); //	지구
	bitmap = LoadDIBitmap("Resources/TextureMapping/earth.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[1]); //	태양
	bitmap = LoadDIBitmap("Resources/TextureMapping/sun.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[2]); //	수성
	bitmap = LoadDIBitmap("Resources/TextureMapping/mercury.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[3]); //	금성
	bitmap = LoadDIBitmap("Resources/TextureMapping/venus.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[4]); //	화성
	bitmap = LoadDIBitmap("Resources/TextureMapping/mars.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[5]); //	목성
	bitmap = LoadDIBitmap("Resources/TextureMapping/jupiter.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[6]); //	토성
	bitmap = LoadDIBitmap("Resources/TextureMapping/saturn.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[7]); //	천왕성
	bitmap = LoadDIBitmap("Resources/TextureMapping/uranus.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[8]); //	해왕성
	bitmap = LoadDIBitmap("Resources/TextureMapping/neptune.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[9]); //	달
	bitmap = LoadDIBitmap("Resources/TextureMapping/moon.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[10]); //	우주 배경 + 은하수
	bitmap = LoadDIBitmap("Resources/TextureMapping/space.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[11]); //	지구 구름
	bitmap = LoadDIBitmap("Resources/TextureMapping/earth_cloud.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
