#pragma once
#include "zugster/cinder/Shapes.h"
#include "zugster/gizmo/PartBase.h"

namespace zugster { 
	namespace gizmo {
    
		using namespace zugster::cinderext;

		//=============================================================================
		// FilledArcPart
		//=============================================================================
    
		// Just draws a single arc
		class FilledArcPart
			: public PartBase
		{
		  public:
			FilledArcPart()
				: PartBase(DEFAULT_RADIUS)
				, m_arcWidth(12.0)
				, m_beginDegrees(0.0)
				, m_endDegrees(360.0)
			{
				m_fill = true;
			}

			FilledArcPart(double outerRadius, 
						  double arcWidth,    
						  double beginDegrees = 0.0, 
						  double endDegrees = 360.0)
				: PartBase(outerRadius)
				, m_beginDegrees(beginDegrees)
				, m_endDegrees(endDegrees)
				, m_arcWidth(arcWidth)
			{
				m_fill = true;
				onRotationChanged(m_rotation, m_rotation);
			} 

			FilledArcPart(const Vec2f &position, 
						  double outerRadius, 
						  double arcWidth,    
						  double beginDegrees = 0.0, 
						  double endDegrees = 360.0)
				: PartBase(outerRadius, position)
				, m_beginDegrees(beginDegrees)
				, m_endDegrees(endDegrees)
				, m_arcWidth(arcWidth)
			{
				m_fill = true;
				onRotationChanged(m_rotation, m_rotation);
			} 
        
			virtual ~FilledArcPart()
			{
			}
        
			virtual void renderGizmo(GizmoContext &context)
			{
				double radius = m_size.x / 2;
				hollow_arc(context.Cairo, 
							context.Position + m_position, 
							radius - m_arcWidth, 
							radius,
							m_beginRadians, 
							m_endRadians);
				context.draw(m_outline, m_fill, m_state);
			}

			virtual void loadXML(const cinder::XmlTree &element, 
								 const PartFactoryRef &factory)
			{
				PartBase::loadXML(element, factory);
				m_beginDegrees = xmlGetChildValue<float>(element, "BeginDegrees", 0.0f);
				m_endDegrees   = xmlGetChildValue<float>(element, "EndDegrees", 360.0f);
				m_arcWidth     = xmlGetChildValue<float>(element, "ArcWidth", 1.0f);
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "FilledArc");
				xmlWriteTag(os, "BeginDegrees", m_beginDegrees);
				xmlWriteTag(os, "EndDegrees", m_endDegrees);
				xmlWriteTag(os, "ArcWidth", m_arcWidth);
				xmlEndTag(os, "FilledArc");
			}

        
		  protected:
			virtual void onRotationChanged(double rotation, double oldRotation)
			{
				m_beginRadians = ::cinder::toRadians( rotate_value(m_beginDegrees) );
				m_endRadians   = ::cinder::toRadians( rotate_value(m_endDegrees) );
			}
        
			double m_beginDegrees;
			double m_beginRadians;
			double m_endDegrees;
			double m_endRadians;
			double m_arcWidth;
		};
    
	}
} // end namespace zugster::gizmo
