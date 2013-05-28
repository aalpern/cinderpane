#pragma once
#include "cinderpane/gizmo/PartBase.h"
#include "cinderpane/util/vector.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    /// A container for multiple parts. CompositePart implements
    /// the \b std::vector<> interface, so it can be used with an
    /// STL algorithms which can operate on a vector.
    class CompositePart
        : public PartBase
    {
        typedef PartBase base_type;
      public:
        CompositePart() {}
        CompositePart(const Vec2f &size)
            : base_type(size) {}
        CompositePart(double radius)
            : base_type(radius) {}
        CompositePart(const Vec2f &size, const Vec2f &position)
            : base_type(size, position) {}
        CompositePart(double radius, const Vec2f &position)
            : base_type(radius, position) {}

        virtual ~CompositePart() {}

        virtual IPart* setDisplayState(DisplayState state)
        {
            PartBase::setDisplayState(state);
            for ( auto part : m_parts )  {
                part->setDisplayState(state);
            }
            return this;
        }

        /// @name IRenderable
        /// @{
        virtual void updateGizmo()
        {
            base_type::updateGizmo();
            for ( auto part : m_parts ) {
                part->updateGizmo();
            }
        }

        virtual void renderGizmo(GizmoContext &context)
        {
            if ( !isVisible() )
                return;

            base_type::renderGizmo(context);
            for ( auto part : m_parts ) {
                if (part->isVisible()) {
                    part->renderGizmo(context);
                }
            }
        }
        /// @}

        /// @name IPersistable
        /// @{
        virtual void loadXML(const cinder::XmlTree &element,
                             const PartFactoryRef &factory)
        {
            for ( auto child : element ) {
                PartRef part = factory->makePart(child.getTag());
                part->loadXML(child, factory);
                push_back(part);
            }
        }

        virtual void storeXML(std::ostream &os) const
        {
            for ( auto part : m_parts ) {
                part->storeXML(os);
            }
        }
        /// @}

        CINDERPANE_DELEGATE_VECTOR(PartVector, m_parts);

      protected:
        container_type m_parts;
    };
    typedef shared_ptr<CompositePart> CompositePartRef;
  }
}

/// @}
