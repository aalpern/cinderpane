#pragma once
#include <math.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "cinder/cairo/Cairo.h"

/// @defgroup Ext
/// @ingroup Cinderpane

/// @addtogroup Ext
/// @{

namespace cinderpane {
  namespace ext {

    using cinder::RectT;
    using cinder::Vec2;
    using cinder::Vec2f;
    using cinder::Vec3;
    using cinder::Vec3f;

    /** Uniform interface for creating vector objects */
    inline Vec2f make_vec(float x, float y)
    {
        return Vec2f(x, y);
    }

    /** Uniform interface for creating vector objects */
    inline Vec2f make_vec(double x, double y)
    {
        return Vec2f((float)x, (float)y);
    }

    /** Uniform interface for creating vector objects */
    inline Vec3f make_vec(float x, float y, float z)
    {
        return cinder::Vec3f(x, y, z);
    }

    /** Uniform interface for creating vector objects */
    inline Vec3f make_vec(double x, double y, double z)
    {
        return Vec3f((float)x, (float)y, (float)z);
    }

    /// Add two numerical values together, wrapping the result to
    /// remain within a range. When the result passes the lower
    /// bound, it wraps around to the upper bound, and vice-versa.
    template<typename T>
    T wrap_value(T lowerLimit, T upperLimit, T value)
    {
        if ( value > upperLimit ) {
            // while ( value > upperLimit )
            value -= lowerLimit;
        }
        else if ( value < lowerLimit ) {
            // while ( value < lowerLimit )
            value += upperLimit;
        }
        return value;
    }

    /// Wrap a value around the boundaries for angles in degrees
    /// (0.0, 360.0)
    double wrap_degrees(double value)
    {
        return wrap_value<double>(0.0, 360.0, value);
    }

    /// Wrap a voud around the boundaries for angles in radians
    /// (0.0, 2 * PI)
    double wrap_radians(double value)
    {
        return wrap_value<double>(0.0, 2 * M_PI, value);
    }

    /// Increment an angle representing a rotation in degrees,
    /// wrapping the resulting value so that the result is always
    /// in the range of [0.0, 360.0]
    double rotate_degrees(double value, double rotation)
    {
        return wrap_degrees(value + rotation);
    }

    /// Increment an angle representing a rotation in radians,
    /// wrapping the resulting value so that the result is always
    /// in the range of [0.0, 2 * pi]
    double rotate_radians(double value, double rotation)
    {
        return wrap_radians(value + rotation);
    }

    /// Calculate the x,y coordinates of a point on the perimeter
    /// of a circle.
    ///
    /// @parameter origin The center of the circle
    /// @parameter radius That would be the...radius....of the circle?.
    /// @parameter angle  The angle in radians.
    Vec2f perimeter(const Vec2f &origin, double radius, double angle)
    {
        double x = origin.x + (cos(angle) * radius);
        double y = origin.y + (sin(angle) * radius);
        return make_vec(x, y);
    }

    template<typename T, typename U>
    inline RectT<T>
    inset_rect(const RectT<T> &rect, U xValue, U yValue)
    {
        return RectT<T>(rect.x1 + xValue,
                        rect.y1 + yValue,
                        rect.x2 - xValue,
                        rect.y2 - yValue);
    }

    template<typename T, typename U>
    inline RectT<T>
    offset_rect(const RectT<T> &rect, U xValue, U yValue)
    {
        return RectT<T>(rect.x1 + xValue,
                        rect.y1 + yValue,
                        rect.x2 + xValue,
                        rect.y2 + yValue);
    }

    template<typename T, typename U>
    inline RectT<T>
    expand_rect(const RectT<T> &rect, U xValue, U yValue)
    {
        return RectT<T>(rect.x1 - xValue,
                        rect.y1 - yValue,
                        rect.x2 + xValue,
                        rect.y2 + yValue);
    }

    //=============================================================================

    template<typename T>
    cinder::Vec2<T> parse_vec2(const std::string &str)
    {
        std::vector<std::string> parts;
        boost::algorithm::split(parts, str, boost::algorithm::is_any_of(","));
        cinder::Vec2<T> v;
        if ( parts.size() == 0 )
            return v;
        if ( parts.size() >= 1 ) {
            try {
                v.x = boost::lexical_cast<T>( parts[0] );
            } catch ( ... ) {}
        }
        if ( parts.size() >= 2 ) {
            try {
                v.y = boost::lexical_cast<T>( parts[1] );
            } catch ( ... ) {}
        }
        return v;
    }

    cinder::ColorA parse_colorA(const std::string &str, const cinder::ColorA &defaultValue = cinder::ColorA(0.5, 0.5, 0.5, 0.5))
    {
        std::vector<std::string> parts;
        boost::algorithm::split(parts, str, boost::algorithm::is_any_of(","));

        try {
            cinder::ColorA value;
            value.r = boost::lexical_cast<float>( parts[0] );
            value.g = boost::lexical_cast<float>( parts[1] );
            value.b = boost::lexical_cast<float>( parts[2] );
            value.a = boost::lexical_cast<float>( parts[3] );
            return value;
        } catch ( ... ) {
            return defaultValue;
        }
    }

    template<typename T>
    std::ostream& write_color(std::ostream &os, const cinder::ColorAT<T> &color)
    {
        return os << color.r << ","
                  << color.g << ","
                  << color.b << ","
                  << color.a;
    }

    class CairoSaveState
    {
      public:
        CairoSaveState(cinder::cairo::Context &ctx)
            : ctx(ctx) {
            ctx.save();
        }
        ~CairoSaveState() {
            ctx.restore();
        }
      private:
        cinder::cairo::Context &ctx;
    };

  }
} // end namespace cinderpane::ext

/// @}
