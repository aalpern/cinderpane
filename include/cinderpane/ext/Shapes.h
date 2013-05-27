#pragma once
#include "cinder/Cinder.h"
#include "cinder/CinderMath.h"
#include "cinder/cairo/Cairo.h"
#include "cinderpane/ext/Utils.h"

namespace cinderpane {
	namespace ext {

		using cinder::Vec2f;
		using cinder::Rectf;
		using cinder::cairo::Context;

		//=============================================================================
		// Rectangular Shapes
		//=============================================================================

		/// Taken from Cairo Samples at
		/// http://cairographics.org/samples/curve_rectangle/
		void curve_rect(Context &ctx, double left, double top, double right, double bottom, double radius = -1.0f)
		{
			double width  = right - left;
			double height = bottom - top;
			double x0     = left;
			double y0     = top;
			double x1     = x0 + width;
			double y1     = y0 + height;

			if ( radius < 0.0f )
				radius = height / 5.0f;

			if ((width / 2) < height) {
				if ((height / 2) < radius) {
					ctx.moveTo (x0, (y0 + y1) / 2);
					ctx.curveTo(x0, y0, x0, y0, (x0 + x1) / 2, y0);
					ctx.curveTo(x1, y0, x1, y0, x1, (y0 + y1) / 2);
					ctx.curveTo(x1, y1, x1, y1, (x1 + x0) / 2, y1);
					ctx.curveTo(x0, y1, x0, y1, x0, (y0 + y1) / 2);
				} else {
					ctx.moveTo (x0, y0 + radius);
					ctx.curveTo(x0, y0, x0, y0, (x0 + x1) / 2, y0);
					ctx.curveTo(x1, y0, x1, y0, x1, y0 + radius);
					ctx.lineTo (x1, y1 - radius);
					ctx.curveTo(x1, y1, x1, y1, (x1 + x0) / 2, y1);
					ctx.curveTo(x0, y1, x0, y1, x0, y1- radius);
				}
			} else {
				if ((height / 2) < radius) {
					ctx.moveTo (x0, (y0 + y1) / 2);
					ctx.curveTo(x0, y0, x0, y0, x0 + radius, y0);
					ctx.lineTo (x1 - radius, y0);
					ctx.curveTo(x1, y0, x1, y0, x1, (y0 + y1) / 2);
					ctx.curveTo(x1, y1, x1, y1, x1 - radius, y1);
					ctx.lineTo (x0 + radius, y1);
					ctx.curveTo(x0, y1, x0, y1, x0, (y0 + y1) / 2);
				} else {
					ctx.moveTo (x0,          y0 + radius);
					ctx.curveTo(x0,          y0, x0, y0, x0 + radius, y0);
					ctx.lineTo (x1 - radius, y0);
					ctx.curveTo(x1,          y0, x1, y0, x1, y0 + radius);
					ctx.lineTo (x1,          y1 - radius);
					ctx.curveTo(x1,          y1, x1, y1, x1 - radius, y1);
					ctx.lineTo (x0 + radius, y1);
					ctx.curveTo(x0,          y1, x0, y1, x0, y1- radius);
				}
			}
			ctx.closePath();
		}

		void curve_rect(Context &ctx, const Rectf &rect, double radius = -1.0f)
		{
			curve_rect(ctx, rect.x1, rect.y1, rect.x2, rect.y2, radius);
		}

		void curve_rect(Context &ctx, const Vec2f &upperLeft, const Vec2f &lowerRight, double radius = -1.0f)
		{
			curve_rect(ctx, upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y, radius);
		}


		/// Take from Cairo Samples at
		/// http://cairographics.org/samples/rounded_rectangle/
		void round_rect(Context &ctx, double left, double top, double right, double bottom,
						double cornerRadius = -1.0f)
		{
			double x      = left;
			double y      = top;
			double width  = right - left;
			double height = bottom - top;
			double aspect = width / height;
			double degrees = M_PI / 180.0;

			if ( cornerRadius < 0.0f )
				cornerRadius = height / 5.0f;
			double radius  = cornerRadius / aspect;

			ctx.arc(x + width - radius, y + radius,          radius, -90 * degrees,   0 * degrees);
			ctx.arc(x + width - radius, y + height - radius, radius,   0 * degrees,  90 * degrees);
			ctx.arc(x + radius,         y + height - radius, radius,  90 * degrees, 180 * degrees);
			ctx.arc(x + radius,         y + radius,          radius, 180 * degrees, 270 * degrees);
			ctx.closePath();
		}

		void round_rect(Context &ctx, const Rectf &rect, double cornerRadius = -1.0f)
		{
			round_rect(ctx, rect.x1, rect.y1, rect.x2, rect.y2, cornerRadius);
		}

