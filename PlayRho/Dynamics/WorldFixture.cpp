/*
 * Original work Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2020 Louis Langholtz https://github.com/louis-langholtz/PlayRho
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <PlayRho/Dynamics/WorldFixture.hpp>

#include <PlayRho/Dynamics/World.hpp>
#include <PlayRho/Dynamics/WorldBody.hpp>

namespace playrho {
namespace d2 {

Filter GetFilterData(const World& world, FixtureID id)
{
    return world.GetFilterData(id);
}

void SetFilterData(World& world, FixtureID id, const Filter& value)
{
    world.SetFilterData(id, value);
}

void Refilter(World& world, FixtureID id)
{
    world.Refilter(id);
}

BodyID GetBody(const World& world, FixtureID id)
{
    return world.GetBody(id);
}

void* GetUserData(const World& world, FixtureID id)
{
    return world.GetUserData(id);
}

Transformation GetTransformation(const World& world, FixtureID id)
{
    return GetTransformation(world, GetBody(world, id));
}

Shape GetShape(const World& world, FixtureID id)
{
    return world.GetShape(id);
}

void SetSensor(World& world, FixtureID id, bool value)
{
    world.SetSensor(id, value);
}

bool IsSensor(const World& world, FixtureID id)
{
    return world.IsSensor(id);
}

AreaDensity GetDensity(const World& world, FixtureID id)
{
    return world.GetDensity(id);
}

const std::vector<FixtureProxy>& GetProxies(const World& world, FixtureID id)
{
    return world.GetProxies(id);
}

const FixtureProxy& GetProxy(const World& world, FixtureID id, ChildCounter child)
{
    return GetProxies(world, id).at(child);
}

bool TestPoint(const World& world, FixtureID id, Length2 p)
{
    return TestPoint(GetShape(world, id), InverseTransform(p, GetTransformation(world, id)));
}

} // namespace d2
} // namespace playrho
