#pragma once
#include <map>
#include <string>
#include "cinderpane/gizmo/Part.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
	namespace gizmo {

		struct IPartFactory
		{
			virtual ~IPartFactory() {}

			virtual PartRef makePart(const std::string &type = "") const = 0;
		};
		typedef boost::shared_ptr<IPartFactory> PartFactoryRef;

		template<typename PartType>
		class BasicPartFactory
			: public IPartFactory
		{
		  public:
			typedef PartType part_type;

			BasicPartFactory() {}
			virtual ~BasicPartFactory() {}

			virtual PartRef makePart(const std::string &type = "") const
			{
				return PartRef(new part_type());
			}
		};

		class PartFactory
			: public IPartFactory
		{
			typedef std::map<std::string, PartFactoryRef> factory_map;
		  public:
			PartFactory() {}
			virtual ~PartFactory() {}

			template<typename PartType>
			void add_factory(const std::string &typeName)
			{
				m_factories[typeName] = PartFactoryRef(new BasicPartFactory<PartType>());
			}

			virtual PartRef makePart(const std::string &type = "") const
			{
				factory_map::const_iterator i = m_factories.find(type);
				if ( i == m_factories.end() )
				{
					// TODO - define some exceptions to throw
					return PartRef();
				}

				return i->second->makePart(type);
			}

		  protected:
			factory_map m_factories;
		};
	}
}

/// @}
