#pragma once
#include "zugster/gizmo/fwd.h"
#include "zugster/gizmo/Part.h"

/// @addtogroup Gizmo
/// @{

namespace zugster {
	namespace gizmo {
		
		struct ITheme
		{
			virtual ~ITheme() {}

			virtual ColorA getLineColor(IPart::DisplayState state, bool selected) const = 0;
			virtual ColorA getFillColor(IPart::DisplayState state, bool selected) const = 0;
			virtual ColorA getTextColor(IPart::DisplayState state, bool selected) const = 0;
		};
		typedef boost::shared_ptr<ITheme> ThemeRef;
		
	}
}

/// @}