		void round_rect(Context &ctx, const Vec2f &upperLeft, const Vec2f &lowerRight, double cornerRadius = -1.0f)
		{
			round_rect(ctx, upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y, cornerRadius);
		}

		/// Draw a simple regular rectangular grid with the current
		/// context settings. The number of rows and columns can be
		/// specified indepdendently.
		void grid(cinder::cairo::Context &ctx,
				  double left, double top, double right, double bottom,
				  int hcells, int vcells)
		{
			double width       = right  - left;
			double height      = bottom - top;
			double cell_width  = width  / hcells;
			double cell_height = height / vcells;

			// Draw the vertical lines
			for ( int i = 1; i < hcells; i++ )
			{
				ctx.moveTo(left + cell_width * i, top);
				ctx.lineTo(left + cell_width * i, bottom);
			}

			// Draw the horizontal lines
			for ( int i = 1; i < vcells; i++ )
			{
				ctx.moveTo(left,  top + cell_height * i);
				ctx.lineTo(right, top + cell_height * i);
			}
		}

		/// Draw a simple rectangular grid, using a single \c Rectf
		/// object to specify the bounds.
		void grid(Context &ctx, const Rectf &rect, int hcells, int vcells)
		{
			grid(ctx, rect.x1, rect.y1, rect.x2, rect.y2, hcells, vcells);
		}

		/// Draw a simple rectangular grid, using a pair of Vec2f
		/// objects to specify the bounds.
		void grid(Context &ctx, const Vec2f &upperLeft, const Vec2f &lowerRight,
				  int hcells, int vcells)
		{
			grid(ctx, upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y, hcells, vcells);
		}

		//=============================================================================
		// Radial Shapes
		//=============================================================================

		/// Draw a hollow arc segment. This is accomplished by drawing
		/// 2 concentric arcs and the lines between their endpoints.
		void hollow_arc(Context &ctx,
						const Vec2f &position,
						double innerRadius, double outerRadius,
						double begin, double end)
		{
			// Draw the border
			Vec2f outerBegin = perimeter(position, outerRadius, begin);
			// Vec2f outerEnd   = perimeter(position, outerRadius, end);
			Vec2f innerBegin = perimeter(position, innerRadius, begin);
			Vec2f innerEnd   = perimeter(position, innerRadius, end);

			ctx.moveTo(innerBegin);
			ctx.lineTo(outerBegin);
			ctx.arc(position, outerRadius, begin, end);
			ctx.lineTo(innerEnd);
			ctx.arcNegative(position, innerRadius, end, begin);
			ctx.closePath();
		}

		template<typename Ctx>
		inline void
		hollow_arc_2(Ctx &ctx, const Vec2f &pos,
				   double outerRadius, double innerRadius, double begin, double end)
		{
			Vec2f b1 = perimeter(pos, outerRadius, begin);
			// Vec2f b2 = perimeter(pos, innerRadius, begin);
			// Vec2f e1 = perimeter(pos, outerRadius, end);
			Vec2f e2 = perimeter(pos, innerRadius, end);

			ctx.moveTo(b1);
			ctx.arc(pos, outerRadius, begin, end);
			ctx.lineTo(e2);
			ctx.arc(pos, innerRadius, end, begin, false);
			ctx.lineTo(b1);
		}


		/// Inspired by the "backchannel visualizer" from Stamen. See
		/// http://stamen.com/projects/backchannel
		///
		/// @todo Add angle range restriction
		///
		/// @param values The values to chart; they should all be
		/// normalized to the range 0.0 - 1.0
		void radial_bar_chart(Context &ctx, const Vec2f &position, double minRadius, double maxRadius,
							  double *values, size_t valueCount,
							  double startAngle = cinder::toRadians(180.0))
		{
			double angleStep = (M_PI * 2) / valueCount;
			double angle     = startAngle;

			for ( size_t i = 0; i < valueCount; i++ )
			{
				double value  = values[i];
				double height = cinder::lmap(value, 0.0, 1.0, minRadius, maxRadius);
				Vec2f  p1     = perimeter(position, minRadius, angle);
				Vec2f  p2     = perimeter(position, height, angle);

				ctx.moveTo(p1);
				ctx.lineTo(p2);

				angle = wrap_radians(angle + angleStep);
			}
		}

		//=============================================================================
		// Curves
		//=============================================================================

		void curve_between(Context &ctx, double x1, double y1, double x2, double y2)
		{
			// TBD
		}

		void curve_between(Context &ctx, const Vec2f &p1, const Vec2f &p2)
		{
			curve_between(ctx, p1.x, p1.y, p2.x, p2.y);
		}

	}
}
