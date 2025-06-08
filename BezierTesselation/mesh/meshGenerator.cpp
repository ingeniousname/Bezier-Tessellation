#include "meshGenerator.hpp"
#include "mesh.hpp"
#include "GL\glew.h"

std::shared_ptr<Mesh> MeshGenerator::pointMesh = nullptr;
std::shared_ptr<Mesh> MeshGenerator::cursorMesh = nullptr;
std::shared_ptr<Mesh> MeshGenerator::gridMesh = nullptr;

std::shared_ptr<Mesh> MeshGenerator::GetTriangleMesh()
{
	std::vector<float> vertices = {
		-0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f,
		0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
		0.f, 0.5f, 0.0f, 0.f, 0.f, 1.f,
	};

	std::vector<unsigned> indices = {
		0, 1, 1, 2, 2, 0
	};

	return Mesh::createMesh(vertices, indices);
}

std::shared_ptr<Mesh> MeshGenerator::GetSphereMesh()
{
	std::vector<float> vertices;
	std::vector<unsigned> indices;

	const int n_subdiv = 50;
	for (int i = 0; i < n_subdiv; i++)
	{
		float theta = i * 3.1415f * 2 / n_subdiv;
		for (int i = 0; i <= n_subdiv; i++)
		{
			float phi = i * 3.1415f / n_subdiv;

			vertices.push_back(cos(theta) * sin(phi));
			vertices.push_back(sin(theta) * sin(phi));
			vertices.push_back(cos(phi));
		}
	}

	for (int i = 0; i < n_subdiv; i++)
	{
		for (int j = 0; j < n_subdiv; j++)
		{
			indices.push_back(i * n_subdiv + j);
			indices.push_back(i * n_subdiv + j + 1);
			indices.push_back(((i + 1) % n_subdiv) * n_subdiv + j + 1);

			indices.push_back(((i + 1) % n_subdiv) * n_subdiv + j + 1);
			indices.push_back(((i + 1) % n_subdiv) * n_subdiv + j);
			indices.push_back(i * n_subdiv + j);
		}
	}

	return Mesh::createMesh(vertices, indices);
}

std::shared_ptr<Mesh> MeshGenerator::GetPointMesh()
{
	if (pointMesh == nullptr)
	{
		std::vector<float> vertices = {
			-0.5f, -0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			0.5f, 0.5f, 0.f,
			-0.5f, 0.5f, 0.f,
		};

		std::vector<unsigned int> indices = {
			0, 1, 2, 2, 3, 0
		};

		pointMesh = Mesh::createMesh(vertices, indices);
	}

	return pointMesh;
}

std::shared_ptr<Mesh> MeshGenerator::GetGridMesh()
{
	const float gridSize = 50.f;
	if (gridMesh == nullptr)
	{
		std::vector<float> verticies;
		for (float x = -gridSize / 2; x <= gridSize / 2; x += 1.f)
		{
			verticies.push_back(x);
			verticies.push_back(0);
			verticies.push_back(-gridSize / 2);

			verticies.push_back(x);
			verticies.push_back(0);
			verticies.push_back(gridSize / 2);
		}

		for (float z = -gridSize / 2; z <= gridSize / 2; z += 1.f)
		{
			verticies.push_back(-gridSize / 2);
			verticies.push_back(0);
			verticies.push_back(z);

			verticies.push_back(gridSize / 2);
			verticies.push_back(0);
			verticies.push_back(z);
		}

		std::vector<unsigned int> indices;
		for (int i = 0; 3 * (i + 1) < verticies.size(); i += 2)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		gridMesh = Mesh::createMesh(verticies, indices);
	}

	return gridMesh;
}

std::shared_ptr<Mesh> MeshGenerator::GetCursorMesh()
{
	if (cursorMesh == nullptr)
	{
		std::vector<float> vertices = {
			-0.25f, 0.f, 0.f, 1.f, 0.f, 0.f,
			0.25f, 0.f, 0.f, 1.f, 0.f, 0.f,
			0.f, -0.25f, 0.f, 0.f, 1.f, 0.f,
			0.f, 0.25f, 0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, -0.25f, 0.f, 0.f, 1.f,
			0.f, 0.f, 0.25f, 0.f, 0.f, 1.f,
		};

		std::vector<unsigned int> indices = {
			0, 1, 2, 3, 4, 5
		};

		cursorMesh = Mesh::createMesh(vertices, indices, { {3, GL_FLOAT}, {3, GL_FLOAT} });
	}

	return cursorMesh;
}

std::pair<std::vector<float>, std::vector<unsigned int>> MeshGenerator::GetTorusVerticies(float r, float R, int div_r, int div_R)
{
	std::vector<float> vertices;
	std::vector<unsigned> indices;

	// division around large radius
	for (int i = 0; i < div_R; i++)
	{
		float beta = i * 3.1415f * 2 / div_R;
		// division around small radius
		for (int j = 0; j < div_r; j++)
		{
			float alpha = j * 3.1415f * 2 / div_r;
			float q0 = (R + r * cosf(alpha)) * cosf(beta);
			float y = r * sinf(alpha);
			float z = (R + r * cosf(alpha)) * sinf(beta);

			vertices.push_back(q0);
			vertices.push_back(y);
			vertices.push_back(z);
		}

	}

	for (int i = 0; i < div_R; i++)
	{
		for (int j = 0; j < div_r; j++)
		{
			indices.push_back(i * div_r + j);
			indices.push_back(((i + 1) % div_R) * div_r + j);

			indices.push_back(i * div_r + j);
			indices.push_back(i * div_r + (j + 1) % div_r);
		}
	}

	return { vertices, indices };
}


std::shared_ptr<Mesh> MeshGenerator::GetTorusMesh(float r, float R, int div_r, int div_R)
{
	auto vertexData = GetTorusVerticies(r, R, div_r, div_R);
	return Mesh::createMesh(vertexData.first, vertexData.second);
}
