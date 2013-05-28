#pragma once
#include "cinderpane/gizmo/CommandBase.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    class CustomFn : public CommandBase
    {
      public:
        typedef void (*function_type)(GizmoContext &);

        CustomFn(function_type fn)
            : m_fn(fn) {}

        virtual ~CustomFn() {}

        virtual void renderGizmo(GizmoContext &ctx)
        {
            m_fn(ctx);
        }

      private:
        function_type m_fn;
    };
  }
}

/// @}
