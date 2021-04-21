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

#ifndef PLAYRHO_COLLISION_SHAPES_SHAPECONF_HPP
#define PLAYRHO_COLLISION_SHAPES_SHAPECONF_HPP

#include <PlayRho/Common/Units.hpp>
#include <PlayRho/Common/Finite.hpp>
#include <PlayRho/Common/Settings.hpp>
#include <PlayRho/Dynamics/Filter.hpp>

namespace playrho {
namespace d2 {

/// @brief Base configuration for initializing shapes.
/// @note This is a nested base value class for initializing shapes.
struct BaseShapeConf
{
    /// @brief Friction coefficient.
    ///
    /// @note This must be a value between 0 and +infinity. It is safer however to
    ///   keep the value below the square root of the max value of a Real.
    /// @note This is usually in the range [0,1].
    /// @note The square-root of the product of this value multiplied by a touching
    ///   fixture's friction becomes the friction coefficient for the contact.
    ///
    NonNegative<Real> friction = NonNegative<Real>{Real{2} / Real{10}};
    
    /// @brief Restitution (elasticity) of the associated shape.
    ///
    /// @note This should be a valid finite value.
    /// @note This is usually in the range [0,1].
    ///
    Finite<Real> restitution = Finite<Real>{0};
    
    /// @brief Area density of the associated shape.
    ///
    /// @note This must be a non-negative value.
    /// @note Use 0 to indicate that the shape's associated mass should be 0.
    ///
    NonNegative<AreaDensity> density = NonNegative<AreaDensity>{0_kgpm2};

    /// Contact filtering data.
    Filter filter;

    /// A sensor shape collects contact information but never generates a collision response.
    bool isSensor = false;
};

/// @brief Builder configuration structure.
/// @details This is a builder structure of chainable methods for building a shape
///   configuration.
/// @note This is a templated nested value class for initializing shapes that
///   uses the Curiously Recurring Template Pattern (CRTP) to provide method chaining
///   via static polymorphism.
/// @see https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
template <typename ConcreteConf>
struct ShapeBuilder: BaseShapeConf
{
    // Note: don't use 'using ShapeConf::ShapeConf' here as it doesn't work in this context!
    
    /// @brief Default constructor.
    constexpr ShapeBuilder() = default;

    /// @brief Initializing constructor.
    constexpr explicit ShapeBuilder(const BaseShapeConf& value) noexcept:
        BaseShapeConf{value}
    {
        // Intentionally empty.
    }
    
    /// @brief Uses the given friction.
    constexpr ConcreteConf& UseFriction(NonNegative<Real> value) noexcept;
    
    /// @brief Uses the given restitution.
    constexpr ConcreteConf& UseRestitution(Finite<Real> value) noexcept;
    
    /// @brief Uses the given density.
    constexpr ConcreteConf& UseDensity(NonNegative<AreaDensity> value) noexcept;

    /// @brief Uses the given filter.
    constexpr ConcreteConf& UseFilter(Filter value) noexcept;

    /// @brief Uses the given is-sensor value.
    constexpr ConcreteConf& UseIsSensor(bool value) noexcept;
};

template <typename ConcreteConf>
constexpr ConcreteConf&
ShapeBuilder<ConcreteConf>::UseFriction(NonNegative<Real> value) noexcept
{
    friction = value;
    return static_cast<ConcreteConf&>(*this);
}

template <typename ConcreteConf>
constexpr ConcreteConf&
ShapeBuilder<ConcreteConf>::UseRestitution(Finite<Real> value) noexcept
{
    restitution = value;
    return static_cast<ConcreteConf&>(*this);
}

template <typename ConcreteConf>
constexpr ConcreteConf&
ShapeBuilder<ConcreteConf>::UseDensity(NonNegative<AreaDensity> value) noexcept
{
    density = value;
    return static_cast<ConcreteConf&>(*this);
}

template <typename ConcreteConf>
constexpr ConcreteConf&
ShapeBuilder<ConcreteConf>::UseFilter(Filter value) noexcept
{
    filter = value;
    return static_cast<ConcreteConf&>(*this);
}

template <typename ConcreteConf>
constexpr ConcreteConf&
ShapeBuilder<ConcreteConf>::UseIsSensor(bool value) noexcept
{
    isSensor = value;
    return static_cast<ConcreteConf&>(*this);
}

/// @brief Shape configuration structure.
struct ShapeConf: public ShapeBuilder<ShapeConf>
{
    using ShapeBuilder::ShapeBuilder;
};

// Free functions...

/// @brief Gets the density of the given shape configuration.
/// @relatedalso BaseShapeConf
constexpr NonNegative<AreaDensity> GetDensity(const BaseShapeConf& arg) noexcept
{
    return arg.density;
}

/// @brief Gets the restitution of the given shape configuration.
/// @relatedalso BaseShapeConf
constexpr Finite<Real> GetRestitution(const BaseShapeConf& arg) noexcept
{
    return arg.restitution;
}

/// @brief Gets the friction of the given shape configuration.
/// @relatedalso BaseShapeConf
constexpr NonNegative<Real> GetFriction(const BaseShapeConf& arg) noexcept
{
    return arg.friction;
}

/// @brief Gets the filter of the given shape configuration.
/// @relatedalso BaseShapeConf
constexpr Filter GetFilter(const BaseShapeConf& arg) noexcept
{
    return arg.filter;
}

/// @relatedalso BaseShapeConf
inline void SetFilter(BaseShapeConf& arg, Filter value)
{
    arg.filter = value;
}

/// @brief Gets the is-sensor state of the given shape configuration.
/// @relatedalso BaseShapeConf
constexpr bool IsSensor(const BaseShapeConf& arg) noexcept
{
    return arg.isSensor;
}

/// @relatedalso BaseShapeConf
inline void SetSensor(BaseShapeConf& arg, bool value)
{
    arg.isSensor = value;
}

} // namespace d2
} // namespace playrho

#endif // PLAYRHO_COLLISION_SHAPES_SHAPECONF_HPP
