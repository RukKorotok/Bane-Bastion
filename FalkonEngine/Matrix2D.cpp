#include "pch.h"
#include "Matrix2D.h"

constexpr float PI = 3.14159265358979323846f;

namespace FalkonEngine
{   
	//Matrix2D
	//-----------------------------------------------------------------------------------------------------------
	Matrix2D::Matrix2D()
	{
		m_matrix[0][0] = 1.0f; m_matrix[0][1] = 0.0f; m_matrix[0][2] = 0.0f;
		m_matrix[1][0] = 0.0f; m_matrix[1][1] = 1.0f; m_matrix[1][2] = 0.0f;
		m_matrix[2][0] = 0.0f; m_matrix[2][1] = 0.0f; m_matrix[2][2] = 1.0f;
	}
	//-----------------------------------------------------------------------------------------------------------
	Matrix2D::Matrix2D(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22)
	{
		m_matrix[0][0] = a00; m_matrix[0][1] = a01; m_matrix[0][2] = a02;
		m_matrix[1][0] = a10; m_matrix[1][1] = a11; m_matrix[1][2] = a12;
		m_matrix[2][0] = a20; m_matrix[2][1] = a21; m_matrix[2][2] = a22;
	}
	//-----------------------------------------------------------------------------------------------------------
	Matrix2D::Matrix2D(Vector2Df position, float angle, Vector2Df scale)
	{
		float radians = angle * (PI / 180.0f);
		float cosA = cos(radians);
		float sinA = sin(radians);

		m_matrix[0][0] = cosA * scale.x;
		m_matrix[0][1] = -sinA * scale.y;
		m_matrix[0][2] = position.x;

		m_matrix[1][0] = sinA * scale.x;
		m_matrix[1][1] = cosA * scale.y;
		m_matrix[1][2] = position.y;

		m_matrix[2][0] = 0.0f;
		m_matrix[2][1] = 0.0f;
		m_matrix[2][2] = 1.0f;

		FE_APP_TRACE("Matrix2D: Transform matrix created for position (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
	}
	//-----------------------------------------------------------------------------------------------------------
	Matrix2D Matrix2D::operator*(const Matrix2D& other) const
	{
		Matrix2D result;
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				result.m_matrix[row][col] = 
					m_matrix[row][0] * other.m_matrix[0][col] +
					m_matrix[row][1] * other.m_matrix[1][col] +
					m_matrix[row][2] * other.m_matrix[2][col];
			}
		}
		return result;
	}
	//-----------------------------------------------------------------------------------------------------------
	const std::array<std::array<float, 3>, 3>& Matrix2D::GetMatrix() const
	{
		return m_matrix;
	}
	//-----------------------------------------------------------------------------------------------------------
	Matrix2D Matrix2D::GetInversed() const
	{
		
		float det = Determinant();

		if (std::abs(det) < 1e-6f)
		{
			FE_CORE_ERROR("Attempted to invert a singular matrix (Determinant is 0)!");
			return Matrix2D();
		}

		float invDet = 1.0f / det;
		Matrix2D res;

		res.m_matrix[0][0] = (m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1]) * invDet;
		res.m_matrix[0][1] = (m_matrix[0][2] * m_matrix[2][1] - m_matrix[0][1] * m_matrix[2][2]) * invDet;
		res.m_matrix[0][2] = (m_matrix[0][1] * m_matrix[1][2] - m_matrix[0][2] * m_matrix[1][1]) * invDet;

		res.m_matrix[1][0] = (m_matrix[1][2] * m_matrix[2][0] - m_matrix[1][0] * m_matrix[2][2]) * invDet;
		res.m_matrix[1][1] = (m_matrix[0][0] * m_matrix[2][2] - m_matrix[0][2] * m_matrix[2][0]) * invDet;
		res.m_matrix[1][2] = (m_matrix[1][0] * m_matrix[0][2] - m_matrix[0][0] * m_matrix[1][2]) * invDet;

		res.m_matrix[2][0] = (m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0]) * invDet;
		res.m_matrix[2][1] = (m_matrix[2][0] * m_matrix[0][1] - m_matrix[0][0] * m_matrix[2][1]) * invDet;
		res.m_matrix[2][2] = (m_matrix[0][0] * m_matrix[1][1] - m_matrix[1][0] * m_matrix[0][1]) * invDet;

		return res;
	}
	//-----------------------------------------------------------------------------------------------------------
	float Matrix2D::Determinant() const
	{
		{
			return m_matrix[0][0] * (m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1]) -
				m_matrix[0][1] * (m_matrix[1][0] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][0]) +
				m_matrix[0][2] * (m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0]);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Matrix2D::Print() const
	{
		std::string output = "\n";
		for (int row = 0; row < 3; ++row)
		{
			output += "| ";
			for (int col = 0; col < 3; ++col)
			{
				output += std::to_string(m_matrix[row][col]) + " ";
			}
			output += "|\n";
		}
		FE_CORE_INFO("Matrix2D Content: " + output);
	}
}