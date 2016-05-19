/*
* Copyright (c) 2006-2013 Erin Catto http://www.box2d.org
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

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <Box2D/Box2D.h>

namespace box2d {

struct GLRenderPoints;
struct GLRenderLines;
struct GLRenderTriangles;

struct b2AABB;

//
struct Camera
{
	Camera()
	{
		m_center = Vec2(0.0f, 20.0f);
		m_extent = 25.0f;
		m_zoom = 1.0f;
		m_width = 1280;
		m_height = 800;
	}

	Vec2 ConvertScreenToWorld(const Vec2& screenPoint);
	Vec2 ConvertWorldToScreen(const Vec2& worldPoint);
	void BuildProjectionMatrix(float_t* m, float_t zBias);

	Vec2 m_center;
	float_t m_extent;
	float_t m_zoom;
	int32 m_width;
	int32 m_height;
};

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class DebugDraw : public b2Draw
{
public:
	DebugDraw();
	~DebugDraw();

	void Create();
	void Destroy();
	
	virtual void DrawPolygon(const Vec2* vertices, size_type vertexCount, const b2Color& color) override;

	virtual void DrawSolidPolygon(const Vec2* vertices, size_type vertexCount, const b2Color& color) override;

	virtual void DrawCircle(const Vec2& center, float_t radius, const b2Color& color) override;

	virtual void DrawSolidCircle(const Vec2& center, float_t radius, const Vec2& axis, const b2Color& color) override;

	virtual void DrawSegment(const Vec2& p1, const Vec2& p2, const b2Color& color) override;

	virtual void DrawTransform(const b2Transform& xf) override;

    void DrawPoint(const Vec2& p, float_t size, const b2Color& color);

    void DrawString(int x, int y, const char* string, ...); 

    void DrawString(const Vec2& p, const char* string, ...);

    void DrawAABB(b2AABB* aabb, const b2Color& color);

    void Flush();
    
private:
	GLRenderPoints* m_points;
    GLRenderLines* m_lines;
    GLRenderTriangles* m_triangles;
};

extern DebugDraw g_debugDraw;
extern Camera g_camera;

} // namespace box2d

#endif
