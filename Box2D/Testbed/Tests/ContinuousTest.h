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

#ifndef CONTINUOUS_TEST_H
#define CONTINUOUS_TEST_H

namespace box2d {

class ContinuousTest : public Test
{
public:

	ContinuousTest()
	{
		{
			BodyDef bd;
			bd.position = Vec2(0.0f, 0.0f);
			Body* body = m_world->CreateBody(&bd);

			EdgeShape edge;

			edge.Set(Vec2(-10.0f, 0.0f), Vec2(10.0f, 0.0f));
			body->CreateFixture(&edge, 0.0f);

			PolygonShape shape;
			shape.SetAsBox(0.2f, 1.0f, Vec2(0.5f, 1.0f), 0.0f);
			body->CreateFixture(&shape, 0.0f);
		}

#if 1
		{
			BodyDef bd;
			bd.type = DynamicBody;
			bd.position = Vec2(0.0f, 20.0f);
			//bd.angle = 0.1f;

			PolygonShape shape;
			shape.SetAsBox(2.0f, 0.1f);

			m_body = m_world->CreateBody(&bd);
			m_body->CreateFixture(&shape, 1.0f);

			m_angularVelocity = RandomFloat(-50.0f, 50.0f);
			//m_angularVelocity = 46.661274f;
			m_body->SetLinearVelocity(Vec2(0.0f, -100.0f));
			m_body->SetAngularVelocity(m_angularVelocity);
		}
#else
		{
			BodyDef bd;
			bd.type = DynamicBody;
			bd.position = Vec2(0.0f, 2.0f);
			Body* body = m_world->CreateBody(&bd);

			CircleShape shape;
			shape.m_p.SetZero();
			shape.m_radius = 0.5f;
			body->CreateFixture(&shape, 1.0f);

			bd.bullet = true;
			bd.position = Vec2(0.0f, 10.0f);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&shape, 1.0f);
			body->SetLinearVelocity(Vec2(0.0f, -100.0f));
		}
#endif

		extern int32 gjkCalls, gjkIters, gjkMaxIters;
		extern int32 toiCalls, toiIters;
		extern int32 toiRootIters, toiMaxRootIters;
		extern float_t toiTime, toiMaxTime;

		gjkCalls = 0; gjkIters = 0; gjkMaxIters = 0;
		toiCalls = 0; toiIters = 0;
		toiRootIters = 0; toiMaxRootIters = 0;
		toiTime = 0.0f; toiMaxTime = 0.0f;
	}

	void Launch()
	{
		extern int32 gjkCalls, gjkIters, gjkMaxIters;
		extern int32 toiCalls, toiIters;
		extern int32 toiRootIters, toiMaxRootIters;
		extern float_t toiTime, toiMaxTime;

		gjkCalls = 0; gjkIters = 0; gjkMaxIters = 0;
		toiCalls = 0; toiIters = 0;
		toiRootIters = 0; toiMaxRootIters = 0;
		toiTime = 0.0f; toiMaxTime = 0.0f;

		m_body->SetTransform(Vec2(0.0f, 20.0f), 0.0f);
		m_angularVelocity = RandomFloat(-50.0f, 50.0f);
		m_body->SetLinearVelocity(Vec2(0.0f, -100.0f));
		m_body->SetAngularVelocity(m_angularVelocity);
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		extern int32 gjkCalls, gjkIters, gjkMaxIters;

		if (gjkCalls > 0)
		{
			g_debugDraw.DrawString(5, m_textLine, "gjk calls = %d, ave gjk iters = %3.1f, max gjk iters = %d",
				gjkCalls, gjkIters / float_t(gjkCalls), gjkMaxIters);
			m_textLine += DRAW_STRING_NEW_LINE;
		}

		extern int32 toiCalls, toiIters;
		extern int32 toiRootIters, toiMaxRootIters;
		extern float_t toiTime, toiMaxTime;

		if (toiCalls > 0)
		{
			g_debugDraw.DrawString(5, m_textLine, "toi calls = %d, ave [max] toi iters = %3.1f [%d]",
								toiCalls, toiIters / float_t(toiCalls), toiMaxRootIters);
			m_textLine += DRAW_STRING_NEW_LINE;
			
			g_debugDraw.DrawString(5, m_textLine, "ave [max] toi root iters = %3.1f [%d]",
				toiRootIters / float_t(toiCalls), toiMaxRootIters);
			m_textLine += DRAW_STRING_NEW_LINE;

			g_debugDraw.DrawString(5, m_textLine, "ave [max] toi time = %.1f [%.1f] (microseconds)",
				1000.0f * toiTime / float_t(toiCalls), 1000.0f * toiMaxTime);
			m_textLine += DRAW_STRING_NEW_LINE;
		}

		if (m_stepCount % 60 == 0)
		{
			//Launch();
		}
	}

	static Test* Create()
	{
		return new ContinuousTest;
	}

	Body* m_body;
	float_t m_angularVelocity;
};

} // namespace box2d

#endif
