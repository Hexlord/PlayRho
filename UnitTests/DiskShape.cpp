/*
 * Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/Box2D
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

#include "gtest/gtest.h"
#include <Box2D/Collision/Shapes/DiskShape.hpp>
#include <Box2D/Collision/AABB.hpp>

using namespace box2d;

TEST(DiskShape, ByteSize)
{
    if (sizeof(RealNum) == 4)
    {
        EXPECT_EQ(sizeof(DiskShape), size_t(32));
    }
    else if (sizeof(RealNum) == 8)
    {
        EXPECT_EQ(sizeof(DiskShape), size_t(56));
    }
    else if (sizeof(RealNum) == 16)
    {
        EXPECT_EQ(sizeof(DiskShape), size_t(112));
    }
    else
    {
        FAIL();
    }
}

TEST(DiskShape, DefaultConstruction)
{
    DiskShape foo{};
    
    EXPECT_EQ(typeid(foo), typeid(DiskShape));
    EXPECT_EQ(foo.GetChildCount(), child_count_t{1});
    EXPECT_EQ(foo.GetRadius(), DiskShape::GetDefaultRadius());
    EXPECT_EQ(foo.GetLocation().x, Length{0});
    EXPECT_EQ(foo.GetLocation().y, Length{0});
}

TEST(DiskShape, InitConstruction)
{
    const auto radius = RealNum(1) * Meter;
    const auto position = Vec2{-1, 1} * Meter;
    auto conf = DiskShape::Conf{};
    conf.vertexRadius = radius;
    conf.location = position;
    DiskShape foo{conf};
    
    EXPECT_EQ(typeid(foo), typeid(DiskShape));
    EXPECT_EQ(foo.GetChildCount(), child_count_t{1});
    EXPECT_EQ(foo.GetRadius(), radius);
    EXPECT_EQ(foo.GetLocation().x, position.x);
    EXPECT_EQ(foo.GetLocation().y, position.y);
}

TEST(DiskShape, TestPoint)
{
    const auto radius = RealNum(1) * Meter;
    const auto position = Vec2{0, 0} * Meter;
    auto conf = DiskShape::Conf{};
    conf.vertexRadius = radius;
    conf.location = position;
    DiskShape foo{conf};
    EXPECT_TRUE(TestPoint(foo, Vec2{ 0,  0} * Meter));
    EXPECT_TRUE(TestPoint(foo, Vec2{+1,  0} * Meter));
    EXPECT_TRUE(TestPoint(foo, Vec2{ 0, +1} * Meter));
    EXPECT_TRUE(TestPoint(foo, Vec2{ 0, -1} * Meter));
    EXPECT_TRUE(TestPoint(foo, Vec2{-1,  0} * Meter));
    EXPECT_FALSE(TestPoint(foo, Vec2{-1,  -1} * Meter));
    EXPECT_FALSE(TestPoint(foo, Vec2{+1,  +1} * Meter));
    EXPECT_FALSE(TestPoint(foo, Vec2{+RealNum(0.9),  +RealNum(0.9)} * Meter));
}

TEST(DiskShape, ComputeAABB)
{
    const auto radius = RealNum(2.4) * Meter;
    const auto position = Vec2{2, 1} * Meter;
    auto conf = DiskShape::Conf{};
    conf.vertexRadius = radius;
    conf.location = position;
    DiskShape foo{conf};
    const auto aabb = ComputeAABB(foo, Transform_identity);
    EXPECT_EQ(aabb.GetLowerBound().x, position.x - radius);
    EXPECT_EQ(aabb.GetLowerBound().y, position.y - radius);
    EXPECT_EQ(aabb.GetUpperBound().x, position.x + radius);
    EXPECT_EQ(aabb.GetUpperBound().y, position.y + radius);
    EXPECT_TRUE(almost_equal(StripUnit(GetExtents(aabb).x), StripUnit(radius)));
    EXPECT_TRUE(almost_equal(StripUnit(GetExtents(aabb).y), StripUnit(radius)));
    EXPECT_EQ(GetCenter(aabb).x, position.x);
    EXPECT_EQ(GetCenter(aabb).y, position.y);
}