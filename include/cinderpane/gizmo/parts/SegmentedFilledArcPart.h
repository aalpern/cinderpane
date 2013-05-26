#pragma once
#include <vector>
#include "cinderpane/cinder/Shapes.h"
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
	namespace gizmo {

		class SegmentedFilledArcPart
			: public PartBase
		{
		  public:
			SegmentedFilledArcPart(double outerRadius = 48,
								   double arcWidth = 8,
								   int segmentCount = 4,
								   double segmentGapDegrees = 1.0)
				: PartBase(outerRadius)
				, m_arcWidth(arcWidth)
				, m_segmentCount(segmentCount)
				, m_segmentGapDegrees(segmentGapDegrees)
			{
				m_fill = true;
				Calculate();
			}

			SegmentedFilledArcPart(const Vec2f &position,
								   double outerRadius,
								   double arcWidth,
								   int segmentCount, double segmentGapDegrees = 1.0)
				: PartBase(outerRadius, position)
				, m_arcWidth(arcWidth)
				, m_segmentCount(segmentCount)
				, m_segmentGapDegrees(segmentGapDegrees)
			{
				m_fill = true;
				Calculate();
			}

			SegmentedFilledArcPart(double outerRadius, double arcWidth, const double *segments, int count)
				: PartBase(outerRadius)
				, m_arcWidth(arcWidth)
			{
				m_fill = true;
				for ( int i = 0; i < count; i++ )
					m_segments.push_back(*(segments + i));
			}

			SegmentedFilledArcPart(const Vec2f &position, double outerRadius, double arcWidth, const double *segments, int count)
				: PartBase(outerRadius, position)
				, m_arcWidth(arcWidth)
			{
				m_fill = true;
				for ( int i = 0; i < count; i++ )
					m_segments.push_back(*(segments + i));
			}

			virtual ~SegmentedFilledArcPart()
			{
			}

			virtual void renderGizmo(GizmoContext &context)
			{
				if ( m_segments.size() > 0 )
				{
//					double begin = m_rotation;
//					double end = m_rotation;
					bool isGap = false;
					for ( std::vector<double>::iterator i = m_segments.begin(); i != m_segments.end(); i++ )
					{
						// TODO
						isGap = !isGap;
					}
				}
				else
				{
					double radius = m_size.x / 2;
					for ( int i = 0; i < m_segmentCount; i++ )
					{
						double begin = rotate_value((m_segmentSizeDegrees * i) + (m_segmentGapDegrees * i));
						double end   = rotate_value((m_segmentSizeDegrees * (i + 1.0)) + (m_segmentGapDegrees * i));

						hollow_arc(context.Cairo, context.Position + m_position, radius - m_arcWidth, radius,
									cinder::toRadians(begin), cinder::toRadians(end));
						context.draw(m_outline, m_fill, m_state);
					}
				}
			}

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory)
			{
				m_segmentCount      = xmlGetChildValue<int>(element, "SegmentCount", 4);
				m_segmentGapDegrees = xmlGetChildValue<double>(element, "SegmentGap", 45.0);
				m_arcWidth     = xmlGetChildValue<float>(element, "ArcWidth", 1.0f);
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "SegmentedFilledArc");
				xmlWriteTag(os, "SegmentCount", m_segmentCount);
				xmlWriteTag(os, "SegmentGap", m_segmentGapDegrees);
				xmlWriteTag(os, "ArcWidth", m_arcWidth);
				xmlEndTag(os, "SegmentedFilledArc");
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
			double m_arcWidth;
			std::vector<double> m_segments;
		};

	}
} // end namespace cinderpane::gizmo
