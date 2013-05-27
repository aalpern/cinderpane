#pragma once
#include "cinderpane/gizmo/Parts.h"
#include "cinderpane/gizmo/Commands.h"
#include "cinderpane/gizmo/Gizmo.h"
#include "cinderpane/gizmo/GizmoPane.h"

/// @addtogroup Gizmo
/// @{

namespace cinderpane {
  namespace gizmo {

    PartFactoryRef make_part_factory()
    {
        shared_ptr<PartFactory> factory( new PartFactory() );

        // Basic parts
        factory->add_factory<ArcPart>("Arc");
        factory->add_factory<CirclePart>("Circle");
        factory->add_factory<FilledArcPart>("FilledArc");
        factory->add_factory<GradientCirclePart>("GradientCircle");
        factory->add_factory<GridPart>("Grid");
        factory->add_factory<PerimeterPart>("Perimeter");
        factory->add_factory<RadialGridPart>("RadialGrid");
        factory->add_factory<RadialLinesPart>("RadialLines");
        factory->add_factory<RectanglePart>("Rectangle");
        factory->add_factory<RectangularReticlePart>("RectangularReticle");
        factory->add_factory<RegularPolygonPart>("RegularPolygon");
        factory->add_factory<SegmentedArcPart>("SegmentedArc");
        factory->add_factory<SegmentedFilledArcPart>("SegmentedFilledArc");
        factory->add_factory<TextPart>("Text");
        factory->add_factory<FrameratePart>("Framerate");

        // Commands
        factory->add_factory<SetLinewidth>("SetLineWidth");
        factory->add_factory<SetLinecolor>("SetLineColor");
        factory->add_factory<SetFillcolor>("SetFillColor");
        factory->add_factory<ContextSave>("Save");
        factory->add_factory<ContextRestore>("Restore");

        // Gizmo
        factory->add_factory<Gizmo>("Gizmo");

        return factory;
    }

    void write_xml(const std::string &path, GizmoRef gizmo)
    {
        std::ofstream s(path.c_str());
        gizmo->storeXML(s);
        s.close();
    }

    GizmoRef read_xml(const std::string &path)
    {
        cinder::XmlTree doc( cinder::loadFile(path) );
        GizmoRef gizmo = make_shared<Gizmo>();
        gizmo->loadXML(doc, make_part_factory());
        return gizmo;
    }
  }
}

/// @}
