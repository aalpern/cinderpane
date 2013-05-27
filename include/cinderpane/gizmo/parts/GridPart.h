#pragma once
#include <string.h>
#include "cinder/cairo/Cairo.h"
#include "cinderpane/ext/Utils.h"
#include "cinderpane/ext/Shapes.h"
#include "cinderpane/gizmo/PartBase.h"
#include "cinderpane/gizmo/parts/RectanglePart.h"

namespace cinderpane {
  namespace gizmo {

    using namespace cinderpane::ext;

    class GridPart
        : public PartBase
    {
      public:
        GridPart(double radius = DEFAULT_RADIUS)
            : PartBase(radius)
            , m_hdivisions(4)
            , m_vdivisions(4)
            , m_type(RectangleNormal)
        {
        }

        GridPart(const cinder::Vec2f &size,
                 int hdivisions, int vdivisions,
                 bool outline = true, bool fill = false, RectangleType type = RectangleNormal)
            : PartBase(size)
            , m_type(type)
            , m_hdivisions(hdivisions)
            , m_vdivisions(vdivisions)
        {
            m_outline = outline;
            m_fill = fill;
        }

        GridPart(double radius,
                 int hdivisions, int vdivisions,
                 bool outline = true, bool fill = false, RectangleType type = RectangleNormal)
            : PartBase(radius)
            , m_type(type)
            , m_hdivisions(hdivisions)
            , m_vdivisions(vdivisions)
        {
            m_outline = outline;
            m_fill = fill;
        }

        GridPart(const cinder::Vec2f &position, const cinder::Vec2f &size,
                 int hdivisions, int vdivisions,
                 bool outline = true, bool fill = false, RectangleType type = RectangleNormal)
            : PartBase(size, position)
            , m_type(type)
            , m_hdivisions(hdivisions)
            , m_vdivisions(vdivisions)
        {
            m_outline = outline;
            m_fill = fill;
        }

        virtual ~GridPart() {}

        virtual void updateGizmo()
        {
        }

        virtual void renderGizmo(GizmoContext &ctx)
        {
            CairoSaveState save(ctx.Cairo);

            // upper left
            cinder::Vec2f position = ( ctx.Position + m_position ) - ( m_size / 2 );
            // cinder::Vec2f center  = ctx.Position + m_position;

            // Outline & fill the grid
            ctx.Cairo.newPath();
            switch (m_type)
            {
              case RectangleNormal:
                  ctx.Cairo.rectangle(position.x, position.y, m_size.x, m_size.y);
                  break;
              case RectangleRound:
                  round_rect(ctx.Cairo, position, position + m_size);
                  break;
              case RectangleCurve:
                  curve_rect(ctx.Cairo, position, position + m_size);
                  break;
            }
            ctx.draw(m_outline, m_fill, m_state);
            ctx.Cairo.newPath();


            // Set clipping
            switch ( m_type )
            {
              case RectangleRound:
                  round_rect(ctx.Cairo, position, position + m_size);
                  ctx.Cairo.clip();
                  break;
              case RectangleCurve:
                  curve_rect(ctx.Cairo, position, position + m_size);
                  ctx.Cairo.clip();
                  break;
              default:
                  break;
            }

            // Draw the grid
            grid(ctx.Cairo, position, position + m_size, m_hdivisions, m_vdivisions);
            ctx.draw(true, false, m_state);

        }

        /// @name Accessors
        /// @{
        int getHDivisions() const
        {
            return m_hdivisions;
        }

        void setHDivisions(int value)
        {
            m_hdivisions = value;
        }

        int getVDivisions() const
        {
            return m_vdivisions;
        }

        void setVDivisions(int value)
        {
            m_vdivisions = value;
        }

        void setGridDivisions(int hdiv, int vdiv)
        {
            m_hdivisions = hdiv;
            m_vdivisions = vdiv;
        }
        /// @}

        virtual void loadXML(const cinder::XmlTree &element,
                             const PartFactoryRef &factory)
        {
            PartBase::loadXML(element, factory);
            m_hdivisions = xmlGetChildValue<int>(element, "HDiv", 4);
            m_vdivisions = xmlGetChildValue<int>(element, "VDiv", 4);
            m_type       = toType(xmlGetChildValue<std::string>(element, "Type", "rectangle").c_str());
        }

        virtual void storeXML(std::ostream &os) const
        {
            xmlBeginPart(os, "Grid");
            xmlWriteTag(os, "HDiv", m_hdivisions);
            xmlWriteTag(os, "VDiv", m_vdivisions);
            xmlWriteTag(os, "Type", toString(m_type));
            xmlEndTag(os, "Grid");
        }

      protected:
        static const char * toString(RectangleType type)
        {
            switch ( type ) {
              case RectangleCurve:
                  return "curve";
              case RectangleRound:
                  return "round";
              case RectangleNormal:
              default:
                  return "normal";
            }
        }

        static RectangleType toType(const char *str)
        {
            if ( strcmp(str, "curve") == 0 )
                return RectangleCurve;
            if ( strcmp(str, "round") == 0 )
                return RectangleRound;
            if ( strcmp(str, "normal") == 0 )
                return RectangleNormal;
            return RectangleNormal;
        }

        RectangleType m_type;
        int			  m_hdivisions;
        int			  m_vdivisions;
    };


  }
} // end namespace cinderpane::gizmo
