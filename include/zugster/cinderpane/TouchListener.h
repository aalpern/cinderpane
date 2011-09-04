#pragma once
#include "cinder/app/App.h"

/// @addtogroup Cinderpane
/// @{

namespace zugster {
	namespace cinderpane {

		struct ITouchListener
		{
			virtual bool onTouchesBegan( cinder::app::TouchEvent event ) { return false; }
			virtual bool onTouchesMoved( cinder::app::TouchEvent event ) { return false; }
			virtual bool onTouchesEnded( cinder::app::TouchEvent event ) { return false; }
		};
		typedef boost::shared_ptr<ITouchListener> TouchListenerRef;
	}
}

/// @}
