#include "stdafx.h"
#include <vector>
#include <cstring>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "DDsTexture.h"
#include "TextShader.h"
#include "Text2D.h"

namespace Dot {
	unsigned int Text2DTextureID;
	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;
	unsigned int Text2DShaderID;
	unsigned int Text2DUniformID;

	void InitText2D(const char* texturePath) {

		
	
		Text2DShaderID = LoadShaders("res/shaders/Text/TextShader.vs", "res/shaders/Text/TextShader.fs");

		// Initialize uniforms' IDs
		//Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");

	}

	void printText2D(const char* text, int x, int y, int size) {

	
		glUseProgram(Text2DShaderID);

		// Bind texture
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		//glUniform1i(Text2DUniformID, 0);

		// 1rst attribute buffer : vertices
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		//glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//glEnable(GL_BLEND);
		////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw call
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		//glDisable(GL_BLEND);

		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);

	}

	void cleanupText2D() {

		// Delete buffers
		glDeleteBuffers(1, &Text2DVertexBufferID);
		glDeleteBuffers(1, &Text2DUVBufferID);

		// Delete texture
		glDeleteTextures(1, &Text2DTextureID);

		// Delete shader
		glDeleteProgram(Text2DShaderID);
	}
}