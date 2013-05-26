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

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory)
			{
			}

			virtual void storeXML(std::ostream &os) const
			{
				xmlWriteTag(os, "Save");
			}
		};

	}
}

/// @}
