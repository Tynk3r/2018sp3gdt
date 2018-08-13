#include "SpriteAnimation.h"
#include "GL\glew.h"
#include "Vertex.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
	: Mesh(meshName)
	, m_row(row)
	, m_col(col)
	, m_currentTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
{
	if (m_anim->animActive == true)
	{
		// Get Animation current time, m_currentTime. 
		//Animation current time increments with dt
		m_currentTime += static_cast <float> (dt);

		//find int number of frames
		int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1); //return largest value, +1 cause start from 0. If number gotten is smaller than 1, will just take 1 and will just show whole strip. just check in case

																				//get float time frametime
		float frameTime = m_anim->animTime / numFrame; //find the frametime for each frame, cfause whole anaimtion have a set time

													   //get animation current frame
		m_currentFrame = Math::Min(m_anim->endFrame, m_anim->startFrame + static_cast <int> (m_currentTime / frameTime));

		//current time / framtime = delta time, cause every frame has a certain frame its funning, we need to get when its going to finish and when the next one will happen
		//ig want get current frame must get from there

		// Check if m_currentTime is greater than or equal animTime 
		if (m_currentTime >= m_anim->animTime)
		{
			if (m_anim->repeatCount == 0) //check if its finishing on based on the current time
			{
				m_anim->animActive = false;
				m_currentTime = 0.f;
				//if not repeating, just restart
				m_currentFrame = m_anim->startFrame;
				m_anim->ended = true;
			}
			if (m_anim->repeatCount == 1)
			{
				//reset m_currentTime  set m_currentFrame = startFrame
				m_currentTime = 0.0f;
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}

void SpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	//if(textureID > 0)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}


	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == DRAW_LINES)

		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	else if (mode == DRAW_TRIANGLE_STRIP)

		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	else   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//if(textureID > 0)
	{
		glDisableVertexAttribArray(3);
	}
}