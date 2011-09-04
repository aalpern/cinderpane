#pragma once
#include <boost/shared_ptr.hpp>
#include "zugster/gizmo/CompositePart.h"

/// @addtogroup Gizmo
/// @{

namespace zugster { 
	namespace gizmo {
    
		/// The basic gizmo class is just a position and a container for a bunch
		/// of renderable sub-objects. 
		class Gizmo
			: public CompositePart
		{
			typedef CompositePart base_type;
		  public:
			Gizmo() {}
			Gizmo(double x, double y)
				: base_type(make_vec(0.0, 0.0), make_vec(x, y)) {}
			Gizmo(const cinder::Vec2f &position)
				: base_type(make_vec(0.0, 0.0), position) {}
			Gizmo(const cinder::Vec2f &position, const Vec2f &size)
				: base_type(size, position) {}
			
			virtual ~Gizmo() {}

			virtual void renderGizmo(GizmoContext &context)
			{
				if ( !isVisible() )
					return;
				context.save();
				context.Position = m_position;
				base_type::renderGizmo(context);
				context.restore();
			}			
			
			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "Gizmo");
				base_type::storeXML(os);
				xmlEndTag(os, "Gizmo");
			}
		};
		typedef boost::shared_ptr<Gizmo> GizmoRef;

	} 
} // end namespace zugster::gizmo

/// @}
