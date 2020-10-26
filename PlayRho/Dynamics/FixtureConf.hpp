/*
 * Original work Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
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

#ifndef PLAYRHO_DYNAMICS_FIXTURECONF_HPP
#define PLAYRHO_DYNAMICS_FIXTURECONF_HPP

/// @file
/// Declarations of the FixtureConf struct and any free functions associated with it.

#include <PlayRho/Dynamics/Filter.hpp>
#include <PlayRho/Dynamics/BodyID.hpp>
#include <PlayRho/Collision/Shapes/Shape.hpp>

#include <utility> // for std::move

namespace playrho {
namespace d2 {

class Fixture;

/// @brief Fixture definition.
/// @details A fixture is used to attach a shape to a body for collision detection. A fixture
///   inherits its transform from its body. Fixtures hold additional non-geometric data
///   such as collision filters, etc.
/// @ingroup PhysicalEntities
/// @see World::CreateFixture, World::GetFixture, World::SetFixture, World::Destroy.
struct FixtureConf {
    FixtureConf& UseShape(Shape value) noexcept
    {
        shape = std::move(value);
        return *this;
    }

    FixtureConf& UseBody(BodyID value) noexcept
    {
        body = value;
        return *this;
    }

    /// @brief Uses the given sensor state value.
    FixtureConf& UseIsSensor(bool value) noexcept
    {
        isSensor = value;
        return *this;
    }

    /// @brief Uses the given filter value.
    FixtureConf& UseFilter(Filter value) noexcept
    {
        filter = value;
        return *this;
    }

    Shape shape;

    /// Contact filtering data.
    Filter filter;

    BodyID body = InvalidBodyID;

    /// A sensor shape collects contact information but never generates a collision
    /// response.
    bool isSensor = false;
};

/// @brief Gets the body of the given configuration.
inline BodyID GetBody(const FixtureConf& conf) noexcept
{
    return conf.body;
}

/// @brief Gets the shape of the given configuration.
inline const Shape& GetShape(const FixtureConf& conf) noexcept
{
    return conf.shape;
}

/// @brief Gets the density of the given configuration.
inline NonNegative<AreaDensity> GetDensity(const FixtureConf& conf) noexcept
{
    return GetDensity(GetShape(conf));
}

/// @brief Gets the friction of the given configuration.
inline Real GetFriction(const FixtureConf& conf) noexcept
{
    return GetFriction(GetShape(conf));
}

/// @brief Gets the restitution of the given configuration.
inline Real GetRestitution(const FixtureConf& conf) noexcept
{
    return GetRestitution(GetShape(conf));
}

/// @brief Gets whether or not the given configuration is a sensor.
inline bool IsSensor(const FixtureConf& conf) noexcept
{
    return conf.isSensor;
}

/// @brief Sets whether or not the given configuration is a sensor.
inline void SetSensor(FixtureConf& conf, bool value) noexcept
{
    conf.isSensor = value;
}

/// @brief Gets the filter-data of the given configuration.
inline Filter GetFilterData(const FixtureConf& conf) noexcept
{
    return conf.filter;
}

/// @brief Sets the filter-data of the given configuration.
inline void SetFilterData(FixtureConf& conf, Filter value) noexcept
{
    conf.filter = value;
}

/// @brief Whether contact calculations should be performed between the two fixtures.
/// @return <code>true</code> if contact calculations should be performed between these
///   two fixtures; <code>false</code> otherwise.
/// @relatedalso Fixture
inline bool ShouldCollide(const FixtureConf& fixtureA, const FixtureConf& fixtureB) noexcept
{
    return ShouldCollide(GetFilterData(fixtureA), GetFilterData(fixtureB));
}

/// @brief Gets the default friction amount for the given fixtures.
Real GetDefaultFriction(const FixtureConf& fixtureA, const FixtureConf& fixtureB);

/// @brief Gets the default restitution amount for the given fixtures.
Real GetDefaultRestitution(const FixtureConf& fixtureA, const FixtureConf& fixtureB);

} // namespace d2
} // namespace playrho

#endif // PLAYRHO_DYNAMICS_FIXTURECONF_HPP
