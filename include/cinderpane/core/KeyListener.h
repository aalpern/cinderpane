#pragma once
#include "cinder/app/App.h"
#include "cinderpane/shared_ptr.h"

/// @addtogroup Core
/// @{

namespace cinderpane {
	namespace core {

		struct IKeyListener
		{
			virtual bool onKeyDown( cinder::app::KeyEvent event ) { return false; }
			virtual bool onKeyUp( cinder::app::KeyEvent event ) { return false; }
		};
		typedef shared_ptr<IKeyListener> KeyListenerRef;
	}
}

/// @}
