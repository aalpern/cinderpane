#pragma once
#include "zugster/cinder/Utils.h"
#include "zugster/gizmo/PartBase.h"

namespace zugster { 
	namespace gizmo {

		using namespace zugster::cinderext;
      
		/// A part which can render another part at a specific angle
		/// along the perimeter of a circle. Useful for "orbiting" anything
		/// renderable around a point.
		class PerimeterPart
			: public PartBase
		{
		  public:
			PerimeterPart()
				: PartBase(DEFAULT_RADIUS)
				, m_rotatePart(false)
			{
			}

			PerimeterPart(double radius, PartRef part, bool rotatePart = false)
				: PartBase(radius)
				, m_part(part)
				, m_rotatePart(rotatePart)
			{
			}

			virtual void updateGizmo()
			{
				PartBase::updateGizmo();
				if ( m_part )
					m_part->updateGizmo();
			}

			virtual void renderGizmo(GizmoContext &ctx)
			{
				if ( !m_part )
					return;
				GizmoSavePosition save(ctx);
				cinder::Vec2f point = perimeter(ctx.Position + m_position, m_size.x / 2, cinder::toRadians(m_rotation));
				ctx.Position = point;
				m_part->renderGizmo(ctx);	  
			}

			virtual void loadXML(const cinder::XmlTree &element, 
								 const PartFactoryRef &factory)
			{
				PartBase::loadXML(element, factory);
                const cinder::XmlTree &child = element.getChildren().front();
				m_part = factory->makePart(child.getTag());
				m_part->loadXML(child, factory);
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "Perimeter");
				m_part->storeXML(os);
				xmlEndTag(os, "Perimeter");
			}

		  protected:
			virtual void onRotationChanged(double rotation, double oldRotation)
			{
				if ( m_rotatePart )
					m_part->rotate(rotation - oldRotation);
			}

			bool    m_rotatePart;
			PartRef m_part;
		};

    }
} // end namespace zugster::gizmo
