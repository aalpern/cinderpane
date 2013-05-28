#pragma once
#include "cinderpane/gizmo/CommandBase.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
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

      protected:
        ColorA m_lineColor;
    };

  }
}

/// @}
