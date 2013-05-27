#pragma once
#include <list>
#include <iostream>
#include "cinderpane/core/Positionable.h"
#include "cinderpane/core/MouseListener.h"
#include "cinderpane/core/KeyListener.h"
#include "cinderpane/core/TouchListener.h"

/// @addtogroup Core
/// @{

#define CINDERPANE_DISPATCH_POSITIONAL_EVENT(type, container, function)	\
	for ( type::iterator i = container.begin(); i != container.end();	\
		  i++ )															\
	{																	\
		PositionableRef pos = dynamic_pointer_cast<IPositionable>(*i);	\
		if ( pos && !pos->getBounds().contains(event.getPos()) )		\
			continue;													\
		if ( (*i)->function(event) )									\
			return true;												\
	}																	\
	return false

#define CINDERPANE_DISPATCH_EVENT(type, container, function)			\
	for ( type::iterator i = container.begin(); i != container.end();	\
		  i++ )															\
	{																	\
		if ( (*i)->function(event) )									\
			return true;												\
	}																	\
	return false

namespace cinderpane {
  namespace core {

    using std::cout;
    using std::endl;

    class EventDispatcher
        : public IMouseListener
        , public IKeyListener
    {
        typedef std::list<MouseListenerRef> mouse_container;
        typedef std::list<KeyListenerRef>   key_container;
        typedef std::list<TouchListenerRef> touch_container;

      public:
        EventDispatcher() {}
        ~EventDispatcher() {}

        void add(KeyListenerRef k)
        {
            m_keys.push_back(k);
        }

        void add(MouseListenerRef m)
        {
            m_mice.push_back(m);
        }

        void add(TouchListenerRef t)
        {
            m_touches.push_back(t);
        }

        void remove(KeyListenerRef k)
        {
            m_keys.remove(k);
        }

        void remove(MouseListenerRef m)
        {
            m_mice.remove(m);
        }

        void remove(TouchListenerRef t)
        {
            m_touches.remove(t);
        }

        virtual bool onMouseDown( cinder::app::MouseEvent event )
        {
            CINDERPANE_DISPATCH_POSITIONAL_EVENT(mouse_container, m_mice, onMouseDown);
        }

        virtual bool onMouseUp( cinder::app::MouseEvent event )
        {
            CINDERPANE_DISPATCH_POSITIONAL_EVENT(mouse_container, m_mice, onMouseUp);
        }

        virtual bool onMouseWheel( cinder::app::MouseEvent event )
        {
            CINDERPANE_DISPATCH_POSITIONAL_EVENT(mouse_container, m_mice, onMouseWheel);
        }

        virtual bool onMouseMove( cinder::app::MouseEvent event )
        {
            CINDERPANE_DISPATCH_POSITIONAL_EVENT(mouse_container, m_mice, onMouseMove);
        }

        virtual bool onMouseDrag( cinder::app::MouseEvent event )
        {
            CINDERPANE_DISPATCH_POSITIONAL_EVENT(mouse_container, m_mice, onMouseDrag);
        }

        virtual bool onKeyDown( cinder::app::KeyEvent event )
        {
            CINDERPANE_DISPATCH_EVENT(key_container, m_keys, onKeyDown);
        }

        virtual bool onKeyUp( cinder::app::KeyEvent event )
        {
            CINDERPANE_DISPATCH_EVENT(key_container, m_keys, onKeyUp);
        }

        virtual bool onTouchesBegan( cinder::app::TouchEvent event )
        {
            return false;
        }

        virtual bool onTouchesMoved( cinder::app::TouchEvent event )
        {
            return false;
        }

        virtual bool onTouchesEnded( cinder::app::TouchEvent event )
        {
            return false;
        }

      private:
        mouse_container m_mice;
        key_container   m_keys;
        touch_container m_touches;
    };
  }
}

/// @}
