#pragma once
#include <iostream>
#include "zugster/gizmo/CommandBase.h"
#include "zugster/gizmo/Context.h"

/// @addtogroup Gizmo
/// @{

namespace zugster { 
	namespace gizmo {

		using namespace zugster::cinderext;

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
			
			virtual void setDisplayState(IPart::DisplayState value)
			{
				m_state = value;
			}

			/// @name Rotation
			/// @{

			/** Rotate the part by an increment */
			virtual void rotate(double increment) 
			{
				double oldRotation = m_rotation;
				m_rotation = zugster::cinderext::rotate_degrees(m_rotation, increment);
				onRotationChanged(m_rotation, oldRotation);
			}

			/** Set the absolute rotation of the part */
			virtual void setRotation(double rotation)
			{
				if ( rotation == m_rotation )
					return;

				double oldRotation = m_rotation;
				m_rotation = rotation;
				onRotationChanged(m_rotation, oldRotation);
			}
	
			/** Get the current absolute rotation of the part */
			virtual double getRotation() const 
			{
				return m_rotation;
			}
	
			/** Set the auto-rotation speed in RPM */
			virtual void setRotationSpeed(double rpm)
			{
				m_rotationSpeedRPM = rpm;
				double rotationsPerSecond = rpm / 60.0;
				m_rotationIncrementPerFrame = ( rotationsPerSecond / cinder::app::getFrameRate() ) * 360.0;
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
			virtual void setOutline(bool value) 
			{
				m_outline = value;
			}
			
			/** Get the current outline render setting */
			virtual bool isOutlined() const
			{
				return m_outline;
			}
			
			/** Set whether the part should be filled when rendered */
			virtual void setFilled(bool value) 
			{
				m_fill = value;
			}
			
			/** Get the current fill render setting */
			virtual bool isFilled() const
			{
				return m_fill;
			}
			/// @}

			virtual void loadXML(const cinder::XmlTree &element, 
								 const PartFactoryRef &factory)
			{
				m_rotation = element.getAttributeValue("rotation", 0.0);
				m_rotationSpeedRPM = element.getAttributeValue("rotationSpeed", 0.0);
				m_fill     = element.getAttributeValue("fill", false);
				m_outline  = element.getAttributeValue("outline", true);
				m_visible  = element.getAttributeValue("visible", true);
				m_position 
                    = zugster::cinderext::parse_vec2<double>(
                        element.getAttributeValue<std::string>("position", "0,0"));
				m_size     
                    = zugster::cinderext::parse_vec2<double>(
                        element.getAttributeValue<std::string>("size", "0,0"));
			}

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
				return zugster::cinderext::rotate_degrees(value, m_rotation);
			}

			std::ostream& xmlBeginPart(std::ostream &os, const std::string &tagname) const
			{
				return os << "<" << tagname 
						  << " position=\"" << m_position.x << "," << m_position.y << "\""
						  << " size=\"" << m_size.x << "," << m_size.y << "\""
						  << " fill=\"" << m_fill << "\""
						  << " outline=\"" << m_outline << "\""
						  << " visible=\"" << m_visible << "\""
						  << " rotation=\"" << m_rotation << "\""
						  << " rotationSpeed=\"" << m_rotationSpeedRPM << "\""
						  << ">" << std::endl;
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
