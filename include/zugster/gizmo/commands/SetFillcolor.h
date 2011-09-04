#pragma once
#include "zugster/gizmo/CommandBase.h"

/// @addtogroup Gizmo
/// @{

namespace zugster { 
	namespace gizmo {

		class SetFillcolor : public CommandBase
		{
		  public:
			SetFillcolor()
				: m_fillcolor(0.5, 0.5, 0.5, 1.0) {}
			SetFillcolor(const ColorA &fillcolor)
				: m_fillcolor(fillcolor) {}

			virtual ~SetFillcolor() {}

			void setFillColor(const ColorA &color)
			{
				m_fillcolor = color;
			}

			virtual void renderGizmo(GizmoContext &ctx)
			{
				// if ( !ctx.UseThemes )
					ctx.FillColor = m_fillcolor;				
			}

			virtual void loadXML(const cinder::XmlTree &element, 
								 const PartFactoryRef &factory)
			{
				m_fillcolor = zugster::cinderext::parse_colorA(element.getValue(), m_fillcolor);
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlWriteTag(os, "SetFillColor", m_fillcolor);
			}

		  protected:
			ColorA m_fillcolor;
		};

	}
}

/// @}
