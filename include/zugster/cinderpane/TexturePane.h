#pragma once
#include "cinder/gl/Texture.h"
#include "zugster/cinderpane/PaneBase.h"

/// @addtogroup Cinderpane
/// @{

namespace zugster {
	namespace cinderpane {
		
		/// TexturePane is the root class for interface elements which
		/// render to a GL texture. Subclasses should implement the
		/// renderPane() interface to return a GL texture.

		class TexturePane
			: public PaneBase
		{
		  public:
			TexturePane() {}
			TexturePane(const Vec2f &position)
				: PaneBase(position) {}
			TexturePane(const Vec2f &position, const Vec2f &size)
				: PaneBase(position, size) {}
			TexturePane(const Vec2f &position, const Vec2f &size, bool visible)
				: PaneBase(position, size, visible) {}

			virtual ~TexturePane() {}

			void setTexture(const cinder::gl::Texture &texture)
			{
				m_texture = texture;
			}

			virtual void drawPane()
			{
				if ( !m_texture )
					m_texture = renderPane();

				// double check in case renderPane didn't do anything
				if ( m_texture ) 
					cinder::gl::draw(m_texture, getPosition());
			}

			virtual cinder::gl::Texture renderPane()
			{
				return m_texture;
			}

		  protected:
			cinder::gl::Texture m_texture;
		};

	}
}

/// @}

