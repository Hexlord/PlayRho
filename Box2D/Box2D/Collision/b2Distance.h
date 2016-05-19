
/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
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

#ifndef B2_DISTANCE_H
#define B2_DISTANCE_H

#include <Box2D/Common/b2Math.h>

namespace box2d
{

class Shape;

/// A distance proxy is used by the GJK algorithm.
/// It encapsulates any shape.
class b2DistanceProxy
{
public:
	using size_type = size_t; // must be big enough to hold max posible count of vertices

	b2DistanceProxy() = default;

	/// Initialize the proxy using the given shape.
	/// @note The shape must remain in scope while the proxy is in use.
	b2DistanceProxy(const Shape& shape, child_count_t index);

	/// Gets the "radius" of the associated shape.
	/// @return Non-negative distance.
	float_t GetRadius() const noexcept { return m_radius; }

	/// Get the supporting vertex index in the given direction.
	size_type GetSupport(const Vec2& d) const noexcept;

	/// Get the supporting vertex in the given direction.
	const Vec2& GetSupportVertex(const Vec2& d) const;

	/// Get the vertex count.
	inline size_type GetVertexCount() const noexcept { return m_count; }

	/// Get a vertex by index. Used by Distance.
	Vec2 GetVertex(size_type index) const;

private:
	Vec2 m_buffer[2];
	const Vec2* m_vertices = nullptr;
	size_type m_count = 0;
	float_t m_radius = float_t{0}; ///< "Radius" of the associated shape.
};

/// Used to warm start Distance.
class b2SimplexCache
{
public:
	static constexpr auto MaxCount = unsigned{3};
	using size_type = std::remove_cv<decltype(MaxCount)>::type;

	using index_t = size_t;

	float_t GetMetric() const noexcept { return metric; }
	size_type GetCount() const noexcept { return count; }

	index_t GetIndexA(size_type index) const
	{
		assert(index < count);
		return indexA[index];
	}

	index_t GetIndexB(size_type index) const
	{
		assert(index < count);
		return indexB[index];
	}

	void ClearIndices() noexcept { count = 0; }

	void SetMetric(float_t m) noexcept { metric = m; }

	void AddIndex(index_t a, index_t b)
	{
		assert(count < MaxCount);
		indexA[count] = a;
		indexB[count] = b;
		++count;
	}

private:
	float_t metric;		///< length or area
	size_type count = 0;
	index_t indexA[MaxCount];	///< vertices on shape A
	index_t indexB[MaxCount];	///< vertices on shape B
};

/// Input for Distance.
/// You have to option to use the shape radii
/// in the computation. Even 
struct b2DistanceInput
{
	b2DistanceProxy proxyA;
	b2DistanceProxy proxyB;
	Transform transformA;
	Transform transformB;
	bool useRadii;
};

/// Output for Distance.
struct b2DistanceOutput
{
	Vec2 pointA;		///< closest point on shapeA
	Vec2 pointB;		///< closest point on shapeB
	float_t distance;
	int32 iterations;	///< number of GJK iterations used
};

/// Compute the closest points between two shapes. Supports any combination of:
/// CircleShape, PolygonShape, EdgeShape. The simplex cache is input/output.
/// On the first call, b2SimplexCache.count should be set to zero.
b2DistanceOutput Distance(b2SimplexCache& cache,  const b2DistanceInput& input);

//////////////////////////////////////////////////////////////////////////

inline Vec2 b2DistanceProxy::GetVertex(size_type index) const
{
	assert(index >= 0);
	assert(index < m_count);
	return m_vertices[index];
}

inline b2DistanceProxy::size_type b2DistanceProxy::GetSupport(const Vec2& d) const noexcept
{
	auto bestIndex = decltype(m_count){0};
	auto bestValue = Dot(m_vertices[0], d);
	for (auto i = decltype(m_count){1}; i < m_count; ++i)
	{
		const auto value = Dot(m_vertices[i], d);
		if (value > bestValue)
		{
			bestIndex = i;
			bestValue = value;
		}
	}

	return bestIndex;
}

inline const Vec2& b2DistanceProxy::GetSupportVertex(const Vec2& d) const
{
	auto bestIndex = decltype(m_count){0};
	auto bestValue = Dot(m_vertices[0], d);
	for (auto i = decltype(m_count){1}; i < m_count; ++i)
	{
		const auto value = Dot(m_vertices[i], d);
		if (value > bestValue)
		{
			bestIndex = i;
			bestValue = value;
		}
	}

	return m_vertices[bestIndex];
}

} /* namespace box2d */

#endif
