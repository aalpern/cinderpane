#pragma once
#include "cinderpane/gizmo/PartBase.h"
#include "cinderpane/gizmo/parts/RadialLinesPart.h"

namespace cinderpane {
  namespace gizmo {

    class RadialGridPart
        : public PartBase
    {
      public:
        RadialGridPart()
            : PartBase(48.0)
            , m_circleCount(4)
            , m_lines(48.0, 8.0, 12)
        {
        }

        RadialGridPart(double radius, int circleCount, int lineCount, bool fill = false)
            : PartBase(radius)
            , m_circleCount(circleCount)
            , m_lines(radius, (int)radius, lineCount)
        {
            m_fill = fill;
            if ( m_circleCount == 0 )
                m_circleCount = 1;
            // circleCount includes the outermost
            m_circleSpacing = radius / circleCount;
        }

        virtual ~RadialGridPart()
        {
        }

        virtual void renderGizmo(GizmoContext &context)
        {
            Vec2f position = context.Position + m_position;
            float radius = m_size.x / 2;
            context.Cairo.circle(position, radius);

            for ( int i = 1; i < m_circleCount; i++ )
            {
                context.Cairo.circle(position, radius - ( m_circleSpacing * i ));
            }

            context.draw(true, m_fill, m_state);

            m_lines.renderGizmo(context);
        }

        virtual void setDisplayState(DisplayState state)
        {
            PartBase::setDisplayState(state);
            m_lines.setDisplayState(state);
        }

        virtual void loadXML(const cinder::XmlTree &element,
                             const PartFactoryRef &factory)
        {
            PartBase::loadXML(element, factory);
            m_circleSpacing = xmlGetChildValue<float>(element, "CircleSpacing", m_circleSpacing);
            m_circleCount = xmlGetChildValue<int>(element, "CircleCount", m_circleCount);
            m_lines.setLineCount( xmlGetChildValue<int>(element, "LineCount", m_lines.getLineCount()) );
            m_lines.setRotation(m_rotation);
            m_lines.setRotationSpeed(m_rotationSpeedRPM);
        }

        virtual void storeXML(std::ostream &os) const
        {
            xmlBeginPart(os, "RadialGrid");
            xmlWriteTag(os, "CircleSpacing", m_circleSpacing);
            xmlWriteTag(os, "CircleCount", m_circleCount);
            xmlWriteTag(os, "LineCount", m_lines.getLineCount());
            xmlEndTag(os, "RadialGrid");
        }

      protected:
        virtual void onRotationChanged(double rotation, double oldRotation)
        {
            m_lines.setRotation(rotation);
        }

        virtual void onPositionChanged(const Vec2f &pos, const Vec2f &old)
        {
            m_lines.setPosition(pos);
        }

        double          m_circleSpacing;
        int             m_circleCount;
        RadialLinesPart m_lines;
    };

  }
} // end namespace cinderpane::gizmo
