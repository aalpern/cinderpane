#pragma once
#include <string>
#include "cinderpane/gizmo/PartBase.h"

namespace cinderpane {
	namespace gizmo {

		class TextPart
			: public PartBase
		{
		  public:
			TextPart()
				: m_text("Cinder")
				, m_fontSize(12.0)
			{
			}

			TextPart(const std::string &text, double fontSize = 12.0, const std::string &fontName = "")
				: m_text(text)
				, m_fontSize(fontSize)
				, m_fontName(fontName)
			{
			}

			TextPart(const Vec2f &offset, const std::string &text, double fontSize = 12.0, const std::string &fontName = "")
				: PartBase(make_vec(0.0, 0.0), offset)
				, m_text(text)
				, m_fontSize(fontSize)
				, m_fontName(fontName)
			{
			}

			virtual ~TextPart()
			{
			}

			void setFontSize(double fontSize)
			{
				m_fontSize = fontSize;
			}

			void setText(const std::string text)
			{
				m_text = text;
			}

			virtual void renderGizmo(GizmoContext &context)
			{
				Vec2f position = context.Position + m_position;
				context.Cairo.save();

				context.Cairo.setSource(context.getTextColor(m_state));

				if ( m_fontSize > 0.0 )
					context.Cairo.setFontSize(m_fontSize);
				if ( m_fontName != "" )
					context.Cairo.setFontFace(cinder::cairo::FontFace(m_fontName));


				cinder::cairo::TextExtents extent = context.Cairo.textExtents(m_text);
				context.Cairo.moveTo(position.x - (extent.width() / 2),
									 position.y + ( extent.height() / 2));
				// position.y - (extent.height() / 2));
				context.Cairo.showText(m_text);

				context.Cairo.restore();
			}

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory)
			{
				PartBase::loadXML(element, factory);
				m_text = xmlGetChildValue<std::string>(element, "Text", "????");
				m_fontSize = xmlGetChildValue<double>(element, "FontSize", 12.0);
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlBeginPart(os, "Text");
				xmlWriteTag(os, "Text", m_text);
				xmlWriteTag(os, "FontSize", m_fontSize);
				xmlEndTag(os, "Text");
			}

		  protected:
			std::string m_text;
			std::string m_fontName;
			double m_fontSize;
		};
		typedef boost::shared_ptr<TextPart> TextPartRef;

	}
} // end namespace cinderpane::gizmo
