#pragma once
#include <stack>
#include <boost/shared_ptr.hpp>
// #include "cairo.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"
#include "cinder/cairo/Cairo.h"
#include "zugster/gizmo/Part.h"
#include "zugster/Gizmo/SolarizedTheme.h"
#include "zugster/Gizmo/MonochromeTheme.h"

/// @addtogroup Gizmo
/// @{

namespace zugster { 
	namespace gizmo {

		/// Provides a context for rendering a gizmo or gizmo part, including the 
		/// Cairo context, the Surface (which may be useful if the dimensions of it 
		/// are needed), and the position of the parent gizmo.  
		class GizmoContext
		{
		  public:
			GizmoContext(cinder::cairo::Context &c, cinder::cairo::SurfaceBase &s)
				: Cairo(c)
				, Surface(s)
				, FillColor(0.0, 0.0, 0.0, 1.0)
				, LineColor(1.0, 1.0, 1.0, 1.0) 
			{
				// Theme = make_shared<SolarizedThemeLight>();
				Theme = make_shared<SolarizedThemeDark>();
				// Theme = make_shared<MonochromeTheme>(ColorA(0.0, 0.85, 0.9, 0.9), ColorA(0.0, 0.3, 0.3, 0.5));
			}

			cinder::cairo::Context &       Cairo;
			cinder::cairo::SurfaceBase &   Surface;
			cinder::Vec2f                  Position;
			cinder::ColorA                 LineColor;
			cinder::ColorA                 FillColor;
			ThemeRef                       Theme;
			
			void save()
			{
				Cairo.save();
				m_state.push( std::pair<cinder::ColorA, cinder::ColorA>(LineColor, FillColor) );
				m_position_state.push( Position );
			}

			void restore()
			{
				Cairo.restore();
				if ( m_state.size() > 0 ) 
				{
					LineColor = m_state.top().first;
					FillColor = m_state.top().second;
					m_state.pop();
				}
				if ( m_position_state.size() > 0 )
				{
					Position = m_position_state.top();
					m_position_state.pop();
				}
			}
			
			const cinder::ColorA getFillColor(IPart::DisplayState state, bool selected = false) const
			{
				return Theme ? Theme->getFillColor(state, selected) : FillColor;
			}

			const cinder::ColorA getLineColor(IPart::DisplayState state, bool selected = false) const
			{
				return Theme ? Theme->getLineColor(state, selected) : LineColor;
			}

			const cinder::ColorA getTextColor(IPart::DisplayState state, bool selected = false) const
			{
				return Theme ? Theme->getTextColor(state, selected) : LineColor;
			}

			void draw(bool outline, bool fill, IPart::DisplayState state, bool selected = false)
			{
				Cairo.save();
				if ( fill )
				{
					Cairo.setSource(getFillColor(state, selected));
					// Cairo.setOperator(CAIRO_OPERATOR_ADD);
					Cairo.fillPreserve();
				} 
				if ( outline )
				{
					Cairo.setSource(getLineColor(state, selected));
					// Cairo.setOperator(CAIRO_OPERATOR_SOURCE);
					Cairo.stroke();
				}
				Cairo.newPath();

				Cairo.restore();
			}

			void fill()
			{
				Cairo.save();				
				Cairo.setSource(FillColor);
				Cairo.fill();
				Cairo.restore();
			}

			void fillPreserve()
			{
				Cairo.save();
				Cairo.setSource(FillColor);
				Cairo.fillPreserve();
				Cairo.restore();
			}

		  private:
			std::stack< std::pair<cinder::ColorA, cinder::ColorA> > m_state;
			std::stack< Vec2f > m_position_state;
		};

		class GizmoSavePosition
		{
		  public:
			GizmoSavePosition(GizmoContext &ctx)
				: m_ctx(ctx), m_position(ctx.Position) {}
			~GizmoSavePosition() {
				m_ctx.Position = m_position;
			}
		  private:
			GizmoContext &m_ctx;
			cinder::Vec2f m_position;
		};		
	} 
} // end namespace zugster::gizmo

/// @}
