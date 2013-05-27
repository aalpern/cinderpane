#pragma once
#include "cinderpane/gizmo/Gizmo.h"
#include "cinderpane/gizmo/Util.h"
#include "cinderpane/core/TexturePane.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    /// A class for presenting a rendered Gizmo as part of a
    /// Pane-based interface. Because a Gizmo can contain other
    /// Gizmos, this can be a single point for containing several
    /// different gizmos which are logically grouped together and
    /// rendered together on-screen.
    ///
    /// A GizmoPane can be optionally 'frozen', which will cause
    /// its rendered GL texture to be cached and redrawn without
    /// calling updateGizmo() or renderPane(). Because drawing
    /// Cairo-based widgets is done on the CPU and is relatively
    /// expensive, any static elements do not need to be drawn
    /// repeatedly. They can be rendered to a texture once, and
    /// then just blitted to the screen as necessary.

    class GizmoPane
        : public cinderpane::core::TexturePane
    {
      public:
        GizmoPane(double x, double y, double h, double w, GizmoRef gizmo)
            : TexturePane(make_vec(x, y), Vec2f(h, w))
            , m_frozen(false)
            , m_gizmo(gizmo) {}
        GizmoPane(const Vec2f &position, const Vec2f &size, GizmoRef gizmo)
            : TexturePane(position, size)
            , m_frozen(false)
            , m_gizmo(gizmo) {}
        GizmoPane(const Rectf &rect, GizmoRef gizmo)
            : TexturePane(rect.getUpperLeft(), rect.getSize())
            , m_frozen(false)
            , m_gizmo(gizmo) {}

        virtual ~GizmoPane() {}

        void freeze(bool value = true)
        {
            m_frozen = value;
        }

        virtual void updatePane()
        {
            if ( m_frozen && m_texture )
                return;
            m_gizmo->updateGizmo();
            m_texture = render_to_texture(m_size.x,
                                          m_size.y,
                                          m_gizmo,
                                          true /* center */,
                                          true /* alpha */);
        }

        virtual cinder::gl::Texture renderPane()
        {
            if ( !m_texture )
                updatePane();
            return m_texture;
        }

      private:
        bool m_frozen;
        GizmoRef m_gizmo;
    };

  }
}

/// @}
