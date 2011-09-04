#pragma once
#include <vector>
#include "zugster/shared_ptr.h"
#include "cinder/Cinder.h"

/// @addtogroup Gizmo
/// @{

namespace zugster { 
	namespace gizmo {
		
		using cinder::Vec2f;
		using cinder::Rectf;
		using cinder::ColorAf;
		using cinder::ColorA;

		using boost::make_shared;

		struct IRenderable;
		typedef boost::shared_ptr<IRenderable> RenderableRef;
		
		struct IPart;
		typedef boost::shared_ptr<IPart> PartRef;
		typedef std::vector<PartRef>     PartVector;

		class GizmoContext;

		static const double DEFAULT_RADIUS = 48.0;
	} 
}

/// @}
