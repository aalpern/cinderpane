#pragma once
#include "zugster/cinder/Solarized.h"
#include "zugster/gizmo/Theme.h"

/// @addtogroup Gizmo
/// @{

namespace zugster {
	namespace gizmo {
		
		class SolarizedTheme : public ITheme
		{
		  public:
			SolarizedTheme() {}
			virtual ~SolarizedTheme() {}

			virtual ColorA getLineColor(IPart::DisplayState state, bool selected) const
			{
				switch ( state ) 
				{
				  case IPart::DisplayDisabled:
					  return m_OutlineDisabled * m_lineAdjust;
				  case IPart::DisplayNormal:
					  return m_Outline * m_lineAdjust;
				  case IPart::DisplaySecondary:
					  return m_OutlineSecondary * m_lineAdjust;
				  case IPart::DisplayHighlight:
					  return m_OutlineHighlight * m_lineAdjust;
				  case IPart::DisplayOK:
					  return m_StatusOK * m_lineAdjust;
				  case IPart::DisplayWarning:
					  return m_StatusWarning * m_lineAdjust;
				  case IPart::DisplaySubCritical:
					  return m_StatusSubCritical * m_lineAdjust;
				  case IPart::DisplayCritical:
					  return m_StatusCritical * m_lineAdjust;
				  default:
					  return m_Outline * m_lineAdjust;
				}
			}

			virtual ColorA getFillColor(IPart::DisplayState state, bool selected) const
			{
				switch ( state ) 
				{
				  case IPart::DisplayDisabled:
					  return m_Background * m_fillAdjust;
				  case IPart::DisplayNormal:
					  return m_Background * m_fillAdjust;
				  case IPart::DisplaySecondary:
					  return m_Background * m_fillAdjust;
				  case IPart::DisplayHighlight:
					  return m_BackgroundHighlight * m_fillAdjust;
				  case IPart::DisplayOK:
					  return m_StatusOK * m_fillAdjust;
				  case IPart::DisplayWarning:
					  return m_StatusWarning * m_fillAdjust;
				  case IPart::DisplaySubCritical:
					  return m_StatusSubCritical * m_fillAdjust;
				  case IPart::DisplayCritical:
					  return m_StatusCritical * m_fillAdjust;
				  default:
					  return m_Background * m_fillAdjust;
				}
			}

			virtual ColorA getTextColor(IPart::DisplayState state, bool selected) const
			{
				switch ( state )
				{
				  case IPart::DisplayDisabled:
					  return m_OutlineDisabled * m_textAdjust;
				  case IPart::DisplayNormal:
					  return m_Outline * m_textAdjust;
				  case IPart::DisplaySecondary:
					  return m_OutlineSecondary * m_textAdjust;
				  case IPart::DisplayHighlight:
					  return m_OutlineHighlight * m_textAdjust;
				  case IPart::DisplayOK:
					  return m_StatusOK * m_textAdjust;
				  case IPart::DisplayWarning:
					  return m_StatusWarning * m_textAdjust;
				  case IPart::DisplaySubCritical:
					  return m_StatusSubCritical * m_textAdjust;
				  case IPart::DisplayCritical:
					  return m_StatusCritical * m_textAdjust;
				  default:
					  return m_Outline * m_textAdjust;
				}
			}

		  protected:
			ColorA m_lineAdjust;
			ColorA m_fillAdjust;
			ColorA m_textAdjust;

			ColorA m_Background;
			ColorA m_BackgroundHighlight;
			ColorA m_Text;
			ColorA m_TextSecondary;
			ColorA m_TextHighlight;
			ColorA m_Outline;
			ColorA m_OutlineSecondary;
			ColorA m_OutlineHighlight;
			ColorA m_OutlineDisabled;
			ColorA m_StatusOK;
			ColorA m_StatusWarning;
			ColorA m_StatusSubCritical;
			ColorA m_StatusCritical;
		};
		
		class SolarizedThemeDark : public SolarizedTheme
		{
		  public:
			SolarizedThemeDark()
			{
				m_lineAdjust = ColorA(1.0, 1.0, 1.0, 0.9);
				m_fillAdjust = ColorA(1.0, 1.0, 1.0, 0.75);
				m_textAdjust = ColorA(1.0, 1.0, 1.0, 0.9);

				m_Background          = solarized::base03;
				m_BackgroundHighlight = solarized::blue - 0.1;
				m_Text                = solarized::base0;
				m_TextSecondary       = solarized::base01;
				m_TextHighlight       = solarized::base1;
				m_Outline             = solarized::blue;
				m_OutlineSecondary    = solarized::base01;
				m_OutlineHighlight    = solarized::blue;
				m_OutlineDisabled     = solarized::base01;
				m_StatusOK            = solarized::green;
				m_StatusWarning       = solarized::yellow;
				m_StatusSubCritical   = solarized::orange;
				m_StatusCritical      = solarized::red;
			}
		};
		
		class SolarizedThemeLight : public SolarizedThemeDark
		{
		  public:
			SolarizedThemeLight()
				: SolarizedThemeDark()
			{
				m_Background          = solarized::base3;
				m_BackgroundHighlight = solarized::base2;
				m_Text                = solarized::base00;
				m_TextSecondary       = solarized::base1;
				m_TextHighlight       = solarized::base01;
			}
		};
	}
}

/// @}
