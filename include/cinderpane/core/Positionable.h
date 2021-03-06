#pragma once
#include "cinder/gl/Texture.h"
#include "cinderpane/core/fwd.h"

/// @addtogroup Core
/// @{

namespace cinderpane {
  namespace core {

    /// An interface for interfaces items which have attributes of
    /// position, size, and visibility. This provides an interface
    /// for future layout managers to work with both Gizmos and
    /// Panes.
    struct IPositionable
    {
        virtual ~IPositionable() {}

        /// @name Position
        /// @{
        /** Change the position of the part by a vector offset */
        virtual IPositionable* move(const Vec2f &offset) = 0;

        /** Get the current position of the part */
        virtual Vec2f getPosition() const = 0;

        /** Set the position of the part */
        virtual IPositionable* setPosition(const Vec2f &position) = 0;
        /// @}

        /// @name Size
        /// @{
        /** Change the size of the part by an offset */
        virtual IPositionable* morph(const Vec2f &offset) = 0;

        /** Set the absolute size of the part */
        virtual Vec2f getSize() const = 0;

        /** Get the current absolute size of the part */
        virtual IPositionable* setSize(const Vec2f &size) = 0;

        /** Get the current absolute bounds of the part */
        virtual Rectf getBounds() const = 0;
        /// @}

        /// @name Visibility
        /// @{
        /** Get the visibility of the part */
        virtual bool isVisible() const = 0;

        /** Set the visibility of the part */
        virtual IPositionable* setVisible(bool visible) = 0;
        /// @}
    };
    typedef shared_ptr<IPositionable> PositionableRef;
  }
}

/// @}
