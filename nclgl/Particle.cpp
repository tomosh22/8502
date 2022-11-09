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

    //Mesh::BufferData();

};

