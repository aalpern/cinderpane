#pragma once
#include "cinderpane/core/Positionable.h"

/// @addtogroup Core
/// @{

namespace cinderpane {
  namespace core {

    /// An interface for interfaces items which have attributes of
    /// position, size, and visibility. This provides an interface
    /// for future layout managers to work with both Gizmos and
    /// Panes.
    class PositionableBase : public virtual IPositionable
    {
      public:
        PositionableBase()
            : m_visible(true) {}
        PositionableBase(const Vec2f &position)
            : m_position(position), m_visible(true) {}
        PositionableBase(const Vec2f &position, const Vec2f &size)
            : m_position(position), m_size(size), m_visible(true) {}
        PositionableBase(const Vec2f &position, const Vec2f &size, bool visible)
            : m_position(position), m_size(size), m_visible(visible) {}

        virtual ~PositionableBase() {}

        /// @name Position
        /// @{
        virtual IPositionable* move(const Vec2f &offset)
        {
            Vec2f old = m_position;
            m_position += offset;
            onPositionChanged(m_position, old);
            return this;
        }

        virtual Vec2f getPosition() const
        {
            return m_position;
        }

        virtual IPositionable* setPosition(const Vec2f &position)
        {
            if ( m_position == position )
                return this;

            Vec2f old = m_position;
            m_position = position;
            onPositionChanged(position, old);
            return this;
        }
        /// @}

        /// @name Size
        /// @{
        virtual IPositionable* morph(const Vec2f &offset)
        {
            Vec2f old = m_size;
            m_size += offset;
            onSizeChanged(m_size, old);
            return this;
        }

        virtual Vec2f getSize() const
        {
            return m_size;
        }

        virtual IPositionable* setSize(const Vec2f &size)
        {
            if ( m_size == size )
                return this;
            Vec2f old = m_size;
            m_size = size;
            onSizeChanged(size, old);
            return this;
        }

        virtual Rectf getBounds() const
        {
            return Rectf(m_position, m_position + m_size);
        }
        /// @}

        /// @name Visibility
        /// @{
        virtual bool isVisible() const
        {
            return m_visible;
        }

        virtual IPositionable* setVisible(bool visible)
        {
            if ( m_visible == visible )
                return this;
            m_visible = visible;
            onVisibilityChanged();
            return this;
        }
        /// @}

      protected:
        /** Handler function for updating after a move */
        virtual void onPositionChanged(const Vec2f &position, const Vec2f &oldPosition) {}

        /** Handler function for updating after a size change */
        virtual void onSizeChanged(const Vec2f &size, const Vec2f &oldSize) {}

        /** Handler function for updateing after a visiblity change */
        virtual void onVisibilityChanged() {}

        bool  m_visible;
        Vec2f m_position;
        Vec2f m_size;
    };
    typedef shared_ptr<IPositionable> PositionableRef;
  }
}

/// @
