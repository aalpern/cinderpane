#pragma once
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
  namespace gizmo {

    enum RectangleType
    {
        RectangleNormal,
        RectangleRound,
        RectangleCurve
    };

    class RectanglePart
        : public PartBase
    {
      public:
        RectanglePart()
            : PartBase(48.0)
            , m_type(RectangleNormal)
        {
        }

        RectanglePart(double radius, bool outline = true, bool fill = false)
            : PartBase(make_vec(radius * 2, radius * 2))
            , m_type(RectangleNormal)
        {
            m_outline = outline;
            m_fill    = fill;
        }

        RectanglePart(const Vec2f &position, double radius, bool outline = true, bool fill = false)
            : PartBase(make_vec(radius * 2, radius * 2), position)
            , m_type(RectangleNormal)
        {
            m_outline = outline;
            m_fill    = fill;
        }

        RectanglePart(const Vec2f &size, bool outline = true, bool fill = false)
            : PartBase(size)
            , m_type(RectangleNormal)
        {
            m_outline = outline;
            m_fill    = fill;
        }

        RectanglePart(const Vec2f &position, const Vec2f &size, bool outline = true, bool fill = false)
            : PartBase(size, position)
            , m_type(RectangleNormal)
        {
            m_outline = outline;
            m_fill    = fill;
        }
        RectanglePart(const Vec2f &position, const Vec2f &size, RectangleType type, bool outline = true, bool fill = false)
            : PartBase(size, position)
            , m_type(type)
        {
            m_outline = outline;
            m_fill    = fill;
        }

        RectanglePart(double height, double width, bool outline = true, bool fill = false)
            : PartBase(make_vec(height, width))
            , m_type(RectangleNormal)
        {
            m_outline = outline;
            m_fill    = fill;
        }

        RectanglePart(const Vec2f &position, double height, double width, bool outline = true, bool fill = false)
            : PartBase(make_vec(height, width), position)
            , m_type(RectangleNormal)
        {
            m_outline = outline;
            m_fill    = fill;
        }

        virtual ~RectanglePart()
        {
        }

        void setRectangleType(RectangleType type)
        {
            m_type = type;
        }

        virtual void renderGizmo(GizmoContext &context)
        {
            Vec2f offset   = m_size / 2;
            Vec2f position = context.Position + m_position;
            Vec2f topLeft  = position - offset;

            switch ( m_type ) {
              case RectangleNormal:
                  context.Cairo.rectangle(topLeft.x, topLeft.y, m_size.x, m_size.y);
                  break;
              case RectangleRound:
                  round_rect(context.Cairo, topLeft, topLeft + m_size);
                  break;
              case RectangleCurve:
                  curve_rect(context.Cairo, topLeft, topLeft + m_size);
                  break;
            }

            context.draw(m_outline, m_fill, m_state);
        }

      private:
        RectangleType m_type;
    };

  }
} // end namespace cinderpane::gizmo
