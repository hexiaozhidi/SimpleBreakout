#include "ParticleFlowRenderer.h"
#include "../ResourceManager.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

ParticleFlowRenderer::ParticleFlowRenderer(const int &screenWidth, const int &screenHeight) :
    particleFlowShader(ResourceManager::getShader("particle_flow")),
    projection(glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f)),
    vao(0), vbo(0), vboInstancedTextureIndices(0), textureIndices(NUM_PARTICLES_MAX, 0),
    vboInstancedModels(0), models(NUM_PARTICLES_MAX, glm::mat4(0.0f)),
    vboInstancedTintColors(0), tintColors(NUM_PARTICLES_MAX, Palette::getColorByName("white")), ebo(0) {

    particleFlowShader.use().setMat4("projection", projection);

    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    std::vector<int> indices = { 0, 1, 2, 3, 2, 1 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vboInstancedTextureIndices);
    glGenBuffers(1, &vboInstancedModels);
    glGenBuffers(1, &vboInstancedTintColors);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, vboInstancedTextureIndices);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES_MAX * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(int), (void *)0);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, vboInstancedModels);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES_MAX * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, vboInstancedTintColors);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES_MAX * sizeof(Color), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Color), (void *)0);
    glVertexAttribDivisor(7, 1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

ParticleFlowRenderer::~ParticleFlowRenderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vboInstancedTextureIndices);
    glDeleteBuffers(1, &vboInstancedModels);
    glDeleteBuffers(1, &vboInstancedTintColors);
    glDeleteBuffers(1, &ebo);
}

void ParticleFlowRenderer::draw(const ParticleFlow &particleFlow) {
    const std::vector<Texture> &textures = particleFlow.getTextures();
    int numTextures = textures.size(), numAliveParticles = 0;

    for (const Particle &particle : particleFlow.getParticles())
        if (particle.getLifeCountdown() > 0.0f) {
            int textureId = particle.getTexture().getId();
            for (int i = 0; i < numTextures; ++i)
                if (textures[i].getId() == textureId) {
                    textureIndices[numAliveParticles] = i;
                    break;
                }

            glm::mat4 &model = models[numAliveParticles];
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(particle.getPosition(), 0.0f)); // step 2: move the particle to the target position
            model = glm::scale(model, glm::vec3(particle.getSize(), 1.0f)); // step 1: scale up the particle to the target size


            tintColors[numAliveParticles] = particle.getTintColor();
            tintColors[numAliveParticles].a = particle.getLifeCountdown() / particleFlow.getLife();
            ++numAliveParticles;
        }

    if (numAliveParticles) {
        glBindBuffer(GL_ARRAY_BUFFER, vboInstancedTextureIndices);
        glBufferSubData(GL_ARRAY_BUFFER, 0, numAliveParticles * sizeof(int), &textureIndices[0]);

        glBindBuffer(GL_ARRAY_BUFFER, vboInstancedModels);
        glBufferSubData(GL_ARRAY_BUFFER, 0, numAliveParticles * sizeof(glm::mat4), &models[0]);

        glBindBuffer(GL_ARRAY_BUFFER, vboInstancedTintColors);
        glBufferSubData(GL_ARRAY_BUFFER, 0, numAliveParticles * sizeof(Color), &tintColors[0]);

        std::vector<int> particleTextures(numTextures);
        for (int i = 0; i < numTextures; ++i) {
            particleTextures[i] = i;
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i].getId());
        }

        particleFlowShader.use().setInt("particleTexture", particleTextures);

        //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBindVertexArray(vao);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, numAliveParticles);
        glBindVertexArray(0);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}