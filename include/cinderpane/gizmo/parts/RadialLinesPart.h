#pragma once
#include "cinderpane/cinder/Utils.h"
#include "cinderpane/gizmo/PartBase.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
	namespace gizmo {

		using namespace cinderpane::ext;

		class RadialLinesPart
			: public PartBase
		{
		  public:
			RadialLinesPart()
				: PartBase(48.0)
				, m_beginDegrees(0.0)
				, m_endDegrees(360.0)
				, m_lineLength(24.0)
				, m_lineCount(12)
			{
			}

			RadialLinesPart(float outerRadius, float lineLength, int lineCount,
							float beginDegrees = 0.0, float endDegrees = 360.0)
				: PartBase(outerRadius)
				, m_beginDegrees(beginDegrees)
				, m_endDegrees(endDegrees)
				, m_lineLength(lineLength)
				, m_lineCount(lineCount)
			{
				Calculate();
			}

			RadialLinesPart(const Vec2f &position, float outerRadius, float lineLength, int lineCount,
							float beginDegrees = 0.0, float endDegrees = 360.0)
				: PartBase(outerRadius, position)
				, m_beginDegrees(beginDegrees)
				, m_endDegrees(endDegrees)
				, m_lineLength(lineLength)
				, m_lineCount(lineCount)
			{
				Calculate();
			}

			virtual ~RadialLinesPart()
			{
			}

			int getLineCount() const { return m_lineCount; }

			void setLineCount(int value)
			{
				m_lineCount = value;
				Calculate();
			}

			virtual void renderGizmo(GizmoContext &context)
			{
				double outer_radius = m_size.x / 2;
				double inner_radius = outer_radius - m_lineLength;
				double begin  = rotate_value(m_beginDegrees);
				Vec2f position = context.Position + m_position;

				for ( int i = 0; i <= m_lineCount; i++ )
				{
					double angle = cinder::toRadians(begin + ( i * m_incrementDegrees ));
					Vec2f outer = perimeter(position, outer_radius, angle);
					Vec2f inner = perimeter(position, inner_radius, angle);
					context.Cairo.moveTo(outer);
					context.Cairo.lineTo(inner);
				}
				context.draw(true, false, m_state);
			}

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory)
			{
				PartBase::loadXML(element, factory);
				m_beginDegrees = xmlGetChildValue<float>(element, "BeginDegrees",  0.0);
				m_endDegrees   = xmlGetChildValue<float>(element, "EndDegrees",  180.0);
				m_lineCount    = xmlGetChildValue<int>(  element, "LineCount",    12);
				m_lineLength   = xmlGetChildValue<float>(element, "LineLength",    8.0);
				Calculate();
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "RadialLines");
				xmlWriteTag(os, "BeginDegrees", m_beginDegrees);
				xmlWriteTag(os, "EndDegrees",   m_endDegrees);
				xmlWriteTag(os, "LineCount",    m_lineCount);
				xmlWriteTag(os, "LineLength",   m_lineLength);
				xmlEndTag(os, "RadialLines");
			}

		  protected:
			void Calculate()
			{
				m_incrementDegrees = ( m_endDegrees - m_beginDegrees ) / m_lineCount;
			}

			float m_beginDegrees;
			float m_endDegrees;
			float m_lineLength;
			float m_incrementDegrees;
			int   m_lineCount;
		};

	}
} // end namespace cinderpane::gizmo

/// @}
