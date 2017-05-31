/*
 * Original work Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/Box2D
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

/**
 * @file
 * @brief Real number definition file.
 * @details This file may have been autogenerated from the RealNum.hpp.in file.
 */

#ifndef RealNum_hpp
#define RealNum_hpp

namespace box2d
{

    /// @brief Real-number type.
    ///
    /// @details This is the number type underlying numerical calculations conceptually involving
    /// real-numbers. Ideally the implementation of this type doesn't suffer from things like:
    /// catastrophic cancellation, catastrophic division, overflows, nor underflows.
    ///
    /// @note This can be implemented using float, double, long double, Fixed32, or Fixed64.
    ///
    /// @note Regarding division:
    ///
    /// While dividing 1 by a RealNum, caching the result, and then doing multiplications with the
    /// result may well be faster (than repeatedly dividing), dividing 1 by RealNum can also result
    /// in an underflow situation that's then compounded every time it's multiplied with other
    /// values.
    ///
    /// Meanwhile, dividing every value by RealNum isolates any underflows to the particular
    /// division where underflow occurs.
    ///
    /// @warning The note regarding division applies even more so when using a fixed-point type
    /// (for RealNum).
    ///
    using RealNum = float;

}

#endif /* RealNum_hpp */
