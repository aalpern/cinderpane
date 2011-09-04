#pragma once
#include "cinder/app/App.h"

/// @addtogroup Cinderpane
/// @{

namespace zugster {
	namespace cinderpane {

		struct IKeyListener
		{
			virtual bool onKeyDown( cinder::app::KeyEvent event ) { return false; }
			virtual bool onKeyUp( cinder::app::KeyEvent event ) { return false; }
		};
		typedef boost::shared_ptr<IKeyListener> KeyListenerRef;
	}
}

/// @}
