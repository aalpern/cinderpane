#pragma once
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "cinder/Cinder.h"
#include "cinderpane/gizmo/PartFactory.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    /// An interface for storing and retrieving configurations of
    /// gizmos and parts from XML.
    struct IPersistable
    {
        virtual ~IPersistable() {}
    };
  }
}

/// @}
