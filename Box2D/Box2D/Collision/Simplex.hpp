/*
 * Original work Copyright (c) 2007-2009 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2016 Louis Langholtz https://github.com/louis-langholtz/Box2D
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef Simplex_hpp
#define Simplex_hpp

#include <Box2D/Common/ArrayList.hpp>
#include <Box2D/Collision/SimplexVertex.hpp>

namespace box2d
{
	/// Simplex vertices collection.
	///
	/// @note This data structure may be 28 * 3 = 84-bytes large.
	///
	using SimplexVertices = ArrayList<SimplexVertex, MaxSimplexVertices>;
	
	/// Calculates the "search direction" for the given simplex.
	/// @param simplex A one or two vertex simplex.
	/// @warning Behavior is undefined if the given simplex has zero vertices.
	/// @return "search direction" vector.
	constexpr inline Vec2 CalcSearchDirection(const SimplexVertices& simplex) noexcept
	{
		static_assert(std::tuple_size<SimplexVertices>::value == 3,
					  "Invalid maximum # of elements of Simplex");

		assert((simplex.size() == 1) || (simplex.size() == 2));
		switch (simplex.size())
		{
			case 1:
				return -GetPointDelta(simplex[0]);
				
			case 2:
			{
				const auto e12 = GetPointDelta(simplex[1]) - GetPointDelta(simplex[0]);
				const auto sgn = Cross(e12, -GetPointDelta(simplex[0]));
				// If sgn > 0, then origin is left of e12, else origin is right of e12.
				return (sgn > float_t{0})? GetRevPerpendicular(e12): GetFwdPerpendicular(e12);
			}
				
			default:
				return Vec2_zero;
		}
	}
	
	/// Gets the given simplex's "metric".
	inline float_t CalcMetric(const SimplexVertices& simplex)
	{
		static_assert(std::tuple_size<SimplexVertices>::value == 3,
					  "Invalid maximum # of elements of Simplex");

		assert(simplex.size() < 4);
		switch (simplex.size())
		{
			case 0: return float_t{0};
			case 1: return float_t{0};
			case 2:	return Sqrt(LengthSquared(GetPointDelta(simplex[0]) - GetPointDelta(simplex[1])));
			case 3:	return Cross(GetPointDelta(simplex[1]) - GetPointDelta(simplex[0]),
								 GetPointDelta(simplex[2]) - GetPointDelta(simplex[0]));
			default: break; // should not be reached
		}
		return float_t{0};
	}

	/// Simplex.
	///
	/// @detail An encapsulation of a point, line segment, or triangle.
	///   These are defined respectively as: a 0-simplex, a 1-simplex, and a 2-simplex.
	///   Used in doing GJK collision detection.
	///
	/// @sa https://en.wikipedia.org/wiki/Simplex
	/// @sa https://en.wikipedia.org/wiki/Gilbert%2DJohnson%2DKeerthi_distance_algorithm
	///
	class Simplex
	{
	public:
		using Coefficients = ArrayList<float_t, MaxSimplexVertices>;

		static Simplex Get(const SimplexVertices& vertices);

		Simplex() = default;

		SimplexVertices GetSimplexVertices() const noexcept { return m_simplexVertices; }

		const SimplexVertex& GetSimplexVertex(SimplexVertices::size_type index) const noexcept
		{
			return m_simplexVertices[index];
		}

		float_t GetCoefficient(SimplexVertices::size_type index) const noexcept
		{
			return m_normalizedWeights[index];
		}

		SimplexVertices::size_type GetSize() const noexcept { return m_simplexVertices.size(); }

	private:
		static Simplex Get(const SimplexVertex& s0) noexcept;
		static Simplex Get(const SimplexVertex& s0, const SimplexVertex& s1) noexcept;
		static Simplex Get(const SimplexVertex& s0, const SimplexVertex& s1, const SimplexVertex& s2) noexcept;

		Simplex(const SimplexVertices& simplexVertices, const Coefficients& normalizedWeights):
			m_simplexVertices{simplexVertices}, m_normalizedWeights{normalizedWeights}
		{
			assert(simplexVertices.size() == normalizedWeights.size());
			assert(almost_equal(1, [&]()
			{
				auto sum = float_t(0);
				for (auto&& elem: normalizedWeights)
				{
					assert(elem > 0);
					sum += elem;
				}
				return sum;
			}()));
		}

		SimplexVertices m_simplexVertices;
		Coefficients m_normalizedWeights;
	};
	
	/// Gets the simplex for the given collection of vertices.
	/// @param vertices Collection of one, two, or three simplex vertexes.
	/// @warning Behavior is undefined if the given collection has less than 1 or more than 3
	///   vertices.
	/// @return One, two, or three vertex simplex.
	inline Simplex Simplex::Get(const SimplexVertices& vertices)
	{
		const auto count = vertices.size();
		assert(count == 1 || count == 2 || count == 3);
		switch (count)
		{
			case 1: return Get(vertices[0]);
			case 2: return Get(vertices[0], vertices[1]);
			case 3: return Get(vertices[0], vertices[1], vertices[2]);
			default: break;
		}
		return Simplex{};
	}

	inline Simplex Simplex::Get(const SimplexVertex& s0) noexcept
	{
		return Simplex{{s0}, {1}};
	}

	/// Solves the given line segment simplex using barycentric coordinates.
	///
	/// @note The given simplex must have two simplex vertices.
	/// @warning Behavior is undefined if the given simplex doesn't have two vertices.
	///
	/// @detail
	/// p = a1 * w1 + a2 * w2
	/// a1 + a2 = 1
	///
	/// The vector from the origin to the closest point on the line is
	/// perpendicular to the line.
	/// e12 = w2 - w1
	/// dot(p, e) = 0
	/// a1 * dot(w1, e) + a2 * dot(w2, e) = 0
	///
	/// 2-by-2 linear system
	/// [1      1     ][a1] = [1]
	/// [w1.e12 w2.e12][a2] = [0]
	///
	/// Define
	/// d12_1 =  dot(w2, e12)
	/// d12_2 = -dot(w1, e12)
	/// d12 = d12_1 + d12_2
	///
	/// Solution
	/// a1 = d12_1 / d12
	/// a2 = d12_2 / d12
	///
	/// @result One or two vertex "solution".
	inline Simplex Simplex::Get(const SimplexVertex& s0, const SimplexVertex& s1) noexcept
	{
		const auto w1 = GetPointDelta(s0);
		const auto w2 = GetPointDelta(s1);
		const auto e12 = w2 - w1;
		
		// w1 region
		const auto d12_2 = -Dot(w1, e12);
		if (d12_2 <= 0)
		{
			// a2 <= 0, so we clamp it to 0
			return Simplex{{s0}, {1}};
		}
		
		// w2 region
		const auto d12_1 = Dot(w2, e12);
		if (d12_1 <= 0)
		{
			// a1 <= 0, so we clamp it to 0
			return Simplex{{s1}, {1}};
		}
		
		// Must be in e12 region.
		const auto d12_sum = d12_1 + d12_2;
		return Simplex{{s0, s1}, {d12_1 / d12_sum, d12_2 / d12_sum}};
	}

	/// Solves the given 3-vertex simplex.
	///
	/// @note The given simplex must have three simplex vertices.
	/// @warning Behavior is undefined if the given simplex doesn't have three vertices.
	///
	/// @detail
	/// Possible regions:
	/// - points[2]
	/// - edge points[0]-points[2]
	/// - edge points[1]-points[2]
	/// - inside the triangle
	///
	/// @result One, two, or three vertex "solution".
	inline Simplex Simplex::Get(const SimplexVertex& s0, const SimplexVertex& s1, const SimplexVertex& s2) noexcept
	{
		const auto w1 = GetPointDelta(s0);
		const auto w2 = GetPointDelta(s1);
		const auto w3 = GetPointDelta(s2);
		
		// Edge12
		// [1      1     ][a1] = [1]
		// [w1.e12 w2.e12][a2] = [0]
		// a3 = 0
		const auto e12 = w2 - w1;
		const auto w1e12 = Dot(w1, e12);
		const auto w2e12 = Dot(w2, e12);
		const auto d12_1 = w2e12;
		const auto d12_2 = -w1e12;
		
		// Edge13
		// [1      1     ][a1] = [1]
		// [w1.e13 w3.e13][a3] = [0]
		// a2 = 0
		const auto e13 = w3 - w1;
		const auto w1e13 = Dot(w1, e13);
		const auto w3e13 = Dot(w3, e13);
		const auto d13_1 = w3e13;
		const auto d13_2 = -w1e13;
		
		// Edge23
		// [1      1     ][a2] = [1]
		// [w2.e23 w3.e23][a3] = [0]
		// a1 = 0
		const auto e23 = w3 - w2;
		const auto w2e23 = Dot(w2, e23);
		const auto w3e23 = Dot(w3, e23);
		const auto d23_1 = w3e23;
		const auto d23_2 = -w2e23;
		
		// Triangle123
		const auto n123 = Cross(e12, e13);
		
		const auto d123_1 = n123 * Cross(w2, w3);
		const auto d123_2 = n123 * Cross(w3, w1);
		const auto d123_3 = n123 * Cross(w1, w2);
		
		// w1 region
		if ((d12_2 <= 0) && (d13_2 <= 0))
		{
			return Simplex{{s0}, {1}};
		}
		
		// e12
		if ((d12_1 > 0) && (d12_2 > 0) && (d123_3 <= 0))
		{
			const auto d12_sum = d12_1 + d12_2;
			return Simplex{{s0, s1}, {d12_1 / d12_sum, d12_2 / d12_sum}};
		}
		
		// e13
		if ((d13_1 > 0) && (d13_2 > 0) && (d123_2 <= 0))
		{
			const auto d13_sum = d13_1 + d13_2;
			return Simplex{{s0, s2}, {d13_1 / d13_sum, d13_2 / d13_sum}};
		}
		
		// w2 region
		if ((d12_1 <= 0) && (d23_2 <= 0))
		{
			return Simplex{{s1}, {1}};
		}
		
		// w3 region
		if ((d13_1 <= 0) && (d23_1 <= 0))
		{
			return Simplex{{s2}, {1}};
		}
		
		// e23
		if ((d23_1 > 0) && (d23_2 > 0) && (d123_1 <= 0))
		{
			const auto d23_sum = d23_1 + d23_2;
			return Simplex{{s2, s1}, {d23_2 / d23_sum, d23_1 / d23_sum}};
		}
		
		// Must be in triangle123
		const auto d123_sum = d123_1 + d123_2 + d123_3;
		return Simplex{{s0, s1, s2}, {d123_1 / d123_sum, d123_2 / d123_sum, d123_3 / d123_sum}};
	}

	inline Vec2 GetScaledDelta(const Simplex& simplex, SimplexVertices::size_type index)
	{
		return simplex.GetSimplexVertex(index).GetPointDelta() * simplex.GetCoefficient(index);
	}

	/// Gets the "closest point".
	/// @note This uses the vertices "a" values when count is 2.
	constexpr inline Vec2 GetClosestPoint(const Simplex& simplex)
	{
		switch (simplex.GetSize())
		{
			case 1: return GetScaledDelta(simplex, 0);
			case 2: return GetScaledDelta(simplex, 0) + GetScaledDelta(simplex, 1);
			case 3: return Vec2_zero;
			default: return Vec2_zero;
		}
	}

}	

#endif /* Simplex_hpp */
