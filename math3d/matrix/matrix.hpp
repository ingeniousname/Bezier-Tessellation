#pragma once
#include "../vector/vector.hpp"
#include "../quat/quat.hpp"
#include <cstring>
#include <algorithm>

template<typename T>
class Matrix4x4 {
	T data[16];
public:

	Matrix4x4();

	T* operator[](int idx);
	const T* operator[](int idx) const;

	static Matrix4x4<T> identity();
	static Matrix4x4<T> rotX(float alpha);
	static Matrix4x4<T> rotY(float alpha);
	static Matrix4x4<T> rotZ(float alpha);
	static Matrix4x4<T> rotQuat(const Quat& q);
	static Matrix4x4<T> translate(const Vector4<float>& v);
	static Matrix4x4<T> translate(float x, float y, float z);
	static Matrix4x4<T> scale(const Vector4<float>& v);
	static Matrix4x4<T> scale(float x, float y, float z);

	static Matrix4x4<T> viewRH(const Vector4<float>& up, const Vector4<float>& right, const Vector4<float>& forward, const Vector4<float> position);
	static Matrix4x4<T> inverseViewRH(const Vector4<float>& up, const Vector4<float>& right, const Vector4<float>& forward, const Vector4<float> position);
	static Matrix4x4<T> perspectiveRH(float fov, float aspect, float zNear, float zFar);
	static Matrix4x4<T> stereoPerspectiveRH(float fov, float aspect, float zNear, float zFar, float iod, float convergence, bool isLeftEye);
	static Matrix4x4<T> inversePerspectiveRH(float fov, float aspect, float zNear, float zFar);


	Vector4<T> operator*(const Vector4<T>& v) const;
	Matrix4x4<T> operator*(const Matrix4x4<T>& other) const;

	Matrix4x4<T> transpose() const;
};

template<typename T>
inline Matrix4x4<T>::Matrix4x4()
{
	memset(data, 0, 16 * sizeof(T));
}

template<typename T>
inline T* Matrix4x4<T>::operator[](int idx)
{
	return data + idx * 4;
}

template<typename T>
inline const T* Matrix4x4<T>::operator[](int idx) const
{
	return data + idx * 4;
}

template<typename T>
inline Vector4<T> Matrix4x4<T>::operator*(const Vector4<T>& v) const
{
	T u[4];
	for (int i = 0; i < 4; ++i)
		u[i] = data[i * 4] * v.x + data[i * 4 + 1] * v.y + data[i * 4 + 2] * v.z + data[i * 4 + 3] * v.w;
	return Vector4<T>(u[0], u[1], u[2], u[3]);
}

template<typename T>
inline Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& other) const
{
	Matrix4x4 out;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			out[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				out[i][j] += (*this)[i][k] * other[k][j];
			}
		}
	}

	return out;

}

