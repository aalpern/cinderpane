#pragma once
#include "cinder/cairo/Cairo.h"
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
  namespace gizmo {

    class GradientCirclePart
        : public PartBase
    {
      public:
        GradientCirclePart()
            : PartBase(48.0)
            , m_gradientWidth(12.0)
            , m_innercolor(cinder::ColorAf(0.0, 0.0, 0.0, 0.0))
            , m_outercolor(cinder::ColorAf(1.0, 1.0, 1.0, 1.0))
        {
        }

        GradientCirclePart(float outerRadius,
                           float gradientWidth,
                           const cinder::ColorA &outercolor,
                           const cinder::ColorA &innercolor)
            : PartBase(outerRadius)
            , m_outercolor(outercolor)
            , m_innercolor(innercolor)
            , m_gradientWidth(gradientWidth)
        {
        }

        GradientCirclePart(const Vec2f &position,
                           float outerRadius,
                           float gradientWidth,
                           const cinder::ColorA &outercolor,
                           const cinder::ColorA &innercolor)
            : PartBase(outerRadius, position)
            , m_outercolor(outercolor)
            , m_innercolor(innercolor)
            , m_gradientWidth(gradientWidth)
        {
        }

        virtual ~GradientCirclePart() {}

        virtual void renderGizmo(GizmoContext &context)
        {
            CairoSaveState save(context.Cairo);
            Vec2f position = context.Position + m_position;
            float radius = m_size.x / 2;
            cinder::cairo::GradientRadial gradient(position, radius,
                                                   position, radius - m_gradientWidth);
            gradient.addColorStop(0, m_outercolor);
            gradient.addColorStop(1, m_innercolor);
            context.Cairo.newPath();
            context.Cairo.setSource(gradient);
            context.Cairo.circle(position, radius);
            context.Cairo.fill();
        }

      private:
        float m_gradientWidth;
        cinder::ColorA m_outercolor;
        cinder::ColorA m_innercolor;
    };

  }
} // end namespace cinderpane::gizmo
