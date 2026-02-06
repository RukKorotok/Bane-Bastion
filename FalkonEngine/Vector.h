#pragma once

#include <cmath>
#include <cstdint>
#include <functional>

namespace FalkonEngine
{
	template<typename T>
	struct Vector2D
	{
		T x = (T)0;
		T y = (T)0;

		Vector2D()
		{
			x = (T)0;
			y = (T)0;
		}

		Vector2D(T newX, T newY)
		{
			x = newX;
			y = newY;
		}

		float GetLength() const
		{
			return std::sqrt(static_cast<float>(x * x + y * y));
		}

		float DotProduct(const Vector2D<T>& vector) const
		{
			return x * vector.x + y * vector.y;
		}

		Vector2D<float> ToFloat() const
		{
			return { static_cast<float>(x), static_cast<float>(y) };
		}
		void Normalize()
		{
			float length = GetLength();
			if (length > 0.0f)
			{
				float invLength = 1.0f / length;

				x = static_cast<T>(x * invLength);
				y = static_cast<T>(y * invLength);
			}
		}

		bool operator<(const Vector2D<T>& other) const {
			if (x != other.x) return x < other.x;
			return y < other.y;
		}
		bool operator==(const Vector2D<T>& other) const {
			return x == other.x && y == other.y;
		}

		bool operator!=(const Vector2D<T>& other) const {
			return !(*this == other);
		}
	};

	using Vector2Df = Vector2D<float>;
	using Vector2Di = Vector2D<int>;
	using Position = Vector2Df;
	//-----------------------------------------------------------------------------------------------------------
	template<typename T>
	Vector2D<T> operator+(const Vector2D<T>& left, const Vector2D<T>& right)
	{
		return { left.x + right.x, left.y + right.y };
	}
	//-----------------------------------------------------------------------------------------------------------
	template<typename T>
	Vector2D<T> operator-(const Vector2D<T>& left, const Vector2D<T>& right)
	{
		return { left.x - right.x, left.y - right.y };
	}
	//-----------------------------------------------------------------------------------------------------------
	template<typename T>
	Vector2D<T> operator-(const Vector2D<T>& left)
	{
		return { -left.x, -left.y };
	}
	//-----------------------------------------------------------------------------------------------------------
	template<typename T>
	Vector2D<T> operator*(const Vector2D<T>& left, const Vector2D<T>& right)
	{
		return { left.x * right.x, left.y * right.y }; 
	}
	//-----------------------------------------------------------------------------------------------------------
	template<typename T>
	Vector2D<T> operator*(const T scalar, const Vector2D<T>& right)
	{
		return { scalar * right.x, scalar * right.y };
	}
	//-----------------------------------------------------------------------------------------------------------
	template<typename T>
	Vector2D<T> operator*(const Vector2D<T>& left, const T scalar)
	{
		return { scalar * left.x, scalar * left.y };
	}
	//-----------------------------------------------------------------------------------------------------------
	template<typename T>
	bool operator==(const Vector2D<T>& left, const Vector2D<T>& right)
	{
		return left.x == right.x && left.y == right.y;
	}
	//-----------------------------------------------------------------------------------------------------------
	//template<typename T>
	//bool operator!=(const Vector2D<T>& left, const Vector2D<T>& right)
	//{
	//	return left.x != right.x || left.y != right.y;
	//}
	////-----------------------------------------------------------------------------------------------------------
	template<typename T>
	Vector2Df GetNormalized(const Vector2D<T>& vector)
	{
		Vector2Df result = vector.ToFloat();
		result.Normalize();
		return result;
	}
	//-----------------------------------------------------------------------------------------------------------
	// This complex template allows us to convert any vector type to any other vector type (like our Vector2D to SFML's Vector and vice versa)
	template<typename U, typename V>
	U Convert(const V& v) 
	{
		return { static_cast<decltype(U::x)>(v.x), static_cast<decltype(U::y)>(v.y) };
	}
	//-----------------------------------------------------------------------------------------------------------

	struct Vector2DiHasher
	{
		std::size_t operator()(const Vector2Di& v) const noexcept
		{
			std::uint64_t x = static_cast<std::uint64_t>(v.x);
			std::uint64_t y = static_cast<std::uint64_t>(v.y);
			return std::hash<std::uint64_t>{}((x << 32) | (y & 0xFFFFFFFF));
		}
	};
}
namespace std
{
	//HashLogic
	//-----------------------------------------------------------------------------------------------------------
	template<>
	struct hash<FalkonEngine::Vector2Di>
	{
		size_t operator()(const FalkonEngine::Vector2Di& v) const noexcept
		{
			return hash<int>{}(v.x) ^ (hash<int>{}(v.y) << 1);
		}
	};
}
