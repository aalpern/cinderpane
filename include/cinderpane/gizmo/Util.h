#pragma once
#include <fstream>
#include "cinderpane/gizmo/API.h"
#include "cinder/Cinder.h"
#include "cinder/DataSource.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/cairo/Cairo.h"

namespace cinderpane {
	namespace gizmo {

		static const ColorA transparent(0.5, 0.5, 0.5, 0.0);

		cinder::cairo::SurfaceImage
		render_to_image(double width, double height, GizmoRef gizmo, bool center = true, bool alpha = true)
		{
			cinder::cairo::SurfaceImage surface((int32_t)width, (int32_t)height, alpha);
			cinder::cairo::Context      ctx(surface);
			GizmoContext                context(ctx, surface);

			ctx.setSource(transparent);
			ctx.fill();

			if ( center )
				gizmo->setPosition( make_vec( width / 2, height / 2 ) );
			gizmo->renderGizmo(context);

			return surface;
		}

		cinder::gl::Texture
		render_to_texture(double width, double height, GizmoRef gizmo, bool center = true, bool alpha = true)
		{
			cinder::cairo::SurfaceImage surface = render_to_image(width, height, gizmo, center, alpha);
			return cinder::gl::Texture(surface.getSurface());
		}

        cinder::gl::Texture
		render_to_texture(const Vec2f &size, GizmoRef gizmo, bool center = true, bool alpha = true)
		{
			return render_to_texture(size.x, size.y, gizmo, center, alpha);
		}

		void
		render_to_file(const std::string &filename, double width, double height, GizmoRef gizmo, bool center = true, bool alpha = true)
		{
			cinder::cairo::SurfaceImage surface = render_to_image(width, height, gizmo, center, alpha);
			cinder::writeImage( filename, surface.getSurface(), cinder::ImageTarget::Options(), "png");
		}

		void
		render_to_framebuffer(cinder::gl::Fbo &fbo, GizmoRef gizmo, bool center = true, bool alpha = true)
		{
			using namespace cinder;
			gl::Texture  texture = render_to_texture(fbo.getWidth(), fbo.getHeight(), gizmo, center, alpha);
            cinder::Area vp      = gl::getViewport();

			fbo.bindFramebuffer();
			gl::setViewport(fbo.getBounds());
			gl::clear(transparent);

			texture.bind();
			gl::drawSolidRect( fbo.getBounds() );

			fbo.unbindFramebuffer();
			texture.unbind();
			gl::setViewport(vp);
		}
	}
}
