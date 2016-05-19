/*
* Copyright (c) 2007-2009 Erin Catto http://www.box2d.org
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

#include <Box2D/Collision/b2Collision.h>
#include <Box2D/Collision/b2Distance.h>

namespace box2d {

WorldManifold::WorldManifold(const b2Manifold& manifold,
								 const Transform& xfA, float_t radiusA,
								 const Transform& xfB, float_t radiusB)
{
	this->Assign(manifold, xfA, radiusA, xfB, radiusB);
}

void WorldManifold::Assign(const b2Manifold& manifold,
						  const Transform& xfA, float_t radiusA,
						  const Transform& xfB, float_t radiusB)
{
	if (manifold.GetPointCount() == 0)
		return;

	switch (manifold.GetType())
	{
	case b2Manifold::e_unset:
		assert(manifold.GetType() != b2Manifold::e_unset);
		break;

	case b2Manifold::e_circles:
		{
			normal = Vec2(float_t{1}, float_t{0});
			const auto pointA = Mul(xfA, manifold.GetLocalPoint());
			const auto pointB = Mul(xfB, manifold.GetPoint(0).localPoint);
			if (DistanceSquared(pointA, pointB) > Square(Epsilon))
				normal = Normalize(pointB - pointA);

			const auto cA = pointA + (radiusA * normal);
			const auto cB = pointB - (radiusB * normal);
			points[0] = (cA + cB) / float_t(2);
			separations[0] = Dot(cB - cA, normal);
			pointCount = 1;
		}
		break;

	case b2Manifold::e_faceA:
		{
			normal = Mul(xfA.q, manifold.GetLocalNormal());
			const auto planePoint = Mul(xfA, manifold.GetLocalPoint());
			pointCount = 0;
			for (auto i = decltype(manifold.GetPointCount()){0}; i < manifold.GetPointCount(); ++i)
			{
				const auto clipPoint = Mul(xfB, manifold.GetPoint(i).localPoint);
				const auto cA = clipPoint + (radiusA - Dot(clipPoint - planePoint, normal)) * normal;
				const auto cB = clipPoint - (radiusB * normal);
				points[i] = (cA + cB) / float_t(2);
				separations[i] = Dot(cB - cA, normal);
				++pointCount;
			}
		}
		break;

	case b2Manifold::e_faceB:
		{
			normal = Mul(xfB.q, manifold.GetLocalNormal());
			const auto planePoint = Mul(xfB, manifold.GetLocalPoint());
			pointCount = 0;
			for (auto i = decltype(manifold.GetPointCount()){0}; i < manifold.GetPointCount(); ++i)
			{
				const auto clipPoint = Mul(xfA, manifold.GetPoint(i).localPoint);
				const auto cB = clipPoint + (radiusB - Dot(clipPoint - planePoint, normal)) * normal;
				const auto cA = clipPoint - (radiusA * normal);
				points[i] = (cA + cB) / float_t(2);
				separations[i] = Dot(cA - cB, normal);
				++pointCount;
			}

			// Ensure normal points from A to B.
			normal = -normal;
		}
		break;
	}
}

void b2GetPointStates(b2PointStateArray& state1, b2PointStateArray& state2,
					  const b2Manifold& manifold1, const b2Manifold& manifold2)
{
	for (auto i = decltype(MaxManifoldPoints){0}; i < MaxManifoldPoints; ++i)
	{
		state1[i] = b2PointState::NullState;
		state2[i] = b2PointState::NullState;
	}

	// Detect persists and removes.
	for (auto i = decltype(manifold1.GetPointCount()){0}; i < manifold1.GetPointCount(); ++i)
	{
		const auto& cf = manifold1.GetPoint(i).cf;

		state1[i] = b2PointState::RemoveState;

		for (auto j = decltype(manifold2.GetPointCount()){0}; j < manifold2.GetPointCount(); ++j)
		{
			if (manifold2.GetPoint(j).cf == cf)
			{
				state1[i] = b2PointState::PersistState;
				break;
			}
		}
	}

	// Detect persists and adds.
	for (auto i = decltype(manifold2.GetPointCount()){0}; i < manifold2.GetPointCount(); ++i)
	{
		const auto& cf = manifold2.GetPoint(i).cf;

		state2[i] = b2PointState::AddState;

		for (auto j = decltype(manifold1.GetPointCount()){0}; j < manifold1.GetPointCount(); ++j)
		{
			if (manifold1.GetPoint(j).cf == cf)
			{
				state2[i] = b2PointState::PersistState;
				break;
			}
		}
	}
}

// From Real-time Collision Detection, p179.
bool b2AABB::RayCast(b2RayCastOutput* output, const b2RayCastInput& input) const
{
	auto tmin = -MaxFloat;
	auto tmax = MaxFloat;

	const auto p = input.p1;
	const auto d = input.p2 - input.p1;
	const auto absD = Abs(d);

	Vec2 normal;

	for (auto i = decltype(Vec2::NumElements){0}; i < Vec2::NumElements; ++i)
	{
		if (absD(i) < Epsilon)
		{
			// Parallel.
			if ((p(i) < lowerBound(i)) || (upperBound(i) < p(i)))
			{
				return false;
			}
		}
		else
		{
			const auto inv_d = float_t(1) / d(i);
			auto t1 = (lowerBound(i) - p(i)) * inv_d;
			auto t2 = (upperBound(i) - p(i)) * inv_d;

			// Sign of the normal vector.
			auto s = float_t(-1);

			if (t1 > t2)
			{
				Swap(t1, t2);
				s = float_t(1);
			}

			// Push the min up
			if (t1 > tmin)
			{
				normal = Vec2_zero;
				normal(i) = s;
				tmin = t1;
			}

			// Pull the max down
			tmax = Min(tmax, t2);

			if (tmin > tmax)
			{
				return false;
			}
		}
	}

	// Does the ray start inside the box?
	// Does the ray intersect beyond the max fraction?
	if ((tmin < float_t{0}) || (input.maxFraction < tmin))
	{
		return false;
	}

	// Intersection.
	output->fraction = tmin;
	output->normal = normal;
	return true;
}

b2ClipArray::size_type b2ClipSegmentToLine(b2ClipArray& vOut, const b2ClipArray& vIn,
										   const Vec2& normal, float_t offset,
										   b2ContactFeature::index_t indexA)
{
	// Use Sutherland-Hodgman clipping (https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm ).
	
	// Start with no output points
	auto numOut = b2ClipArray::size_type{0};

	// Calculate the distance of end points to the line
	const auto distance0 = Dot(normal, vIn[0].v) - offset; ///< Distance of point at vIn[0].v from line defined by normal and offset.
	const auto distance1 = Dot(normal, vIn[1].v) - offset; ///< Distance of point at vIn[1].v from line defined by normal and offset.

	// If the points are behind the plane
	if (distance0 <= float_t{0}) vOut[numOut++] = vIn[0];
	if (distance1 <= float_t{0}) vOut[numOut++] = vIn[1];

	// If the points are on different sides of the plane
	if ((distance0 * distance1) < float_t{0})
	{
		// Find intersection point of edge and plane
		const auto interp = distance0 / (distance0 - distance1);
		vOut[numOut].v = vIn[0].v + (vIn[1].v - vIn[0].v) * interp;

		// Vertex A is hitting edge B.
		vOut[numOut].cf = b2ContactFeature(b2ContactFeature::e_vertex, indexA, b2ContactFeature::e_face, vIn[0].cf.indexB);

		++numOut;
	}

	return numOut;
}

bool b2TestOverlap(const b2Shape& shapeA, child_count_t indexA,
				   const b2Shape& shapeB, child_count_t indexB,
				   const Transform& xfA, const Transform& xfB)
{
	b2DistanceInput input;
	input.proxyA = b2DistanceProxy(shapeA, indexA);
	input.proxyB = b2DistanceProxy(shapeB, indexB);
	input.transformA = xfA;
	input.transformB = xfB;
	input.useRadii = true;

	b2SimplexCache cache;
	const auto output = Distance(cache, input);
	return output.distance < (Epsilon * 10);
}

} // namespace box2d
