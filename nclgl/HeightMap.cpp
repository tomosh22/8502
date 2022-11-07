#include "HeightMap.h"

HeightMap::HeightMap(const std::string& name) {
	int iWidth, iHeight, iChans;
	unsigned char* data = SOIL_load_image(name.c_str(), &iWidth, &iHeight, &iChans, 1);
	if (!data) {
		return;
	}
	numVertices = iWidth * iHeight;
	numIndices = (iWidth - 1) * (iHeight - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	colours = new Vector4[numVertices];
	indices = new GLuint[numIndices];
	Vector3 vertexScale = Vector3(16, 1, 16);
	Vector2 textureScale = Vector2(1 / 16.0f, 1 / 16.0f);

	int offset;
	float shadow;
	for (int z = 0; z < iHeight; z++)
	{
		for (int x = 0; x < iWidth; x++)
		{
			offset = (z * iWidth) + x;
			shadow = ((int)data[offset]) / 255.0f;
			colours[offset] = Vector4(shadow, shadow, shadow, 1.0f);
			vertices[offset] = Vector3(x, data[offset], z) * vertexScale;
			textureCoords[offset] = Vector2(x, z) * textureScale;
		}
	}
	SOIL_free_image_data(data);

	int i = 0;
	for (int z = 0; z < iHeight-1; z++)
	{
		for (int x = 0; x < iWidth-1; x++)
		{
			int a = (z * iWidth) + x;
			int b = (z * iWidth) + x + 1;
			int c = ((z+1) * iWidth) + x+1;
			int d = ((z+1) * iWidth) + x;
			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = b;
			indices[i++] = c;
			indices[i++] = a;
			indices[i++] = d;
		}
	}
	GenerateNormals();
	GenerateTangents();
	BufferData();
	heightMapSize.x = vertexScale.x * (iWidth - 1);
	heightMapSize.y = vertexScale.y * 255.0f;
	heightMapSize.z = vertexScale.z * (iHeight - 1);
}