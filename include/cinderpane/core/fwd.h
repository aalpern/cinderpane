#pragma once
#include <vector>
#include "cinderpane/shared_ptr.h"
#include "cinder/Cinder.h"

/// @addtogroup Core
/// @{

namespace cinderpane {
	namespace core {

		using cinder::Vec2f;
		using cinder::Rectf;
		using cinder::Surface;

		struct IPane;
		typedef shared_ptr<IPane>    PaneRef;
		typedef std::vector<PaneRef> PaneVector;
	}
}

/// @}
