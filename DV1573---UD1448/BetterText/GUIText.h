#ifndef _GUI_TEXT_H
#define _GUI_TEXT_H
#include <Pch/Pch.h>

class FontType;
class Camera;

class GUIText {

public:
	GUIText(const std::string& text, float fontSize, FontType* fontType, const glm::vec3& position, float maxLineLength, bool centered);
	~GUIText();

	void setColor(const glm::vec4& color);
	void remove();

	void setMeshInfo(int vao, int verticesCount);
	void setNumberOfLines(int number);
	void setScale(const float& scale);
	void setInitialScale(const float& scale);
	void setPosition(const glm::vec3 position);
	void rotateTowardsCamera(Camera* camera);
	void updateModelMatrix();
	void setAlpha(const float& alpha);
	void setToFaceCamera(bool condition);

	const unsigned int& getUniqueIndex() const;
	const int& getNumberOfLines() const;
	const unsigned int& getMeshVao() const;
	const int& getVertexCount() const;
	const float& getMaxLineSize() const;
	const float& getCurrentScale() const;
	const float& getInitialScale() const;
	const bool getFaceCamera() const;
	const float& getFontSize() const;
	const bool isCentered() const;

	const glm::vec3& getPosition() const;
	const glm::vec4& getColor() const;
	const glm::mat4& getModelMatrix() const;

	FontType* getFontType() const;

	const std::string& getText() const;

private:
	void createBuffers();
	void deleteBuffers();

private:
	FontType* m_fontType;
	unsigned int m_uniqueIndex;
	std::string m_text;
	glm::vec3 m_position;
	glm::vec4 m_color;

	float m_fontSize;
	float m_lineMaxSize;
	float m_scale;
	float m_initialScale;

	int m_numberOfLines;
	unsigned int m_textMeshVao;
	GLuint m_buffers[2];
	int m_vertexCount;

	bool m_faceCamera;
	bool m_centered;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_rotationMatrix;
	Timer m_timer;
};


#endif