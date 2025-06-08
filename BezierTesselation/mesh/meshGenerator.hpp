#pragma once
#include <vector>
#include <memory>
#include <vector\vector.hpp>

class Mesh;

class MeshGenerator {
	static std::shared_ptr<Mesh> pointMesh, cursorMesh, gridMesh;
public:
	static std::shared_ptr<Mesh> GetTriangleMesh();
	static std::shared_ptr<Mesh> GetSphereMesh();
	static std::shared_ptr<Mesh> GetPointMesh();
	static std::shared_ptr<Mesh> GetGridMesh();
	static std::shared_ptr<Mesh> GetCursorMesh();
	static std::pair<std::vector<float>, std::vector<unsigned int>> GetTorusVerticies(float r, float R, int div_r, int div_R);
	static std::shared_ptr<Mesh> GetTorusMesh(float r, float R, int div_r, int div_R);
};