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
			virtual void   setDisplayState(IPart::DisplayState value) {}
			virtual void   rotate(double increment) { }
			virtual void   setRotation(double rotation) { }
			virtual double getRotation() const { return 0.0; }
			virtual void   setRotationSpeed(double rpm) { }
			virtual double getRotationSpeed() const { return 0.0; }
			virtual void   setOutline(bool value) { }
			virtual bool   isOutlined() const { return false; }
			virtual void   setFilled(bool value) { }
			virtual bool   isFilled() const { return false; }
			/// @}

		  protected:
			//=============================================================================
			/// @name XML Utilities for implementing IPersistable
			/// @{
			//=============================================================================

			std::ostream& xmlBeginTag(std::ostream &os, const std::string &tagname) const
			{
				return os << "<" << tagname << ">";
			}

			std::ostream& xmlEndTag(std::ostream &os, const std::string &tagname) const
			{
				return os << "</" << tagname << ">" << std::endl;
			}

			template<typename T>
			std::ostream& xmlWriteTag(std::ostream &os, const std::string &tagname, const T &value) const
			{
				return os << "<" << tagname << ">"
						  << value
						  << "</" << tagname << ">" << std::endl;
			}

			std::ostream& xmlWriteTag(std::ostream &os, const std::string &tagname, const ColorA &value) const
			{
				os << "<" << tagname << ">";
				return cinderpane::ext::write_color(os, value)
					<< "</" << tagname << ">" << std::endl;
			}

			std::ostream& xmlWriteTag(std::ostream &os, const std::string &tagname) const
			{
				return os << "<" << tagname << "/>" << std::endl;
			}

			template<typename T>
			T xmlGetChildValue(const cinder::XmlTree &element, const std::string &childName, const T &defaultValue)
			{
				T value = defaultValue;
				if ( element.hasChild(childName) )
					value = element.getChild(childName).getValue<T>(defaultValue);
				return value;
			}

			/// @}
		};
	}
}

/// @}
