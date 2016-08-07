/*
* Original work Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
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

#ifndef CANTILEVER_H
#define CANTILEVER_H

namespace box2d {

// It is difficult to make a cantilever made of links completely rigid with weld joints.
// You will have to use a high number of iterations to make them stiff.
// So why not go ahead and use soft weld joints? They behave like a revolute
// joint with a rotational spring.
class Cantilever : public Test
{
public:

	enum
	{
		e_count = 8
	};

	Cantilever()
	{
		Body* ground = nullptr;

		// Creates bottom ground
		{
			BodyDef bd;
			ground = m_world->Create(bd);

			EdgeShape shape;
			shape.Set(Vec2(-40.0f, 0.0f), Vec2(40.0f, 0.0f));
			ground->CreateFixture(FixtureDef{&shape, 0.0f});
		}

		// Creates left-end-fixed 8-part plank (below the top one)
		{
			PolygonShape shape;
			shape.SetAsBox(0.5f, 0.125f);

			FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			WeldJointDef jd;

			Body* prevBody = ground;
			for (int32 i = 0; i < e_count; ++i)
			{
				BodyDef bd;
				bd.type = BodyType::Dynamic;
				bd.position = Vec2(-14.5f + 1.0f * i, 5.0f);
				Body* body = m_world->Create(bd);
				body->CreateFixture(fd);

				Vec2 anchor(-15.0f + 1.0f * i, 5.0f);
				jd.Initialize(prevBody, body, anchor);
				m_world->Create(jd);

				prevBody = body;
			}
		}

		// Creates left-end-fixed 3-part plank at top
		{
			PolygonShape shape;
			shape.SetAsBox(1.0f, 0.125f);

			FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			WeldJointDef jd;
			jd.frequencyHz = 5.0f;
			jd.dampingRatio = 0.7f;

			Body* prevBody = ground;
			for (int32 i = 0; i < 3; ++i)
			{
				BodyDef bd;
				bd.type = BodyType::Dynamic;
				bd.position = Vec2(-14.0f + 2.0f * i, 15.0f);
				Body* body = m_world->Create(bd);
				body->CreateFixture(fd);

				Vec2 anchor(-15.0f + 2.0f * i, 15.0f);
				jd.Initialize(prevBody, body, anchor);
				m_world->Create(jd);

				prevBody = body;
			}
		}

		// Creates 8-part plank to the right of the fixed planks (but not farthest right)
		{
			PolygonShape shape;
			shape.SetAsBox(0.5f, 0.125f);

			FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			WeldJointDef jd;

			Body* prevBody = ground;
			for (int32 i = 0; i < e_count; ++i)
			{
				BodyDef bd;
				bd.type = BodyType::Dynamic;
				bd.position = Vec2(-4.5f + 1.0f * i, 5.0f);
				Body* body = m_world->Create(bd);
				body->CreateFixture(fd);

				if (i > 0)
				{
					Vec2 anchor(-5.0f + 1.0f * i, 5.0f);
					jd.Initialize(prevBody, body, anchor);
					m_world->Create(jd);
				}

				prevBody = body;
			}
		}

		// Creates 8-part farthest-right plank
		{
			PolygonShape shape;
			shape.SetAsBox(0.5f, 0.125f);

			FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			WeldJointDef jd;
			jd.frequencyHz = 8.0f;
			jd.dampingRatio = 0.7f;

			Body* prevBody = ground;
			for (int32 i = 0; i < e_count; ++i)
			{
				BodyDef bd;
				bd.type = BodyType::Dynamic;
				bd.position = Vec2(5.5f + 1.0f * i, 10.0f);
				Body* body = m_world->Create(bd);
				body->CreateFixture(fd);

				if (i > 0)
				{
					Vec2 anchor(5.0f + 1.0f * i, 10.0f);
					jd.Initialize(prevBody, body, anchor);
					m_world->Create(jd);
				}

				prevBody = body;
			}
		}

		// Creates triangles
		for (int32 i = 0; i < 2; ++i)
		{
			Vec2 vertices[3];
			vertices[0] = Vec2(-0.5f, 0.0f);
			vertices[1] = Vec2(0.5f, 0.0f);
			vertices[2] = Vec2(0.0f, 1.5f);

			PolygonShape shape;
			shape.Set(vertices, 3);

			FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;

			BodyDef bd;
			bd.type = BodyType::Dynamic;
			bd.position = Vec2(-8.0f + 8.0f * i, 12.0f);
			Body* body = m_world->Create(bd);
			body->CreateFixture(fd);
		}

		// Creates circles
		for (int32 i = 0; i < 2; ++i)
		{
			CircleShape shape;
			shape.SetRadius(float_t(0.5));

			FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;

			BodyDef bd;
			bd.type = BodyType::Dynamic;
			bd.position = Vec2(-6.0f + 6.0f * i, 10.0f);
			Body* body = m_world->Create(bd);
			body->CreateFixture(fd);
		}
	}

	static Test* Create()
	{
		return new Cantilever;
	}

	Body* m_middle;
};

} // namespace box2d

#endif
