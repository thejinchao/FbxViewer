/***************************************************

    FBX Viewer

    (C) Copyright  JinChao. 2014
***************************************************/

/****************************************************************************************

Copyright (C) 2014 Autodesk, Inc.
All rights reserved.

Use of this software is subject to the terms of the Autodesk license agreement
provided at the time of installation or download, or which otherwise accompanies
this software in either electronic or hard copy form.

****************************************************************************************/
#include "StdAfx.h"

#include "FV_DrawText.h"

namespace FbxViewer
{
	#include "FV_ASCII_TEXTURE_DATA.inc"

	namespace
	{
		const float DEFAULT_POINT_SIZE = 20.f;
		const float DEFAULT_GAP = 0.f;

		// The font texture contains all printable ASCII (0x20-0x7E) in value order.
		const int TEXTURE_WIDTH = 256;
		const int TEXTURE_HEIGHT = 256;
		const char TEXTURE_MIN_GLYPH = 0x20;
		const char TEXTURE_MAX_GLYPH = 0x7E;
		const int TEXTURE_GLYPH_COUNT = TEXTURE_MAX_GLYPH - TEXTURE_MIN_GLYPH + 1;
	}

	DrawText::DrawText() : mGlyph(NULL), mTextureName(0), mPointSize(DEFAULT_POINT_SIZE), mGap(DEFAULT_GAP)
	{
		Initialize();
	}

	DrawText::~DrawText()
	{
		delete[] mGlyph;

		glDeleteTextures(1, &mTextureName);
	}

	void DrawText::Display(const char * pText)
	{
		if (!mGlyph || mTextureName == 0)
			return;

		// Push OpenGL attributes.
		glPushAttrib(GL_ENABLE_BIT);
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		glPushAttrib(GL_TEXTURE_BIT);

		glBindTexture(GL_TEXTURE_2D, mTextureName);
		glEnable(GL_TEXTURE_2D);

		// Blend with background color
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Visible for double side
		glDisable(GL_CULL_FACE);

		// Blend with foreground color
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glPushMatrix();
		const char * pCharacter = pText;
		while (*pCharacter != '\0')
		{
			if (*pCharacter == '\n')
			{
				glPopMatrix();
				// Move to the next line, left align
				glTranslatef(0, -mPointSize, 0);
				glPushMatrix();
			}
			else
			{
				Glyph * lGlyph = mGlyph + (*pCharacter - TEXTURE_MIN_GLYPH);
				if (lGlyph)
				{
					// Draw a rectangle with texture, alpha on.
					glBegin(GL_POLYGON);
					glTexCoord2f(lGlyph->texture_left, lGlyph->texture_bottom);
					glVertex2f(lGlyph->vertex_left * mPointSize, lGlyph->vertex_bottom * mPointSize);
					glTexCoord2f(lGlyph->texture_right, lGlyph->texture_bottom);
					glVertex2f(lGlyph->vertex_right * mPointSize, lGlyph->vertex_bottom * mPointSize);
					glTexCoord2f(lGlyph->texture_right, lGlyph->texture_top);
					glVertex2f(lGlyph->vertex_right * mPointSize, lGlyph->vertex_top * mPointSize);
					glTexCoord2f(lGlyph->texture_left, lGlyph->texture_top);
					glVertex2f(lGlyph->vertex_left * mPointSize, lGlyph->vertex_top * mPointSize);
					glEnd();

					const float advance = lGlyph->advance * mPointSize + mGap;
					glTranslatef(advance, 0, 0);
				}
				else
				{
					FBXSDK_printf("Invalid character: %c.\n", *pCharacter);
				}

			}
			++pCharacter;
		}
		glPopMatrix();

		// Pop OpenGL attributes.
		glPopAttrib();
		glPopAttrib();
		glPopAttrib();
	}

	void DrawText::Initialize()
	{
		// Load the texture coordinates and vertex coordinates.
		mGlyph = new Glyph[TEXTURE_GLYPH_COUNT];
		memcpy(mGlyph, FBX_ASCII_TEXTURE, sizeof(Glyph)*TEXTURE_GLYPH_COUNT);

		// Load the texture data, two bytes every pixel.
		// One byte for luminance, always be 0xFF; The other for alpha.
		const int lTextureSize = TEXTURE_WIDTH * TEXTURE_HEIGHT * 2;
		unsigned char * lTextureData = new unsigned char[lTextureSize];
		memcpy(lTextureData, FBX_ASCII_TEXTURE + sizeof(Glyph)*TEXTURE_GLYPH_COUNT, lTextureSize);

		// Save the texture data in GPU
		glGenTextures(1, &mTextureName);
		glBindTexture(GL_TEXTURE_2D, mTextureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
			GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, lTextureData);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		delete[] lTextureData;
	}

}
