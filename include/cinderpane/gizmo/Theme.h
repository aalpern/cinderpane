#pragma once
#include "cinderpane/gizmo/fwd.h"
#include "cinderpane/gizmo/Part.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
	namespace gizmo {

		struct ITheme
		{
			virtual ~ITheme() {}

			virtual ColorA getLineColor(IPart::DisplayState state, bool selected) const = 0;
			virtual ColorA getFillColor(IPart::DisplayState state, bool selected) const = 0;
			virtual ColorA getTextColor(IPart::DisplayState state, bool selected) const = 0;
		};
		typedef std::shared_ptr<ITheme> ThemeRef;

	}
}

/// @}
