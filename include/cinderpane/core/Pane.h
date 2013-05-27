#pragma once
#include "cinder/gl/Texture.h"
#include "cinderpane/core/fwd.h"
#include "cinderpane/core/Positionable.h"

/// @addtogroup Core
/// @{

namespace cinderpane {
  namespace core {

    /// A pane is a high level component of the 2D interface; each
    /// pane is located at an absolute screen coordinate.  There
    /// are no restrictions on their position; they can overlap
    /// other panes. They are essentially the top-level containers
    /// for rendering and positioning items in your application's
    /// 2D interface.
    ///
    /// @see Interface

    struct IPane : public virtual IPositionable
    {
        virtual ~IPane() {}

        /// @name State and Rendering
        /// @{
        virtual void updatePane() = 0;

        virtual void drawPane() = 0;
        /// @}
    };

  }
}

/// @}