template<>
inline Matrix4x4<float> Matrix4x4<float>::identity()
{
	Matrix4x4<float> m;
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::rotX(float alpha)
{
	Matrix4x4<float> m = Matrix4x4<float>::identity();
	m[1][1] = m[2][2] = cosf(alpha);
	m[2][1] = sinf(alpha);
	m[1][2] = -m[2][1];

	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::rotY(float alpha)
{
	Matrix4x4<float> m = Matrix4x4<float>::identity();
	m[0][0] = m[2][2] = cosf(alpha);
	m[0][2] = sinf(alpha);
	m[2][0] = -m[0][2];

	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::rotZ(float alpha)
{
	Matrix4x4<float> m = Matrix4x4<float>::identity();
	m[0][0] = m[1][1] = cosf(alpha);
	m[1][0] = sinf(alpha);
	m[0][1] = -m[1][0];

	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::rotQuat(const Quat& q)
{
	Matrix4x4<float> m;

	m[0][0] = 2 * (q.q0 * q.q0 + q.q1 * q.q1) - 1;
	m[0][1] = 2 * (q.q1 * q.q2 - q.q0 * q.q3);
	m[0][2] = 2 * (q.q1 * q.q3 + q.q0 * q.q2);

	m[1][0] = 2 * (q.q1 * q.q2 + q.q0 * q.q3);
	m[1][1] = 2 * (q.q0 * q.q0 + q.q2 * q.q2) - 1;
	m[1][2] = 2 * (q.q2 * q.q3 - q.q0 * q.q1);

	m[2][0] = 2 * (q.q1 * q.q3 - q.q0 * q.q2);
	m[2][1] = 2 * (q.q2 * q.q3 + q.q0 * q.q1);
	m[2][2] = 2 * (q.q0 * q.q0 + q.q3 * q.q3) - 1;

	m[3][3] = 1;


	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::translate(const Vector4<float>& v)
{
	return Matrix4x4::translate(v.x, v.y, v.z);
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::translate(float x, float y, float z)
{
	Matrix4x4<float> m = Matrix4x4<float>::identity();
	m[0][3] = x;
	m[1][3] = y;
	m[2][3] = z;

	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::scale(const Vector4<float>& v)
{
	return Matrix4x4<float>::scale(v.x, v.y, v.z);
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::scale(float x, float y, float z)
{
	Matrix4x4<float> m;
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	m[3][3] = 1;

	return m;
}

template<typename T>
inline Matrix4x4<T> Matrix4x4<T>::transpose() const
{
	Matrix4x4<float> m;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = (*this)[j][i];
		}
	}

	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::viewRH(const Vector4<float>& up, const Vector4<float>& right, const Vector4<float>& forward, const Vector4<float> position)
{
	Matrix4x4<float> m = Matrix4x4<float>::identity();

	m[0][0] = right.x;
	m[1][0] = right.y;
	m[2][0] = right.z;

	m[0][1] = up.x;
	m[1][1] = up.y;
	m[2][1] = up.z;

	m[0][2] = forward.x;
	m[1][2] = forward.y;
	m[2][2] = forward.z;

	return m.transpose() * Matrix4x4<float>::translate(-position.x, -position.y, -position.z);
}


template<>
inline Matrix4x4<float> Matrix4x4<float>::inverseViewRH(const Vector4<float>& up, const Vector4<float>& right, const Vector4<float>& forward, const Vector4<float> position)
{
	Matrix4x4<float> m = Matrix4x4<float>::identity();

	m[0][0] = right.x;
	m[1][0] = right.y;
	m[2][0] = right.z;

	m[0][1] = up.x;
	m[1][1] = up.y;
	m[2][1] = up.z;

	m[0][2] = forward.x;
	m[1][2] = forward.y;
	m[2][2] = forward.z;

	return Matrix4x4<float>::translate(position.x, position.y, position.z) * m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::perspectiveRH(float fov, float aspect, float zNear, float zFar)
{
	Matrix4x4<float> m;
	m[0][0] = 1 / (tanf(fov / 2) * aspect);
	m[1][1] = 1 / tanf(fov / 2);
	m[2][2] = -(zFar + zNear) / (zFar - zNear);
	m[2][3] = -2 * zFar * zNear / (zFar - zNear);
	m[3][2] = -1;
	return m;
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::stereoPerspectiveRH(float fov, float aspect, float zNear, float zFar, float iod, float convergence, bool isLeftEye)
{
	float tanHalfFov = tanf(fov / 2.0f);
	float top = zNear * tanHalfFov;
	float bottom = -top;
	float right = top * aspect;
	float left = -right;

	float eyeOffset = (isLeftEye ? -iod / 2.0f : iod / 2.0f);

	float shift = (eyeOffset * zNear) / convergence;
	left += shift;
	right += shift;

	Matrix4x4<float> m;
	m[0][0] = (2.0f * zNear) / (right - left);
	m[1][1] = (2.0f * zNear) / (top - bottom);
	m[0][2] = (right + left) / (right - left);
	m[1][2] = (top + bottom) / (top - bottom);
	m[2][2] = -(zFar + zNear) / (zFar - zNear);
	m[2][3] = -(2.0f * zFar * zNear) / (zFar - zNear);
	m[3][2] = -1.0f;

	return m * translate(eyeOffset, 0.f, 0.f);
}

template<>
inline Matrix4x4<float> Matrix4x4<float>::inversePerspectiveRH(float fov, float aspect, float zNear, float zFar)
{
	Matrix4x4<float> m;
	float tanHalfFov = tanf(fov / 2);

	m[0][0] = tanHalfFov * aspect;
	m[1][1] = tanHalfFov;
	m[3][2] = (zFar - zNear) / (-2 * zFar * zNear);
	m[2][3] = -1;
	m[3][3] = (zFar + zNear) / (2 * zFar * zNear);

	return m;
}