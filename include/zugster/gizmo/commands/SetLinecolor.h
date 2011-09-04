#pragma once
#include "zugster/gizmo/CommandBase.h"

/// @addtogroup Gizmo
/// @{

namespace zugster { 
	namespace gizmo {

		class SetLinecolor : public CommandBase
		{
		  public:
			SetLinecolor()
				: m_lineColor(ColorA(1.0, 1.0, 1.0, 1.0)) {}

			SetLinecolor(const ColorA &lineColor)
				: m_lineColor(lineColor) {}

			virtual ~SetLinecolor() {}

			void setLineColor(const ColorA &color)
			{
				m_lineColor = color;
			}

			virtual void renderGizmo(GizmoContext &ctx)
			{
				// if ( !ctx.UseThemes )
				// {
					ctx.LineColor = m_lineColor;				
					ctx.Cairo.setSource(m_lineColor);
					// }
			}

			virtual void loadXML(const cinder::XmlTree &element, 
								 const PartFactoryRef &factory)
			{
				m_lineColor = zugster::cinderext::parse_colorA(element.getValue(), m_lineColor);
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlWriteTag(os, "SetLineColor", m_lineColor);
			}

		  protected:
			ColorA m_lineColor;
		};

	}
}

/// @}
