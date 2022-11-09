#include "Particle.h"
bool Particle::UpdateParticle(float dt) {
	elapsed += dt;
	if (elapsed >= duration)return false;
    velocity = velocity - gravity * dt;
	modelMatrix = modelMatrix * Matrix4::Translation(velocity*dt);
	if (!constColour) {
		colour.x = startColour.x + (endColour.x - startColour.x) * elapsed / duration;
		colour.y = startColour.y + (endColour.y - startColour.y) * elapsed / duration;
		colour.z = startColour.z + (endColour.z - startColour.z) * elapsed / duration;
		colour.w = startColour.w + (endColour.w - startColour.w) * elapsed / duration;
	}
    if (!constScale) {
        scale.x = startScale.x + (endScale.x - startScale.x) * elapsed / duration;
        scale.y = startScale.y + (endScale.y - startScale.y) * elapsed / duration;
        scale.z = startScale.z + (endScale.z - startScale.z) * elapsed / duration;
    }
	
}
Particle::Particle() {
    numVertices = 4;
    type = GL_TRIANGLE_STRIP;
    vertices = new Vector3[numVertices];
    textureCoords = new Vector2[numVertices];
    //colours = new Vector4[numVertices];

    vertices[0] = Vector3(-1.0f, 1.0f, 0.0f);
    vertices[1] = Vector3(-1.0f, -1.0f, 0.0f);
    vertices[2] = Vector3(1.0f, 1.0f, 0.0f);
    vertices[3] = Vector3(1.0f, -1.0f, 0.0f);

    textureCoords[0] = Vector2(0.0f, 1.0f);
    textureCoords[1] = Vector2(0.0f, 0.0f);
    textureCoords[2] = Vector2(1.0f, 1.0f);
    textureCoords[3] = Vector2(1.0f, 0.0f);

    colour = Vector4(1, 1, 1, 1);
    scale = Vector3(1, 1, 1);
    gravity = Vector3(0, 0, 0);
    this->modelMatrix = Matrix4::Translation(Vector3(0, 0, 0));
    duration = 999999;
    elapsed = 0;
    Mesh::BufferData();
}
Particle::Particle(
    const Vector3& position,
    const Vector3& velocity,
    const Vector3& gravity,
    const float& duration,
    const Vector4& startColour,
    const Vector4& endColour,
    const bool& constColour,
    const Vector3& startScale,
    const Vector3& endScale,
    const bool& constScale) {

    this->velocity = velocity;
    this->gravity = gravity;
    this->duration = duration;
    this->elapsed = 0;
    this->startColour = startColour;
    this->constColour = constColour;
    this->startScale = startScale;
    this->constScale = constScale;

    this->modelMatrix = Matrix4::Translation(position);

    numVertices = 4;
    type = GL_TRIANGLE_STRIP;
    vertices = new Vector3[numVertices];
    textureCoords = new Vector2[numVertices];
    //colours = new Vector4[numVertices];

    vertices[0] = Vector3(-1.0f, 1.0f, 0.0f);
    vertices[1] = Vector3(-1.0f, -1.0f, 0.0f);
    vertices[2] = Vector3(1.0f, 1.0f, 0.0f);
    vertices[3] = Vector3(1.0f, -1.0f, 0.0f);

    textureCoords[0] = Vector2(0.0f, 1.0f);
    textureCoords[1] = Vector2(0.0f, 0.0f);
    textureCoords[2] = Vector2(1.0f, 1.0f);
    textureCoords[3] = Vector2(1.0f, 0.0f);

    if (constColour) {
        colour = startColour;
    }
    else {
        this->endColour = endColour;
    }
    if (constScale) {
        scale = startScale;
    }
    else {
        this->endScale = endScale;
    }

    Mesh::BufferData();

};

GLuint Particle::CreateVBO() {
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER,20 * 4, GL_STREAM_DRAW);
    glBindVertexArray(arrayObject);
    return vbo;
}