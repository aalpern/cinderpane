#pragma once
#include "cinder/Text.h"
#include "zugster/cinderpane/PaneBase.h"

/// @addtogroup Cinderpane
/// @{

namespace zugster {
	namespace cinderpane {

		class TextLayoutPane
			: public TexturePane
		{
		  public:
			TextLayoutPane() {}
			TextLayoutPane(const Vec2f &position)
				: TexturePane(position) {}
			TextLayoutPane(const cinder::TextLayout &text)
				: m_text(text) {}
			TextLayoutPane(const Vec2f &position, const cinder::TextLayout &text)
				: TexturePane(position), m_text(text) {}

			virtual ~TextLayoutPane() {}

            cinder::TextLayout& text()
			{
				return m_text;
			}

			const cinder::TextLayout& text() const
			{
				return m_text;
			}

			virtual void updatePane()
			{
				if ( !m_texture )
					m_texture = m_text.render(true);
			}

		  private:
            cinder::TextLayout m_text;
		};

	}
}

/// @}
