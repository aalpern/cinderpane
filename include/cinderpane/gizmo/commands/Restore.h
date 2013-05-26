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

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory)
			{
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlWriteTag(os, "Restore");
			}
		};

	}
}

/// @}
