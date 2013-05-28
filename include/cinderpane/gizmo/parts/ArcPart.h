#pragma once
#include "cinderpane/gizmo/PartBase.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    class ArcPart
        : public PartBase
    {
      public:
        ArcPart()
            : PartBase(48.0)
            , m_lineWidth(1.0)
            , m_beginDegrees(0.0)
            , m_endDegrees(180.0)
        {
        }

        ArcPart(double outerRadius, double lineWidth,
                double beginDegrees = 0.0, double endDegrees = 180.0)
            : PartBase(outerRadius)
            , m_beginDegrees(beginDegrees)
            , m_endDegrees(endDegrees)
            , m_lineWidth(lineWidth)
        {
        }

        ArcPart(const Vec2f &position, double outerRadius, double lineWidth,
                double beginDegrees = 0.0, double endDegrees = 180.0)
            : PartBase(outerRadius, position)
            , m_beginDegrees(beginDegrees)
            , m_endDegrees(endDegrees)
            , m_lineWidth(lineWidth)
        {
        }


        virtual ~ArcPart()
        {
        }

        virtual void renderGizmo(GizmoContext &context)
        {
            context.Cairo.save();
            context.Cairo.setLineWidth(m_lineWidth);

            context.Cairo.arc( context.Position + m_position, m_size.x / 2,
                               cinder::toRadians(rotate_value(m_beginDegrees)),
                               cinder::toRadians(rotate_value(m_endDegrees)));

            context.draw(m_outline, m_fill, m_state);

            context.Cairo.restore();
        }

      protected:
        virtual void onSizedChanged(const Vec2f &size, const Vec2f &oldSize)
        {
            m_lineRadius = size.x - ( m_lineWidth / 2 );
        }

        double m_beginDegrees;
        double m_endDegrees;
        double m_lineWidth;
        double m_lineRadius;
    };

  }
} // end namespace cinderpane::gizmo

/// @}
