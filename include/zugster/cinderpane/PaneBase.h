#pragma once
#include "zugster/cinderpane/PositionableBase.h"

// Disable 'inherits via dominance' warning, which we get thanks to
// the diamond inheritance of IPositionable through both IPane and
// PositionableBase. It's virtual inheritance with only
// PositionableBase providing the implementation, so it's OK.
#pragma warning( disable: 4250 )

#include "zugster/cinderpane/Pane.h"


/// @addtogroup Cinderpane
/// @{

namespace zugster {
	namespace cinderpane {

		class PaneBase
			: public IPane
            , public PositionableBase
		{
		  public:
			PaneBase() {}
			PaneBase(const Vec2f &position)
				: PositionableBase(position) {}
			PaneBase(const Vec2f &position, const Vec2f &size)
				: PositionableBase(position, size) {}
			PaneBase(const Vec2f &position, const Vec2f &size, bool visible)
				: PositionableBase(position, size, visible) {}

			virtual ~PaneBase() {}			
		};

	}
}

/// @}
