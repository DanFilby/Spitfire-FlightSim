#pragma once
#include <string>
#include <unordered_map>

#include "LoadShaders.hpp"
#include "Models.h"
#include "Utilities.hpp"

//how wide each character should be, cuts off unused space. as the font is left aligned in texture
const float FontWidthAdjust = 0.65f;

//fonts available
const enum Fonts{CalibriBold, CourierNew, CourierNewBold};

//font file name
const std::unordered_map<Fonts, std::string> FontMap{
	{Fonts::CalibriBold, "CalibriBoldFontSheet"}, {Fonts::CourierNew, "CourierNewFontSheet"}, {Fonts::CourierNewBold, "CourierNewBoldFontSheet"} };


//A UI text object, given an id and VAO. draws the text to screen
class Text {
public:
	Text(unsigned int _id, GLuint _VertexArrayID,int _triCount):
		id(_id), VertexArrayID(_VertexArrayID),triCount(_triCount){};

	//clears VAO and VBOs
	void ClearText() {
		Utilities::ClearVAO(VertexArrayID);
	};

	void Draw() {
		//for transparent background to text
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(VertexArrayID);
		glDrawArrays(GL_TRIANGLES, 0, triCount);
		glBindVertexArray(0);

		glDisable(GL_BLEND);
	};

	unsigned int id;

private:
	GLuint VertexArrayID;
	int triCount;
};

//class for adding and managing text on screen. needs a font sheet 
class UIText
{
public:
	UIText(Fonts font);
	~UIText();

	unsigned int  AddTextToScreen(std::string text, int x, int y, int size);
	void DrawAllText();
	void DeleteText(unsigned int id);

private:
	std::vector<Text> activeTexts {};

	GLuint shaderProgram;
	GLuint textureID;

	int idCount = 0;
};

inline UIText::UIText(Fonts font)
{
	std::string fontTexturePath = "../Assets/Fonts/" + FontMap.at(font) + ".png";

	shaderProgram = LoadShaders("TextVert.glsl", "TextFrag.glsl");
	textureID = TextureFromFile(fontTexturePath.c_str());
}

inline unsigned int UIText::AddTextToScreen(std::string text, int x, int y, int size)
{
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

	for (unsigned int i = 0; i < text.size(); i++) {
		float adjust = i * (size * (1.0f - FontWidthAdjust));
		float width = i * size - adjust;

		glm::vec2 vertex_up_left = glm::vec2(x + width, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + width + (size* FontWidthAdjust), y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + width + (size* FontWidthAdjust), y);
		glm::vec2 vertex_down_left = glm::vec2(x + width, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = ((character / 16) / 16.0f) - 0.125f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, 1.0f - uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + FontWidthAdjust / 16.0f, 1.0f - uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + FontWidthAdjust / 16.0f, 1.0f - (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, 1.0f - (uv_y + 1.0f / 16.0f));

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}


	GLuint VertexArrayId;
	glGenVertexArrays(1, &VertexArrayId);
	glBindVertexArray(VertexArrayId);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * text.size() * 6, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint UVbuffer;
	glGenBuffers(1, &UVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * text.size() * 6, &UVs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	Text textObj = Text(idCount, VertexArrayId, vertices.size());
	activeTexts.push_back(textObj);

	int uniformLoc = glGetUniformLocation(shaderProgram, "FontSheetTexture");
	glUniform1i(uniformLoc, 0);

	return idCount++;
}

inline void UIText::DrawAllText()
{
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);

	glBindTexture(GL_TEXTURE_2D, textureID);

	for (Text& text : activeTexts) {
		text.Draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}

inline void UIText::DeleteText(unsigned int id)
{
	//just add all other texts to this list
	std::vector<Text> tempList{};

	for (Text & text : activeTexts) {
		if (text.id != id) {
			tempList.push_back(text);
		}
		else {
			text.ClearText();
		}
	}

	activeTexts = tempList;
}

inline UIText::~UIText()
{
}