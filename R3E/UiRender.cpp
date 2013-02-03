#include "Global.h"

#include "Text.hpp"
#include "Font.hpp"
#include "Colour.hpp"
#include "UiRender.hpp"
#include "FontList.hpp"
#include "FileSystem.hpp"
#include "VertexTypes.hpp"
#include "ResourceManager.hpp"

UiRender::UiRender() : mOffsetX(0), mOffsetY(0) {}
UiRender::~UiRender(){}

void UiRender::ResetDrawColour(){
	if(memcmp(&mColour, &Colour::White, sizeof(Colour)) == 0) return;
	memcpy(&mColour, &Colour::White, sizeof(Colour));
	glColor4ubv((GLubyte*)&mColour);
}

void UiRender::SetDrawColour(Colour* colour){
	if(mColour.val == colour->val) return;
	mColour.val = colour->val;
	glColor4ubv((GLubyte*)&mColour);
}

void UiRender::SetDrawColour(unsigned char r, unsigned char g, unsigned char b){
	Colour colour(r, g, b);
	if(mColour.val == colour.val) return;

	mColour.val = colour.val;
	glColor4ubv((GLubyte*)&mColour);
}

void UiRender::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	Colour colour(r, g, b, a);
	if(mColour.val == colour.val) return;

	mColour.val = colour.val;
	glColor4ubv((GLubyte*)&mColour);
}

void UiRender::PushTranslate(int x, int y){
	mOffsetX += x;
	mOffsetY += y;
}

void UiRender::PopTranslate(int x, int y){
	mOffsetX -= x;
	mOffsetY -= y;
}

void UiRender::DrawText(IText* text, float depth){
	text->SetPosition(mOffsetX, mOffsetY);
	text->Render(depth);
}

void UiRender::DrawLine(int _x1, int _y1, int _x2, int _y2){
	float x1 = float(_x1 + mOffsetX),
		  x2 = float(_x2 + mOffsetX),
		  y1 = float(_y1 + mOffsetY),
		  y2 = float(_y2 + mOffsetY);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, -1.0f);
	glVertex3f(x2, y2, -1.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void UiRender::DrawRect(int _x1, int _y1, int _x2, int _y2){
	float x1 = float(_x1 + mOffsetX),
		  x2 = float(_x2 + mOffsetX),
		  y1 = float(_y1 + mOffsetY),
		  y2 = float(_y2 + mOffsetY);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glVertex3f(x1, y1, -1.0f);
	glVertex3f(x1, y2, -1.0f);
	glVertex3f(x2, y2, -1.0f);
	glVertex3f(x2, y1, -1.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void UiRender::DrawImage(IImage* image){
	static UV0_V3F vertices[] = {
		UV0_V3F(Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)),
	};

	SmartPointer<Texture> tex = image->mTex;

	float w = float(tex.Get()->GetWidth() + mOffsetX);
	float h = float(tex.Get()->GetHeight() + mOffsetY);
	float x = float(mOffsetX);
	float y = float(mOffsetY);

	vertices[0].pos.x = x;
	vertices[0].pos.y = y;

	vertices[1].pos.x = x;
	vertices[1].pos.y = h;

	vertices[2].pos.x = w;
	vertices[2].pos.y = h;

	vertices[3].pos.x = w;
	vertices[3].pos.y = y;

	glBindTexture(GL_TEXTURE_2D, tex.Get()->GetTextureID());
	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 1);
}

void UiRender::DrawImage(IImage* image, int x1, int y1, int x2, int y2, int width, int height){
	static UV0_V3F vertices[] = {
		UV0_V3F(Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)),
	};

	SmartPointer<Texture> tex = image->mTex;

	float tw = float(tex.Get()->GetWidth());
	float th = float(tex.Get()->GetHeight());

	float u1 = float(x1) / tw;
	float u2 = float(x2) / tw;

	float v1 = float(y1) / th;
	float v2 = float(y2) / th;

	float w = float(width + mOffsetX);
	float h = float(height + mOffsetY);

	float x = float(mOffsetX);
	float y = float(mOffsetY);

	vertices[0].uv0.x = u1;
	vertices[0].uv0.y = v1;
	vertices[0].pos.x = x;
	vertices[0].pos.y = y;

	vertices[1].uv0.x = u1;
	vertices[1].uv0.y = v2;
	vertices[1].pos.x = x;
	vertices[1].pos.y = h;

	vertices[2].uv0.x = u2;
	vertices[2].uv0.y = v2;
	vertices[2].pos.x = w;
	vertices[2].pos.y = h;

	vertices[3].uv0.x = u2;
	vertices[3].uv0.y = v1;
	vertices[3].pos.x = w;
	vertices[3].pos.y = y;

	glBindTexture(GL_TEXTURE_2D, tex.Get()->GetTextureID());
	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 4);
}

void UiRender::DrawImage(IImage* image, int x1, int y1, int x2, int y2){
	static UV0_V3F vertices[] = {
		UV0_V3F(Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)),
		UV0_V3F(Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f)),
	};

	SmartPointer<Texture> tex = image->mTex;

	float tw = float(tex.Get()->GetWidth());
	float th = float(tex.Get()->GetHeight());

	float u1 = float(x1) / tw;
	float u2 = float(x2) / tw;

	float v1 = float(y1) / th;
	float v2 = float(y2) / th;

	float w = float((x2 - x1) + mOffsetX);
	float h = float((y2 - y1) + mOffsetY);

	float x = float(mOffsetX);
	float y = float(mOffsetY);

	vertices[0].uv0.x = u1;
	vertices[0].uv0.y = v1;
	vertices[0].pos.x = x;
	vertices[0].pos.y = y;

	vertices[1].uv0.x = u1;
	vertices[1].uv0.y = v2;
	vertices[1].pos.x = x;
	vertices[1].pos.y = h;

	vertices[2].uv0.x = u2;
	vertices[2].uv0.y = v2;
	vertices[2].pos.x = w;
	vertices[2].pos.y = h;

	vertices[3].uv0.x = u2;
	vertices[3].uv0.y = v1;
	vertices[3].pos.x = w;
	vertices[3].pos.y = y;

	glBindTexture(GL_TEXTURE_2D, tex.Get()->GetTextureID());
	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 4);
}

IText* UiRender::CreateTextWithFont(int font){
	Text* text = new Text();
	text->SetFont(FontList::Instance().GetFont(font));
	return (IText*)text;
}

int UiRender::GetTextWidth(const char* text, int _font){
	SmartPointer<Font> font = FontList::Instance().GetFont(_font);
	return font->GetStringWidth(text);
}

IImage* UiRender::CreateImage(const char* path){
	return new IImage(TEX_MGR().Load(path));
}

void UiRender::FreeImage(IImage* image){
	delete image;
}

void UiRender::FreeText(IText* text){
	delete text;
}

File* UiRender::OpenFile(const char* path){
	return FILE_SYS()->OpenFile(path, "rb");
}