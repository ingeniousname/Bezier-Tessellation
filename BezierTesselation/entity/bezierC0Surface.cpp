#include "bezierC0Surface.hpp"
#include "mesh/meshGenerator.hpp"
#include <string>
#include "shader/shader.hpp"
#include <GL\glew.h>
#include "error/error.hpp"
#include "shader\shaderManager.hpp"

BezierC0Surface::BezierC0Surface(const std::vector<Vector4<float>>& points, int patchesU, int patchesV)
	: points(points), patchesU(patchesU), patchesV(patchesV), subdivU(10), subdivV(10)
{
	this->updateMesh();
}

BezierC0Surface::~BezierC0Surface()
{
}

void BezierC0Surface::setPolynomialMesh(const std::vector<float>& vertices, int sizeX, int sizeZ)
{
	std::vector<unsigned> indices;
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeZ; j++)
		{
			if (i < sizeX - 1)
			{
				indices.push_back(i * sizeZ + j);
				indices.push_back((i + 1) * sizeZ + j);
			}

			if (j + 1 < sizeZ)
			{
				indices.push_back(i * sizeZ + j);
				indices.push_back(i * sizeZ + j + 1);
			}
		}
	}

	polynomialMesh = Mesh::createMesh(vertices, indices);
}

void BezierC0Surface::updateMesh()
{
	std::vector<float> vertices;
	for (const auto& point : points)
	{
		vertices.push_back(point.x);
		vertices.push_back(point.y);
		vertices.push_back(point.z);
	}

	int sizeU = 3 * patchesU + 1;
	int sizeV = 3 * patchesV + 1;

	setPolynomialMesh(vertices, sizeU, sizeV);

	std::vector<unsigned> indices;

	for (int patchV = 0; patchV < patchesV; ++patchV) {
		for (int patchU = 0; patchU < patchesU; ++patchU) {
			int startU = 3 * patchU;
			int startV = 3 * patchV;

			for (int j = 0; j < 4; ++j) {
				for (int i = 0; i < 4; ++i) {

					int idxU = startU + i;
					indices.push_back(sizeV * idxU + startV + j);
				}
			}
		}
	}

	mesh = Mesh::createMesh(vertices, indices);
}

void BezierC0Surface::draw(const ShaderManager& shaderManager)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.0f);

	if (drawPolynomial)
	{
		this->polynomialMesh->bind();
		shaderManager.get("default")->bind();
		shaderManager.get("default")->setUniformVec4f("color", {1.f, 0.f, 0.f, 1.f});
		shaderManager.get("default")->setUniformMat4fv("model", Matrix4x4<float>::identity());
		Call(glDrawElements(GL_LINES, this->polynomialMesh->getIndiciesCount(), GL_UNSIGNED_INT, nullptr));
	}

	this->mesh->bind();
	shaderManager.get("bezierC0Surface")->bind();
	shaderManager.get("bezierC0Surface")->setUniformVec4f("color", {0.f, 0.f, 1.f, 1.f});

	shaderManager.get("bezierC0Surface")->setUniformInt("subdivU", subdivU);
	shaderManager.get("bezierC0Surface")->setUniformInt("subdivV", subdivV);

	Call(glPatchParameteri(GL_PATCH_VERTICES, 16));
	Call(glDrawElements(GL_PATCHES, this->mesh->getIndiciesCount(), GL_UNSIGNED_INT, nullptr));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}
