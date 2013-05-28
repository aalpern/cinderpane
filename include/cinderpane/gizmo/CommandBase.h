#pragma once
#include <iostream>
#include "cinderpane/core/PositionableBase.h"
#include "cinderpane/gizmo/Part.h"

// Disable 'inherits via dominance' warning, which we get thanks to
// the diamond inheritance of IPositionable through both IPart and
// PositionableBase. It's virtual inheritance with only
// PositionableBase providing the implementation, so it's OK.
#pragma warning( disable: 4250 )

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    class CommandBase
        : public IPart
        , public cinderpane::core::PositionableBase
    {
      public:
        CommandBase() {}
        CommandBase(const Vec2f &position)
            : PositionableBase(position) {}
        CommandBase(const Vec2f &position, const Vec2f &size)
            : PositionableBase(position, size) {}
        CommandBase(const Vec2f &position, const Vec2f &size, bool visible)
            : PositionableBase(position, size, visible) {}

        virtual ~CommandBase () {}

        virtual void updateGizmo() {}

        /// @name IPart stub interface
        /// @{
        virtual IPart::DisplayState getDisplayState() const { return DisplayNormal; }
        virtual IPart* setDisplayState(IPart::DisplayState value) { return this; }
        virtual IPart* rotate(double increment) { return this; }
        virtual IPart* setRotation(double rotation) { return this; }
        virtual double getRotation() const { return 0.0; }
        virtual IPart* setRotationSpeed(double rpm) { return this; }
        virtual double getRotationSpeed() const { return 0.0; }
        virtual IPart* setOutline(bool value) { return this; }
        virtual bool   isOutlined() const { return false; }
        virtual IPart* setFilled(bool value) { return this; }
        virtual bool   isFilled() const { return false; }
        /// @}
    };
  }
}

/// @}
