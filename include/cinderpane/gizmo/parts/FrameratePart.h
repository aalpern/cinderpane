#pragma once
#include <sstream>
#include "cinder/app/App.h"
#include "cinderpane/gizmo/parts/TextPart.h"

namespace cinderpane {
  namespace gizmo {

    class FrameratePart
        : public TextPart
    {
      public:
        FrameratePart()
            : TextPart("") {}
        FrameratePart(double fontSize)
            : TextPart("", fontSize) {}
        FrameratePart(const Vec2f &offset, double fontSize)
            : TextPart(offset, "", fontSize) {}

        virtual void updateGizmo()
        {
            std::ostringstream s;
            s << std::setprecision(2) << cinder::app::App::get()->getAverageFps();
            setText(s.str());
        }
    };

  }
}
