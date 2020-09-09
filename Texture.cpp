#include "Texture.h"

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;

	int bitsize;
	int infosize;
	BITMAPFILEHEADER header;
	if (fopen_s(&fp, filename, "rb") != NULL) {   // ��Ʈ�� ������ ���̳ʸ� ���� ����. (return_value == NULL) �� ��� ���Ͽ��� ����.
		return NULL;
	}

	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)   // ��Ʈ���� ��� ������ �д´�.
	{
		fclose(fp);
		return NULL;
	}

	if (header.bfType != 'MB')      // ���� ��Ʈ���� �ƴ϶�� ������ �����Ѵ�.
	{
		fclose(fp);
		return NULL;
	}

	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);      // BITMAPINFO�� ��ġ�� ����Ѵ�.

	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL)
	{
		fclose(fp);
		return NULL;
	}

	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize)      // BITMAPINFO�� �д´�.
	{
		free(*info);
		fclose(fp);
		return NULL;
	}

	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)      // ��Ʈ�� �̹����� ũ�⸦ ����Ѵ�.
	{
		bitsize = ((*info)->bmiHeader.biWidth *
			(*info)->bmiHeader.biBitCount + 7) / 8 *
			abs((*info)->bmiHeader.biHeight);
	}

	if ((bits = (unsigned char*)malloc(bitsize)) == NULL)      // ��Ʈ�� �����͸� �а� ������ �ݴ´�.
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
	BITMAPINFO * bitInfo;   // ��Ʈ�� ��� ������ ����
	GLubyte * bitmap;		// ��Ʈ�� �����͸� ����ų ������

	glGenTextures(11, textureID);
	glBindTexture(GL_TEXTURE_2D, textureID[0]); //	����
	bitmap = LoadDIBitmap("Resources/TextureMapping/earth.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[1]); //	�¾�
	bitmap = LoadDIBitmap("Resources/TextureMapping/sun.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[2]); //	����
	bitmap = LoadDIBitmap("Resources/TextureMapping/mercury.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[3]); //	�ݼ�
	bitmap = LoadDIBitmap("Resources/TextureMapping/venus.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[4]); //	ȭ��
	bitmap = LoadDIBitmap("Resources/TextureMapping/mars.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[5]); //	��
	bitmap = LoadDIBitmap("Resources/TextureMapping/jupiter.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[6]); //	�伺
	bitmap = LoadDIBitmap("Resources/TextureMapping/saturn.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[7]); //	õ�ռ�
	bitmap = LoadDIBitmap("Resources/TextureMapping/uranus.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[8]); //	�ؿռ�
	bitmap = LoadDIBitmap("Resources/TextureMapping/neptune.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[9]); //	��
	bitmap = LoadDIBitmap("Resources/TextureMapping/moon.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[10]); //	���� ��� + ���ϼ�
	bitmap = LoadDIBitmap("Resources/TextureMapping/space.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID[11]); //	���� ����
	bitmap = LoadDIBitmap("Resources/TextureMapping/earth_cloud.bmp", &bitInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2048, 2048, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
