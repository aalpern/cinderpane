#pragma once
#include "cinderpane/ext/Shapes.h"
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
  namespace gizmo {

    class SegmentedArcPart
        : public PartBase
    {
      public:
        SegmentedArcPart()
            : PartBase(48.0)
            , m_lineWidth(8.0)
            , m_segmentCount(4)
            , m_segmentGapDegrees(60.0)
        {
        }

        SegmentedArcPart(double radius, double lineWidth, int segmentCount, double segmentGapDegrees = 1.0)
            : PartBase(radius)
            , m_segmentCount(segmentCount)
            , m_segmentGapDegrees(segmentGapDegrees)
            , m_lineWidth(lineWidth)
        {
            Calculate();
        }

        SegmentedArcPart(const Vec2f &position, double radius, double lineWidth, int segmentCount, double segmentGapDegrees = 1.0)
            : PartBase(radius, position)
            , m_segmentCount(segmentCount)
            , m_segmentGapDegrees(segmentGapDegrees)
            , m_lineWidth(lineWidth)
        {
            Calculate();
        }

        virtual ~SegmentedArcPart()
        {
        }

        virtual void renderGizmo(GizmoContext &context)
        {
            CairoSaveState save(context.Cairo);
            context.Cairo.setLineWidth(m_lineWidth);
            context.Cairo.newPath();

            double radius = m_size.x / 2;
            for ( int i = 0; i < m_segmentCount; i++ )
            {
                double begin = rotate_value((m_segmentSizeDegrees * i) + (m_segmentGapDegrees * i));
                double end   = rotate_value((m_segmentSizeDegrees * (i + 1.0)) + (m_segmentGapDegrees * i));
                context.Cairo.newPath();
                context.Cairo.arc(context.Position + m_position, radius, ::cinder::toRadians(begin), ::cinder::toRadians(end));
                context.draw(true, false, m_state);
            }
        }

        virtual void loadXML(const cinder::XmlTree &element,
                             const PartFactoryRef &factory)
        {
            m_segmentCount      = xmlGetChildValue<int>(element, "SegmentCount", 4);
            m_segmentGapDegrees = xmlGetChildValue<double>(element, "SegmentGap", 45.0);
            Calculate();
        }

        virtual void storeXML(std::ostream &os) const
        {
            xmlBeginPart(os, "SegmentedArc");
            xmlWriteTag(os, "SegmentCount", m_segmentCount);
            xmlWriteTag(os, "SegmentGap", m_segmentGapDegrees);
            xmlEndTag(os, "SegmentedArc");
        }

      protected:
        void Calculate()
        {
            m_segmentSizeDegrees
                = (360.0 - ( m_segmentGapDegrees * m_segmentCount))
                / m_segmentCount;
        }

        int    m_segmentCount;
        double m_segmentGapDegrees;
        double m_segmentSizeDegrees;
        double m_lineWidth;
    };

  }
} // end namespace cinderpane::gizmo
