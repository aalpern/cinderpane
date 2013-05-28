#pragma once
#include "cinderpane/gizmo/CommandBase.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    class ContextRestore : public CommandBase
    {
      public:
        ContextRestore() {}

        virtual void renderGizmo(GizmoContext &ctx)
        {
            ctx.restore();
        }
    };

  }
}

/// @}
