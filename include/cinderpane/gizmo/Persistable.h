#pragma once
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "cinder/Cinder.h"
#include "cinder/Xml.h"
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

			virtual void loadXML(const cinder::XmlTree &element,
								 const PartFactoryRef &factory) = 0;
			virtual void storeXML(std::ostream &os) const = 0;
		};
	}
}

/// @}
