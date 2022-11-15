#pragma once
#include "Vector4.h"
#include "Vector3.h"
class Light
{
public:
	Light() {};
	Light(const Vector3& position, const Vector4& diffuseColour,const Vector4& specularColour, const float radius) {
		this->position = position;
		this->diffuseColour = diffuseColour;
		this->specularColour = specularColour;
		this->radius = radius;
		intensity = 1;
	}
	~Light(void) {};
	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& val) { position = val; }

	float GetRadius()const { return radius; }
	void SetRadius(const float& val) { radius = val; }

	Vector4 GetDiffuseColour() const { return diffuseColour; }
	void SetDiffuseColour(const Vector4& val) { diffuseColour = val; }

	Vector4 GetSpecularColour() const { return specularColour; }
	void SetSpecularColour(const Vector4& val) { specularColour = val; }

	Vector3 position;
	float radius;
	Vector4 diffuseColour;
	Vector4 specularColour;
	float intensity;

protected:
	
};

