/*
* Original work Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
* Modified work Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/PlayRho
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

#ifndef PLAYRHO_COMPOUND_SHAPES_HPP
#define  PLAYRHO_COMPOUND_SHAPES_HPP

#include "../Framework/Test.hpp"

namespace testbed {

// TODO_ERIN test joints on compounds.
class CompoundShapes : public Test
{
public:
    CompoundShapes()
    {
        CreateFixture(m_world, CreateBody(m_world),
                              Shape{EdgeShapeConf{Vec2(50.0f, 0.0f) * 1_m, Vec2(-50.0f, 0.0f) * 1_m}});
        
        {
            auto conf = DiskShapeConf{};
            conf.vertexRadius = 0.5_m;
            
            conf.location = Vec2{-0.5f, 0.5f} * 1_m;
            const auto circle1 = Shape{DiskShapeConf(conf).UseDensity(2_kgpm2)};
            conf.location = Vec2{0.5f, 0.5f} * 1_m;
            const auto circle2 = Shape(conf);
            for (auto i = 0; i < 10; ++i)
            {
                const auto x = RandomFloat(-0.1f, 0.1f);
                BodyConf bd;
                bd.type = BodyType::Dynamic;
                bd.location = Vec2(x + 5.0f, 1.05f + 2.5f * i) * 1_m;
                bd.angle = 1_rad * RandomFloat(-Pi, Pi);
                const auto body = CreateBody(m_world, bd);
                CreateFixture(m_world, body, circle1);
                CreateFixture(m_world, body, circle2);
            }
        }

        {
            auto conf = PolygonShapeConf{};
            conf.UseDensity(2_kgpm2);
            conf.SetAsBox(0.25_m, 0.5_m);
            const auto polygon1 = Shape{conf};
            conf.UseDensity(2_kgpm2);
            conf.SetAsBox(0.25_m, 0.5_m, Vec2(0.0f, -0.5f) * 1_m, 0.5_rad * Pi);
            const auto polygon2 = Shape{conf};
            for (int i = 0; i < 10; ++i)
            {
                const auto x = RandomFloat(-0.1f, 0.1f);
                BodyConf bd;
                bd.type = BodyType::Dynamic;
                bd.location = Vec2(x - 5.0f, 1.05f + 2.5f * i) * 1_m;
                bd.angle = 1_rad * RandomFloat(-Pi, Pi);
                const auto body = CreateBody(m_world, bd);
                CreateFixture(m_world, body, polygon1);
                CreateFixture(m_world, body, polygon2);
            }
        }

        {
            Transformation xf1;
            xf1.q = UnitVec::Get(0.3524_rad * Pi);
            xf1.p = GetVec2(GetXAxis(xf1.q)) * 1_m;

            auto triangleConf1 = PolygonShapeConf{};
            triangleConf1.Set({
                Transform(Vec2(-1.0f, 0.0f) * 1_m, xf1),
                Transform(Vec2(1.0f, 0.0f) * 1_m, xf1),
                Transform(Vec2(0.0f, 0.5f) * 1_m, xf1)
            });
            triangleConf1.UseDensity(2_kgpm2);
            const auto triangle1 = Shape(triangleConf1);

            Transformation xf2;
            xf2.q = UnitVec::Get(-0.3524_rad * Pi);
            xf2.p = -GetVec2(GetXAxis(xf2.q)) * 1_m;

            auto trianglConf2 = PolygonShapeConf{};
            trianglConf2.Set({
                Transform(Vec2(-1.0f, 0.0f) * 1_m, xf2),
                Transform(Vec2(1.0f, 0.0f) * 1_m, xf2),
                Transform(Vec2(0.0f, 0.5f) * 1_m, xf2)
            });
            trianglConf2.UseDensity(2_kgpm2);
            const auto triangle2 = Shape(trianglConf2);

            for (auto i = 0; i < 10; ++i)
            {
                const auto x = RandomFloat(-0.1f, 0.1f);
                BodyConf bd;
                bd.type = BodyType::Dynamic;
                bd.location = Vec2(x, 2.05f + 2.5f * i) * 1_m;
                bd.angle = 0_rad;
                const auto body = CreateBody(m_world, bd);
                CreateFixture(m_world, body, triangle1);
                CreateFixture(m_world, body, triangle2);
            }
        }

        {
            auto conf = PolygonShapeConf{};
            conf.UseDensity(4_kgpm2);
            conf.SetAsBox(1.5_m, 0.15_m);
            const auto bottom = Shape{conf};
            conf.SetAsBox(0.15_m, 2.7_m, Vec2(-1.45f, 2.35f) * 1_m, +0.2_rad);
            const auto left = Shape{conf};
            conf.UseDensity(4_kgpm2);
            conf.SetAsBox(0.15_m, 2.7_m, Vec2(1.45f, 2.35f) * 1_m, -0.2_rad);
            const auto right = Shape{conf};

            BodyConf bd;
            bd.type = BodyType::Dynamic;
            bd.location = Vec2( 0.0f, 2.0f ) * 1_m;
            const auto body = CreateBody(m_world, bd);
            CreateFixture(m_world, body, bottom);
            CreateFixture(m_world, body, left);
            CreateFixture(m_world, body, right);
        }
        
        SetAccelerations(m_world, GetGravity());
    }
};

} // namespace testbed

#endif
