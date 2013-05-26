#pragma once
#include <stack>
#include <boost/shared_ptr.hpp>
#include "cinder/Cinder.h"
#include "cinderpane/cinder/Solarized.h"
#include "cinderpane/gizmo/fwd.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
	namespace gizmo {

		/// An interface for any class which can be rendered to a GizmoContext, either
		/// as a whole or a part of a parent gizmo.
		struct IRenderable
		{
			virtual ~IRenderable() {}

			virtual void updateGizmo() = 0;
			virtual void renderGizmo(GizmoContext &context) = 0;
		};

	}
} // end namespace cinderpane::gizmo

/// @}
