#pragma once
#include "cinderpane/gizmo/CommandBase.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    class SetLinewidth : public CommandBase
    {
      public:
        SetLinewidth()
            : m_lineWidth(1.0) {}
        SetLinewidth(double lineWidth)
            : m_lineWidth(lineWidth) {}

        virtual ~SetLinewidth() {}

        virtual void renderGizmo(GizmoContext &ctx)
        {
            ctx.Cairo.setLineWidth(m_lineWidth);
        }

        virtual void loadXML(const cinder::XmlTree &element,
                             const PartFactoryRef &factory)
        {
            m_lineWidth = element.getValue<double>(m_lineWidth);
        }

        virtual void storeXML(std::ostream &os) const
        {
            xmlWriteTag(os, "SetLineWidth", m_lineWidth);
        }

      protected:
        double m_lineWidth;
    };

  }
}

/// @}
