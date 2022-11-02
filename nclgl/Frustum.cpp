#include "Frustum.h"
#include "SceneNode.h"
#include "Matrix4.h"
bool Frustum::InsideFrustum(SceneNode& n) {
	for (int x = 0; x < 6; x++) {
		if (!planes[x].SphereInPlane(n.GetWorldTransform().GetPositionVector(), n.GetBoundingRadius()))return false;
	}
	return true;
}

void Frustum::FromMatrix(const Matrix4& mat) {
	Vector3 xAxis = Vector3(mat.values[0], mat.values[4], mat.values[8]);
	Vector3 yAxis = Vector3(mat.values[1], mat.values[5], mat.values[9]);
	Vector3 zAxis = Vector3(mat.values[2], mat.values[6], mat.values[10]);
	Vector3 wAxis = Vector3(mat.values[3], mat.values[7], mat.values[11]);
	planes[0] = Plane(wAxis - xAxis, mat.values[15] - mat.values[12],true);
	planes[1] = Plane(wAxis + xAxis, mat.values[15] + mat.values[12],true);
	planes[2] = Plane(wAxis + yAxis, mat.values[15] + mat.values[13],true);
	planes[3] = Plane(wAxis - yAxis, mat.values[15] - mat.values[13],true);
	planes[4] = Plane(wAxis + zAxis, mat.values[15] + mat.values[14],true);
	planes[5] = Plane(wAxis - zAxis, mat.values[15] - mat.values[14],true);
}