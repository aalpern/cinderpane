#pragma once
#include "cinderpane/gizmo/CommandBase.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    class ContextSave : public CommandBase
    {
      public:
        ContextSave() {}

        virtual void renderGizmo(GizmoContext &ctx)
        {
            ctx.save();
        }
    };

  }
}

/// @}
