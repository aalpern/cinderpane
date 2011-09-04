#pragma once
#include <vector>
#include "zugster/shared_ptr.h"
#include "cinder/Cinder.h"

/// @addtogroup Cinderpane
/// @{

namespace zugster {
	namespace cinderpane {
		
		using cinder::Vec2f;
		using cinder::Rectf;
		using cinder::Surface;

		struct IPane;
		typedef shared_ptr<IPane>    PaneRef;
		typedef std::vector<PaneRef> PaneVector;
	}
}

/// @}
