#pragma once
#include <math.h>
#include "cinderpane/cinder/Shapes.h"
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
	namespace gizmo {

		using namespace cinderpane::ext;

		class RegularPolygonPart
			: public PartBase
		{
		  public:
			RegularPolygonPart()
				: PartBase(48.0)
				, m_sideCount(5)
			{
			}

			RegularPolygonPart(float radius, int sideCount, bool outline = true, bool fill = false)
				: PartBase(radius)
				, m_sideCount(sideCount)
			{
				m_outline = outline;
				m_fill    = fill;
			}

			RegularPolygonPart(const Vec2f &position, float radius, int sideCount, bool outline = true, bool fill = false)
				: PartBase(radius, position)
				, m_sideCount(sideCount)
			{
				m_outline = outline;
				m_fill    = fill;
			}

			virtual ~RegularPolygonPart() {}

			virtual void renderGizmo(GizmoContext &ctx)
			{
				Vec2f position = ctx.Position + m_position;
				double increment = 360.0 / m_sideCount;
				double radius    = m_size.x / 2;

				// Starting point
				double angle = cinder::toRadians( rotate_value( 0.0 ) );
				Vec2f point = perimeter(position, radius, angle);
				ctx.Cairo.moveTo(point);

				for ( int i = 1; i < m_sideCount; i++ )
				{
					angle = cinder::toRadians( rotate_value( i * increment ) );
					point = perimeter(position, radius, angle);
					ctx.Cairo.lineTo(point);
				}
				ctx.Cairo.closePath();
/*
				if ( m_fill )
				{
					ctx.Cairo.save();
					ctx.Cairo.setSource(ctx.FillColor);
					ctx.Cairo.fillPreserve();
					ctx.Cairo.restore();
				}
				if ( m_outline )
					ctx.Cairo.stroke();
					ctx.Cairo.newPath(); */
				ctx.draw(m_outline, m_fill, m_state);
			}

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory)
			{
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "RegularPolygon");
				xmlEndTag(os, "RegularPolygon");
			}

		  protected:
			int m_sideCount;
		};


	}
} // end namespace cinderpane::gizmo
