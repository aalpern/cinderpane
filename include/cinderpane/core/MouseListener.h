#pragma once
#include "cinder/app/App.h"

/// @addtogroup Core
/// @{

namespace cinderpane {
	namespace core {

		struct IMouseListener
		{
			virtual ~IMouseListener() {}

			virtual bool onMouseDown( cinder::app::MouseEvent event ) { return false; }
			virtual bool onMouseUp( cinder::app::MouseEvent event ) { return false; }
			virtual bool onMouseWheel( cinder::app::MouseEvent event ) { return false; }
			virtual bool onMouseMove( cinder::app::MouseEvent event ) { return false; }
			virtual bool onMouseDrag( cinder::app::MouseEvent event ) { return false; }
			virtual bool onMouseEnter( cinder::app::MouseEvent event ) { return false; }
			virtual bool onMouseExit( cinder::app::MouseEvent event ) { return false; }
		};
		typedef boost::shared_ptr<IMouseListener> MouseListenerRef;
	}
}

/// @}
