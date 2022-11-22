#include "Light.h"
void Light::Update(float dt) {
	//return;
	if (this->rotationRadius == -1) {
		return;
	}
	angle += dt;
	position = Vector3(startPosition.x + sin(angle) * rotationRadius, position.y, startPosition.z + cos(angle) * rotationRadius);
	//position.x = startPosition.x + sin(angle) * rotationRadius;
	//position.z = startPos.z + cos(angle) * rotationRadius;
}