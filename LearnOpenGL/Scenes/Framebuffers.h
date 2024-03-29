#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H

#include "Scene.h"
#include "Shader.h"

#include <string_view>

namespace Pink
{

	class Framebuffers : public Scene
	{
	public:
		Framebuffers();
		~Framebuffers();

		void draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);

	private:
		GLuint cubeTexture;
        GLuint floorTexture;
        GLuint fboTexture;

		GLuint cubeVAO;
		GLuint cubeVBO;
        GLuint floorVAO;
        GLuint floorVBO;
        GLuint quadVAO;
        GLuint quadVBO;

        GLuint fbo;
        GLuint rbo;
        
        Shader colorShader;
		Shader postProcessShader;
		Shader textureShader;

        const float cubeVertices[180] = {
            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-right         
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Top-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
            // Left face
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-right
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-right
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-right
            // Right face
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-left
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-right
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right         
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-right
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-left
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left     
             // Bottom face
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Top-right
              0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-left
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-left
             -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-right
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Top-right
             // Top face
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
              0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right     
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left
             -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // Bottom-left        
        };

        float floorVertices[30] = {
             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
             5.0f, -0.5f, -5.0f,  2.0f, 2.0f
        };

        const float quadVertices[24] = { 
            // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            
            // Positions   // Texture coordinates
            //0.5f, -0.5f,  0.0f, 1.0f, // Top-left
            //0.5f, -1.0f,  0.0f, 0.0f, // Bottom-left
            //1.0f, -1.0f,  1.0f, 0.0f, // Bottom-right

            //0.5f, -0.5f,  0.0f, 1.0f, // Top-left
            //1.0f, -1.0f,  1.0f, 0.0f, // Bottom-right
            //1.0f, -0.5f,  1.0f, 1.0f  // Top-right

            0.25f, -0.25f,  0.0f, 1.0f, // Top-left
            0.25f, -1.0f,   0.0f, 0.0f, // Bottom-left
            1.0f,  -1.0f,   1.0f, 0.0f, // Bottom-right

            0.25f, -0.25f,  0.0f, 1.0f, // Top-left
            1.0f,  -1.0f,   1.0f, 0.0f, // Bottom-right
            1.0f,  -0.25f,  1.0f, 1.0f  // Top-right
        };

        void createTexture(GLuint& texture, std::string_view filename);
        void drawScene(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
        void drawScenePostProcessed();
	};

}

#endif