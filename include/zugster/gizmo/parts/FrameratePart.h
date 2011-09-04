#pragma once
#include <sstream>
#include "cinder/app/App.h"
#include "zugster/gizmo/parts/TextPart.h"

namespace zugster {
	namespace gizmo {
		
		class FrameratePart
			: public TextPart
		{
		  public:
			FrameratePart()
				: TextPart("") {}
			FrameratePart(double fontSize)
				: TextPart("", fontSize) {}
			FrameratePart(const Vec2f &offset, double fontSize)
				: TextPart(offset, "", fontSize) {}

			virtual void updateGizmo()
			{
				std::ostringstream s;
				s << std::setprecision(2) << cinder::app::App::get()->getAverageFps();
				setText(s.str());
			}

			virtual void loadXML(const cinder::XmlTree &element, 
								 const PartFactoryRef &factory)
			{
				TextPart::loadXML(element, factory);
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "Framerate");
				xmlWriteTag(os, "Text", m_text);
				xmlWriteTag(os, "FontSize", m_fontSize);
				xmlEndTag(os, "Framerate");
			}
	
		};

	}
}
