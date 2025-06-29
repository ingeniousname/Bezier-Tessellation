#include "bezierC0Surface.hpp"
#include "mesh/meshGenerator.hpp"
#include <string>
#include "shader/shader.hpp"
#include <GL\glew.h>
#include "error/error.hpp"
#include "shader\shaderManager.hpp"
#include <imgui.h>

BezierC0Surface::BezierC0Surface(const std::vector<Vector4<float>>& points, int patchesU, int patchesV)
	: points(points), patchesU(patchesU), patchesV(patchesV), subdivOuter(1), subdivInner(1)
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

void BezierC0Surface::calculateLOD(const Matrix4x4<float>& view)
{
	assert(patchesU == 4 && patchesV == 4);

	int sizeU = 3 * patchesU + 1;
	int sizeV = 3 * patchesV + 1;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Vector4<float> avg(0.f);
			for (int i1 = i * 3; i1 < i * 3 + 4; i1++)
				for (int j1 = j * 3; j1 < j * 3 + 4; j1++)
					avg += this->points[i1 * sizeV + j1];

			avg /= 16;
			avg = view * avg;
			float z = std::abs(avg.z);

			this->lod[i][j] = std::clamp(-16.f * std::log10f(z * 0.01f), 1.f, 64.f);
		}
	}
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

void BezierC0Surface::ShareParameterUI()
{
	ImGui::Checkbox("Draw polynomial", &drawPolynomial);

	if (ImGui::SliderFloat("Subdivisions outside", &subdivOuter, 0.1f, 5.f))
	{
	}

	if (ImGui::SliderFloat("Subdivisions inside", &subdivInner, 0.1f, 5.f))
	{
	}
}

void BezierC0Surface::changeOuterSubdiv(float delta)
{
	subdivOuter += delta;
}

void BezierC0Surface::changeInnerSubdiv(float delta)
{
	subdivInner += delta;
}

void BezierC0Surface::draw(const ShaderManager& shaderManager)
{
	if (drawPolynomial)
	{
		glLineWidth(3.0f);
		this->polynomialMesh->bind();
		shaderManager.get("default")->bind();
		shaderManager.get("default")->setUniformVec4f("color", { 1.f, 0.f, 0.f, 1.f });
		shaderManager.get("default")->setUniformMat4fv("model", Matrix4x4<float>::identity());
		Call(glDrawElements(GL_LINES, this->polynomialMesh->getIndiciesCount(), GL_UNSIGNED_INT, nullptr));
	}


	glLineWidth(1.0f);
	this->mesh->bind();
	shaderManager.get("bezierC0Surface")->bind();
	shaderManager.get("bezierC0Surface")->setUniformVec4f("color", { 0.f, 0.f, 1.f, 1.f });
	shaderManager.get("bezierC0Surface")->setUniformMat4fv("lod", lod);

	shaderManager.get("bezierC0Surface")->setUniformFloat("subdivOuter", subdivOuter);
	shaderManager.get("bezierC0Surface")->setUniformFloat("subdivInner", subdivInner);

	Call(glPatchParameteri(GL_PATCH_VERTICES, 16));
	Call(glDrawElements(GL_PATCHES, this->mesh->getIndiciesCount(), GL_UNSIGNED_INT, nullptr));
}
