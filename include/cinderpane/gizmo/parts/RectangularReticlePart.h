#pragma once
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
	namespace gizmo {

		class RectangularReticlePart
			: public PartBase
		{
		  public:
			RectangularReticlePart()
				: PartBase(48.0)
				, m_length(8.0)
			{
			}

			RectangularReticlePart(double radius, float reticleLength)
				: PartBase(make_vec(radius * 2, radius * 2))
				, m_length(reticleLength)
			{
			}

			RectangularReticlePart(const Vec2f &size, float reticleLength)
				: PartBase(size)
				, m_length(reticleLength)
			{
			}

			RectangularReticlePart(double height, double width, float reticleLength)
				: PartBase(make_vec(height, width))
				, m_length(reticleLength)
			{
			}

			virtual ~RectangularReticlePart()
			{
			}

			virtual void renderGizmo(GizmoContext &context)
			{
				// Vec2f position = context.Position + m_position;
#if 0
				cinderpane::ext::square_reticle(context.Cairo,
												   position.x - (m_size.x / 2),
												   position.y - (m_size.y / 2),
												   position.x + (m_size.x / 2),
												   position.y + (m_size.y / 2),
												   m_length);
#endif
			}

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory)
			{
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "RectangularReticle");
				xmlEndTag(os, "RectangularReticle");
			}

		  private:
			float m_length;
		};

	}
} // end namespace cinderpane::gizmo
