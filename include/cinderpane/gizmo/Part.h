#pragma once
#include <list>
#include "cinder/app/App.h"
#include "cinderpane/ext/Utils.h"
#include "cinderpane/core/Positionable.h"
#include "cinderpane/gizmo/fwd.h"
#include "cinderpane/gizmo/Renderable.h"
#include "cinderpane/gizmo/Persistable.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    /// A part is a piece of an interface gizmo (aka widget, frob,
    /// dingus, thingy, framistat, etc...). Parts can be
    /// composited into collections (See \c CompositePart and \c
    /// Gizmo), and have a number of attributes relating to their
    /// display.
    ///
    /// \b Size & Position
    ///
    /// Each part has size and position values (both stored as \c
    /// Vec2f instance). The position of a part is an offset
    /// relative to the absolute position of the \c Gizmo it's
    /// contained in. When rendering, the absolute position is
    /// supplied by the \c GizmoContext, and each rendering method
    /// applies the part's position to that to obtain the absolute
    /// position it should be drawn in. This makes individual
    /// parts location-independent.
    ///
    /// @see CompositePart
    /// @see Gizmo
    struct IPart
        : public virtual IRenderable
        , public virtual IPersistable
        , public virtual cinderpane::core::IPositionable
    {
        virtual ~IPart() {}

        enum DisplayState
        {
            DisplayDisabled,
            DisplayNormal,
            DisplaySecondary,
            DisplayHighlight,
            DisplayOK,
            DisplayWarning,
            DisplaySubCritical,
            DisplayCritical
        };

        virtual DisplayState getDisplayState() const = 0;

        virtual IPart* setDisplayState(DisplayState value) = 0;

        /// @name Rotation
        /// @{

        /** Rotate the part by an increment */
        virtual IPart* rotate(double increment) = 0;

        /** Set the absolute rotation of the part */
        virtual IPart* setRotation(double rotation) = 0;

        /** Get the current absolute rotation of the part */
        virtual double getRotation() const = 0;

        /** Set the auto-rotation speed in RPM */
        virtual IPart* setRotationSpeed(double rpm) = 0;

        /** Get the current auto-rotation speed */
        virtual double getRotationSpeed() const = 0;

        /// @}

        /// @name Rendering Options
        /// @{

        /** Set whether the part should be outlined when rendered */
        virtual IPart* setOutline(bool value) = 0;

        /** Get the current outline render setting */
        virtual bool   isOutlined() const = 0;

        /** Set whether the part should be filled when rendered */
        virtual IPart* setFilled(bool value) = 0;

        /** Get the current fill render setting */
        virtual bool   isFilled() const = 0;
        /// @}

    };
  }
}

/// @}
