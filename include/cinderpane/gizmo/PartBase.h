#pragma once
#include <iostream>
#include "cinderpane/gizmo/CommandBase.h"
#include "cinderpane/gizmo/Context.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    using namespace cinderpane::ext;

    class PartBase
        : public CommandBase
    {
      public:
        PartBase()
            : m_outline(true)
            , m_fill(false)
            , m_state(IPart::DisplayNormal)
        {
            zero_rotation();
        }

        PartBase(const Vec2f &size)
            : CommandBase(make_vec(0.0, 0.0), size)
            , m_outline(true)
            , m_fill(false)
            , m_state(IPart::DisplayNormal)
        {
            zero_rotation();
        }

        PartBase(double radius)
            : CommandBase(make_vec(0.0, 0.0), make_vec(radius * 2, radius * 2))
            , m_outline(true)
            , m_fill(false)
            , m_state(IPart::DisplayNormal)
        {
            zero_rotation();
        }

        PartBase(const Vec2f &size, const Vec2f &position)
            : CommandBase(position, size)
            , m_outline(true)
            , m_fill(false)
            , m_state(IPart::DisplayNormal)
        {
            zero_rotation();
        }

        PartBase(double radius, const Vec2f &position)
            : CommandBase(position, make_vec(radius * 2, radius * 2))
            , m_outline(true)
            , m_fill(false)
            , m_state(IPart::DisplayNormal)
        {
            zero_rotation();
        }

        virtual ~PartBase () {}

        /// @name IRenderable
        /// @{
        virtual void updateGizmo()
        {
            if ( m_rotationIncrementPerFrame > 0.0 || m_rotationIncrementPerFrame < 0.0 )
            {
                rotate(m_rotationIncrementPerFrame);
            }
        }

        virtual void renderGizmo(GizmoContext &context) {}
        /// @}

        virtual IPart::DisplayState getDisplayState() const
        {
            return m_state;
        }

        virtual IPart* setDisplayState(IPart::DisplayState value)
        {
            m_state = value;
            return this;
        }

        /// @name Rotation
        /// @{

        /** Rotate the part by an increment */
        virtual IPart* rotate(double increment)
        {
            double oldRotation = m_rotation;
            m_rotation = cinderpane::ext::rotate_degrees(m_rotation, increment);
            onRotationChanged(m_rotation, oldRotation);
            return this;
        }

        /** Set the absolute rotation of the part */
        virtual IPart* setRotation(double rotation)
        {
            if ( rotation == m_rotation )
                return this;

            double oldRotation = m_rotation;
            m_rotation = rotation;
            onRotationChanged(m_rotation, oldRotation);
            return this;
        }

        /** Get the current absolute rotation of the part */
        virtual double getRotation() const
        {
            return m_rotation;
        }

        /** Set the auto-rotation speed in RPM */
        virtual IPart* setRotationSpeed(double rpm)
        {
            m_rotationSpeedRPM = rpm;
            double rotationsPerSecond = rpm / 60.0;
            m_rotationIncrementPerFrame = ( rotationsPerSecond / cinder::app::getFrameRate() ) * 360.0;
            return this;
        }

        /** Get the current auto-rotation speed */
        virtual double getRotationSpeed() const
        {
            return m_rotationSpeedRPM;
        }

        /// @}

        /// @name Rendering Options
        /// @{

        /** Set whether the part should be outlined when rendered */
        virtual IPart* setOutline(bool value)
        {
            m_outline = value;
            return this;
        }

        /** Get the current outline render setting */
        virtual bool isOutlined() const
        {
            return m_outline;
        }

        /** Set whether the part should be filled when rendered */
        virtual IPart* setFilled(bool value)
        {
            m_fill = value;
            return this;
        }

        /** Get the current fill render setting */
        virtual bool isFilled() const
        {
            return m_fill;
        }
        /// @}

      protected:
        /** Handler function for updating after a rotation */
        virtual void onRotationChanged(double rotation, double oldRotation) {}

        /** Handler function for updating after a move */
        virtual void onPositionChanged(const Vec2f &position, const Vec2f &oldPosition) {}

        /** Handler function for updating after a size change */
        virtual void onSizeChanged(const Vec2f &size, const Vec2f &oldSize) {}

        /** Handler function for updateing after a visiblity change */
        virtual void onVisibilityChanged() {}

        /** Utility function for derived parts to adjust a radial
         * value according to the current rotation setting */
        double rotate_value(double value)
        {
            return cinderpane::ext::rotate_degrees(value, m_rotation);
        }

        //=============================================================================
        // Data
        //=============================================================================

        void zero_rotation()
        {
            m_rotation = m_rotationSpeedRPM = m_rotationIncrementPerFrame = 0.0;
        }

        DisplayState m_state;
        double m_rotation;
        double m_rotationSpeedRPM;
        double m_rotationIncrementPerFrame;
        bool   m_outline;
        bool   m_fill;
    };
  }
}

/// @}
