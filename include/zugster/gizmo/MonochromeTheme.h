#pragma once
#include "zugster/gizmo/Theme.h"

/// @addtogroup Gizmo
/// @{

namespace zugster {
	namespace gizmo {
		
		class MonochromeTheme : public ITheme
		{
		  public:
			MonochromeTheme(const ColorA &line, const ColorA &fill)
				: m_line(line), m_fill(fill) {}
			virtual ~MonochromeTheme() {}

			virtual ColorA getLineColor(IPart::DisplayState state, bool selected) const {
				return m_line; 
			}

			virtual ColorA getFillColor(IPart::DisplayState state, bool selected) const {
				return m_fill; 
			}

			virtual ColorA getTextColor(IPart::DisplayState state, bool selected) const {
				return m_line; 
			}

		  private:
			ColorA m_line;
			ColorA m_fill;
		};
	}
}

/// @}
