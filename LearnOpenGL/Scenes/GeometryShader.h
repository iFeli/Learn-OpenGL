#ifndef GEOMETRY_SHADER_H
#define GEOMETRY_SHADER_H

#include "Scene.h"
#include "Shader.h"

namespace Pink
{

    class GeometryShader : public Scene
    {
    public:
        GeometryShader();
        ~GeometryShader();

        void draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);

    private:
        GLuint pointsVAO;
        GLuint pointsVBO;
        GLuint matricesUBO;

        Shader shader;

        float pointsVertices[8] = {
            -0.5f,  0.5f, // Top-left.
             0.5f,  0.5f, // Top-right.
             0.5f, -0.5f, // Bottom-right.
            -0.5f, -0.5f  // Bottom-left.
        };
    };

}

#endif