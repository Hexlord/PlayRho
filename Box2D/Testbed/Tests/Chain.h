/*
* Original work Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
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

#ifndef CHAIN_H
#define CHAIN_H

namespace box2d {

class Chain : public Test
{
public:
	Chain()
	{
		Body* ground = nullptr;
		{
			BodyDef bd;
			ground = m_world->Create(bd);

			EdgeShape shape;
			shape.Set(Vec2(-40.0f, 0.0f), Vec2(40.0f, 0.0f));
			ground->CreateFixture(FixtureDef{&shape, 0.0f});
		}

		{
			PolygonShape shape;
			shape.SetAsBox(0.6f, 0.125f);

			FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;
			fd.friction = 0.2f;

			RevoluteJointDef jd;
			jd.collideConnected = false;

			const float_t y = 25.0f;
			Body* prevBody = ground;
			for (int32 i = 0; i < 30; ++i)
			{
				BodyDef bd;
				bd.type = BodyType::Dynamic;
				bd.position = Vec2(0.5f + i, y);
				Body* body = m_world->Create(bd);
				body->CreateFixture(fd);

				Vec2 anchor(float_t(i), y);
				jd.Initialize(prevBody, body, anchor);
				m_world->Create(jd);

				prevBody = body;
			}
		}
	}

	static Test* Create()
	{
		return new Chain;
	}
};

} // namespace box2d

#endif
