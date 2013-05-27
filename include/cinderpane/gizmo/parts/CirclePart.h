#pragma once
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
  namespace gizmo {

    class CirclePart
        : public PartBase
    {
      public:
        CirclePart()
            : PartBase(48.0)
        {
        }

        CirclePart(double radius, bool outline = true, bool fill = false)
            : PartBase(radius)
        {
            m_outline = outline;
            m_fill = fill;
        }

        CirclePart(const Vec2f &position, double radius, bool outline = true, bool fill = false)
            : PartBase(radius, position)
        {
            m_outline = outline;
            m_fill = fill;
        }

        virtual ~CirclePart()
        {
        }

        virtual void renderGizmo(GizmoContext &context)
        {
            context.Cairo.circle(context.Position + m_position, m_size.x / 2);
            context.draw(m_outline, m_fill, m_state);
        }

        virtual void loadXML(const cinder::XmlTree &element,
                             const PartFactoryRef &factory)
        {
            PartBase::loadXML(element, factory);
        }

        virtual void storeXML(std::ostream &os) const
        {
            xmlBeginPart(os, "Circle");
            xmlEndTag(os, "Circle");
        }
    };

  }
} // end namespace cinderpane::gizmo
