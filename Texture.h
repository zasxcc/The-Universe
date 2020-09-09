#pragma once
#include "LibraryHeader.h"

constexpr int MAX_TEXTURE{ 13 };
constexpr int MAX_TEXTURE_SPACEPROBE{ 10 };

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info);

void InitiateTexture(GLuint * textureID);