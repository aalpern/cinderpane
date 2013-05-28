#pragma once
#include "cinderpane/ext/Shapes.h"
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
  namespace gizmo {

    using namespace cinderpane::ext;

    //=============================================================================
    // FilledArcPart
    //=============================================================================

    // Just draws a single arc
    class FilledArcPart
        : public PartBase
    {
      public:
        FilledArcPart()
            : PartBase(DEFAULT_RADIUS)
            , m_arcWidth(12.0)
            , m_beginDegrees(0.0)
            , m_endDegrees(360.0)
        {
            m_fill = true;
        }

        FilledArcPart(double outerRadius,
                      double arcWidth,
                      double beginDegrees = 0.0,
                      double endDegrees = 360.0)
            : PartBase(outerRadius)
            , m_beginDegrees(beginDegrees)
            , m_endDegrees(endDegrees)
            , m_arcWidth(arcWidth)
        {
            m_fill = true;
            onRotationChanged(m_rotation, m_rotation);
        }

        FilledArcPart(const Vec2f &position,
                      double outerRadius,
                      double arcWidth,
                      double beginDegrees = 0.0,
                      double endDegrees = 360.0)
            : PartBase(outerRadius, position)
            , m_beginDegrees(beginDegrees)
            , m_endDegrees(endDegrees)
            , m_arcWidth(arcWidth)
        {
            m_fill = true;
            onRotationChanged(m_rotation, m_rotation);
        }

        virtual ~FilledArcPart()
        {
        }

        virtual void renderGizmo(GizmoContext &context)
        {
            double radius = m_size.x / 2;
            hollow_arc(context.Cairo,
                       context.Position + m_position,
                       radius - m_arcWidth,
                       radius,
                       m_beginRadians,
                       m_endRadians);
            context.draw(m_outline, m_fill, m_state);
        }

      protected:
        virtual void onRotationChanged(double rotation, double oldRotation)
        {
            m_beginRadians = ::cinder::toRadians( rotate_value(m_beginDegrees) );
            m_endRadians   = ::cinder::toRadians( rotate_value(m_endDegrees) );
        }

        double m_beginDegrees;
        double m_beginRadians;
        double m_endDegrees;
        double m_endRadians;
        double m_arcWidth;
    };

  }
} // end namespace cinderpane::gizmo
