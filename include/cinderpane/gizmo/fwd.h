#pragma once
#include <vector>
#include "cinderpane/shared_ptr.h"
#include "cinder/Cinder.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    using cinder::Vec2f;
    using cinder::Rectf;
    using cinder::ColorAf;
    using cinder::ColorA;

    using std::make_shared;

    struct IRenderable;
    typedef shared_ptr<IRenderable> RenderableRef;

    struct IPart;
    typedef std::shared_ptr<IPart> PartRef;
    typedef std::vector<PartRef>     PartVector;

    class GizmoContext;

    static const double DEFAULT_RADIUS = 48.0;
  }
}

/// @}
