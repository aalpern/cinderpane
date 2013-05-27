#include <iostream>
#include <boost/lexical_cast.hpp>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"
#include "cinder/gl/Texture.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Rand.h"
#include "cinderpane/core/Interface.h"
#include "cinderpane/gizmo/API.h"
#include "cinderpane/ext/MovieApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace cinderpane::gizmo;
using namespace cinderpane::ext;
using namespace cinderpane::core;
using boost::lexical_cast;

static const float WIDTH  = 1280.0f;
static const float HEIGHT = 720.0f;

//=============================================================================
// Factory functions
//=============================================================================

static ColorA  trans(0.0, 0.0, 0.0, 0.0);
static ColorA  canvas            = solarized::base00;
static PartRef saveCommand       = make_shared<ContextSave>();
static PartRef restoreCommand    = make_shared<ContextRestore>();
static PartRef thinLineCommand   = make_shared<SetLinewidth>(1.0f);
static PartRef normalLineCommand = make_shared<SetLinewidth>(1.5f);
static PartRef thickLineCommand  = make_shared<SetLinewidth>(2.0f);

float y_start = 48;
float height  = 64;
float width   = 96;
float spacing = 12;
float radius  = height / 2;

void increment(Vec2f &pos) { pos.y += ( height + spacing ); }

GizmoRef make_grids_column()
{
	GizmoRef c = make_shared<Gizmo>(64, 0);
	Vec2f pos(0, y_start);

	c->push_back(normalLineCommand);

	// Rectangle
	c->push_back(make_shared<GridPart>(pos, Vec2f(width, height), 8, 4,
									   true, true, RectangleNormal));
	increment(pos);
	c->push_back(thinLineCommand);
	c->push_back(make_shared<GridPart>(pos, Vec2f(width, height), 8, 4,
									   false, false, RectangleNormal));
	c->back()->setDisplayState(IPart::DisplaySecondary);

	// Round Rect
	c->push_back(normalLineCommand);
	increment(pos);
	c->push_back(make_shared<GridPart>(pos, Vec2f(width, height), 8, 4,
									   true, false, RectangleRound));
	c->back()->setDisplayState(IPart::DisplayDisabled);
	increment(pos);
	c->push_back(make_shared<GridPart>(pos, Vec2f(width, height), 8, 4,
									   true, true, RectangleRound));

	// Curve rect
	increment(pos);
	c->push_back(make_shared<GridPart>(pos, Vec2f(width, height), 8, 4,
									   true, true, RectangleCurve));
	c->back()->setDisplayState(IPart::DisplayOK);

	increment(pos);
	c->push_back(make_shared<GridPart>(pos, Vec2f(width, height), 8, 4,
									   false, true, RectangleCurve));
	c->back()->setDisplayState(IPart::DisplayCritical);


	// Radial
	increment(pos);
	c->push_back(thinLineCommand);
	c->push_back(make_shared<RadialGridPart>(height / 1.8, 4, 12, true));
	c->back()->setPosition(pos);
	increment(pos);
	c->push_back(make_shared<RadialGridPart>(height / 1.8, 4, 12, true));
	c->back()->setOutline(false);
	c->back()->setDisplayState(IPart::DisplaySecondary);
	c->back()->setPosition(pos);
	increment(pos);
	c->push_back(make_shared<RadialGridPart>(height / 1.8, 4, 12, true));
	c->back()->setOutline(false);
	c->back()->setDisplayState(IPart::DisplayWarning);
	c->back()->setPosition(pos);

	return c;
}

GizmoRef make_arcs_column()
{
	GizmoRef c = make_shared<Gizmo>(196, 0);
    Vec2f pos(0, y_start);

	// Solid arc
    c->push_back(make_shared<ArcPart>(pos, radius, 8, 220, 30));

	// Filled arc
	increment(pos);
    c->push_back(make_shared<FilledArcPart>(pos, radius, 8, 220, 30));

	// Solid segmented arc
	increment(pos);
    c->push_back(make_shared<SegmentedArcPart>(pos, radius, 8, 6, 15));

	// Filled segmented arc
	increment(pos);
	c->push_back(saveCommand);
	c->push_back(thinLineCommand);
    c->push_back(make_shared<SegmentedFilledArcPart>(pos, radius, 12, 12, 16));
	c->back()->setDisplayState(IPart::DisplayWarning);
	c->push_back(restoreCommand);

	// Multiple rotating arcs
	increment(pos);
	c->push_back(make_shared<ArcPart>(pos, radius, 4, 0, 30));
	c->back()->setRotationSpeed(30);
	c->push_back(make_shared<ArcPart>(pos, radius, 4, 0, 30));
	c->back()->setRotationSpeed(32.5);
	c->push_back(make_shared<ArcPart>(pos, radius, 4, 0, 30));
	c->back()->setRotationSpeed(25);
	c->push_back(make_shared<ArcPart>(pos, radius, 4, 0, 30));
	c->back()->setRotationSpeed(22.5);
	c->push_back(make_shared<ArcPart>(pos, radius, 4, 0, 30));
	c->back()->setRotationSpeed(20);
	c->push_back(make_shared<ArcPart>(pos, radius, 4, 0, 30));
	c->back()->setRotationSpeed(17.5);
	c->push_back(make_shared<ArcPart>(pos, radius, 4, 0, 30));
	c->back()->setRotationSpeed(15);
	c->push_back(make_shared<ArcPart>(pos, radius - 8, 4, 180, 210));
	c->back()->setRotationSpeed(-15);
	c->back()->setDisplayState(IPart::DisplayOK);
	c->push_back(make_shared<ArcPart>(pos, radius - 8, 4, 210, 240));
	c->back()->setRotationSpeed(-15);
	c->back()->setDisplayState(IPart::DisplayWarning);
	c->push_back(make_shared<ArcPart>(pos, radius - 8, 4, 240, 270));
	c->back()->setRotationSpeed(-15);
	c->back()->setDisplayState(IPart::DisplaySubCritical);
	c->push_back(make_shared<ArcPart>(pos, radius - 8, 4, 270, 300));
	c->back()->setRotationSpeed(-15);
	c->back()->setDisplayState(IPart::DisplayCritical);
	c->push_back(make_shared<ArcPart>(pos, radius / 3, 1, 180, 320));
	c->back()->setRotationSpeed(-12);
	c->push_back(make_shared<ArcPart>(pos, (radius / 3) - 2, 1, 90, 270));
	c->back()->setRotationSpeed(11);

	increment(pos);
	c->push_back(make_shared<CirclePart>(pos, radius, true, true));
	increment(pos);
	c->push_back(make_shared<RectanglePart>(pos, Vec2f(width, height), RectangleNormal, true, true));
	// c->push_back(make_shared<TextPart>(pos, "RectangleNormal"));
	increment(pos);
	c->push_back(make_shared<RectanglePart>(pos, Vec2f(width, height), RectangleCurve, true, true));
	// c->push_back(make_shared<TextPart>(pos, "RectangleCurve"));
	increment(pos);
	c->push_back(make_shared<RectanglePart>(pos, Vec2f(width, height), RectangleRound, true, true));
	// c->push_back(make_shared<TextPart>(pos, "RectangleRound"));

	return c;
}

GizmoRef make_shapes_column()
{
	GizmoRef c = make_shared<Gizmo>(324, 0);
	Vec2f pos(0, y_start);

	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 3, true, true));
    c->back()->setRotation(270);

	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 3, true, true));
	c->back()->setDisplayState(IPart::DisplayDisabled);
    c->back()->setRotationSpeed(12);

	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 4, true, true));
	c->back()->setDisplayState(IPart::DisplaySecondary);
    c->back()->setRotation(45);

	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 4, true, true));
	c->back()->setDisplayState(IPart::DisplayHighlight);
    c->back()->setRotationSpeed(-12);

	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 5, true, true));
	c->back()->setDisplayState(IPart::DisplayOK);
    c->back()->setRotation(270);

	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 5, true, true));
	c->back()->setDisplayState(IPart::DisplayWarning);
    c->back()->setRotationSpeed(12);

	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 6, true, true));
	c->back()->setDisplayState(IPart::DisplayWarning);
    c->back()->setRotation(270);

	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 6, true, true));
	c->back()->setDisplayState(IPart::DisplaySubCritical);
    c->back()->setRotation(270);
    c->back()->setRotationSpeed(-12);

	// Stop sign
	increment(pos);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 8, true, true));
	c->back()->setDisplayState(IPart::DisplayCritical);
    c->back()->setRotation(22.5);
	c->push_back(make_shared<RegularPolygonPart>(pos, radius, 8, true, false));
	c->back()->setDisplayState(IPart::DisplayWarning);
    c->back()->setRotation(22.5);
	c->push_back(make_shared<TextPart>(pos, "STOP", 18.0));
	c->back()->setDisplayState(IPart::DisplayWarning);

	return c;
}


GizmoRef make_lines_column()
{
	GizmoRef c = make_shared<Gizmo>(452, 0);
	Vec2f pos(0, y_start);

	// Some simple lines
	c->push_back(make_shared<RadialLinesPart>(pos, radius, radius, 12));
	c->back()->setRotationSpeed(4);

	// Some other simple lines
	increment(pos);
	c->push_back(saveCommand);
	c->push_back(thickLineCommand);
	c->push_back(make_shared<RadialLinesPart>(pos, radius, 12, 6));
	c->back()->setRotationSpeed(-12);
	c->push_back(restoreCommand);

	// A bunch of tick mark lines for a radar display type dingus
	increment(pos);
	c->push_back(make_shared<RadialLinesPart>(pos, radius, 12, 4));
	c->back()->setDisplayState(IPart::DisplayOK);
	c->push_back(make_shared<RadialLinesPart>(pos, radius, 8, 4));
	c->back()->setRotation(45);
	c->push_back(make_shared<RadialLinesPart>(pos, radius, 4, 36));
	c->push_back(make_shared<RadialLinesPart>(pos, radius / 4, 6, 4));
	c->back()->setDisplayState(IPart::DisplaySubCritical);

	// Nested cross reticles
	increment(pos);
	c->push_back(saveCommand);
	c->push_back(make_shared<SetLinewidth>(2.0));
	c->push_back(make_shared<RadialLinesPart>(pos, radius, 12, 4));
	c->push_back(make_shared<SetLinewidth>(4.0));
	c->push_back(make_shared<RadialLinesPart>(pos, radius - 12, 8, 4));
	c->back()->setDisplayState(IPart::DisplayWarning);
	c->push_back(restoreCommand);


	// A set of radial lines restrict to a subset of angles
	increment(pos);
	c->push_back(saveCommand);
	c->push_back(make_shared<SetLinewidth>(4.0));
	c->push_back(make_shared<RadialLinesPart>(pos, radius, 6, 6, 180, 280));
	c->back()->setDisplayState(IPart::DisplayCritical);
	return c;
}

GizmoRef make_miscellany_column()
{
    GizmoRef c = make_shared<Gizmo>(580, 0);
    Vec2f pos(0, y_start);

    c->push_back(make_shared<GradientCirclePart>(pos, radius, radius, solarized::red, ColorA::white()));
	increment(pos);
    c->push_back(make_shared<GradientCirclePart>(pos, radius, radius / 2, ColorA(0.0f, 0.9f, 0.8f, 0.5f),
												 ColorA(0,0,0,0)));
	increment(pos);
    c->push_back(make_shared<GradientCirclePart>(pos, radius, radius / 2,
												 ColorA(1.0f, 0.5f, 0.5f, 0.8f),
												 ColorA(0,0,0,0)));
    c->push_back(make_shared<GradientCirclePart>(pos, radius / 2, radius / 2,
												 ColorA(0,0,0,0),
												 ColorA(1.0f, 0.5f, 0.5f, 0.8f)));

	return c;
}

GizmoRef make_radial_progress_indicator()
{
    GizmoRef g = make_shared<Gizmo>(836, 128);

    float increment = 16;

    // g->push_back(make_shared<CirclePart>(8, true, true));

    for ( int i = 2; i <= 6; i++ )
    {
        g->push_back(make_shared<RadialLinesPart>(i * increment, 1, 48, 0, 360));
        g->push_back(make_shared<ArcPart>((i * increment) + 3, 8, 0, 50 * i));
        g->back()->rotate(-90);
    }

    return g;
}

int seed = 0;

void custom1(GizmoContext &ctx)
{
	if ( ((int)getElapsedFrames() % (int)getFrameRate() ) == 0 )
		seed = clock();

	static const size_t value_count = 40;
	cinder::Rand r( seed );

	double values[value_count];
    ::memset(values, 0, sizeof(double) * value_count);
	for ( int i = 0; i < value_count; i++ )
		values[i] = r.nextFloat(0.0f, 0.99f);

	ctx.save();
	ctx.Cairo.setLineWidth(4);
	radial_bar_chart(ctx.Cairo, ctx.Position, 48, 96, values, value_count);
	ctx.draw(true, false, IPart::DisplayNormal);
	ctx.restore();
}

GizmoRef make_custom_1()
{
    GizmoRef g = make_shared<Gizmo>(836, 384);

	g->push_back(make_shared<CirclePart>(96, true, true));
	g->push_back(make_shared<CirclePart>(28, true, true));
	g->back()->setDisplayState(IPart::DisplayDisabled);
	g->push_back(make_shared<CustomFn>(custom1));

    return g;
}

GizmoRef make_fancy_compass(double radius)
{
	double outer = radius;

	// Inset the main parts a bit
	radius -= 12;
	GizmoRef g = make_shared<Gizmo>(1092, 128);
	g->push_back(saveCommand);

	// A subtle gradient around the edge
	g->push_back(make_shared<GradientCirclePart>(radius, radius / 2, solarized::blue, ColorA(0, 0, 0, 0)));
	g->push_back(make_shared<SetLinewidth>(2.0));

	// Give it a basic outline
	g->push_back(make_shared<CirclePart>(radius));
	// Notches
	g->push_back(make_shared<RadialLinesPart>(radius, radius / 8,  12));
	g->push_back(make_shared<RadialLinesPart>(radius, radius / 16,  48));

	// Add some dark circles in the center

	g->push_back(make_shared<CirclePart>(radius / 3));
	g->back()->setDisplayState(IPart::DisplayDisabled);
	g->push_back(make_shared<CirclePart>(radius / 2));
	g->back()->setDisplayState(IPart::DisplayDisabled);
	// And some meaningless greeble on the outside
	g->push_back(make_shared<SegmentedFilledArcPart>(outer, 8, 6, 15));
	g->back()->setDisplayState(IPart::DisplayHighlight);
	g->back()->setRotation(7.5);

	// Add the directional pointer; a triangle moved & angled around
	// the perimeter via PerimeterPart
	PartRef indicator = make_shared<RegularPolygonPart>(9, 3, true, true);
	indicator->setDisplayState(IPart::DisplaySubCritical);
	g->push_back(make_shared<PerimeterPart>(radius - 20,
											indicator,
											true));
	g->back()->setRotation(220);
	g->back()->setRotationSpeed(1);
	// Add a little reticle in the center
	g->push_back(make_shared<RadialLinesPart>(10, 8, 4));
	g->back()->setDisplayState(IPart::DisplaySubCritical);
	g->back()->setRotation(45);

	g->push_back(make_shared<FilledArcPart>(outer + 12, 6, 180, 270));
	g->back()->setRotationSpeed(-12);
	g->back()->setFilled(true);
//	g->back()->setDisplayState(IPart::DisplayCritical);
	g->push_back(make_shared<FilledArcPart>(outer + 20, 6, 0, 90));
	g->back()->setRotationSpeed(9);
	g->back()->setFilled(true);
//	g->back()->setDisplayState(IPart::DisplayWarning);

	g->push_back(restoreCommand);

	return g;
}

GizmoRef make_framistat(float scale = 2.5)
{
	GizmoRef g = make_shared<Gizmo>(1092, 128);

	g->push_back(make_shared<CirclePart>(28 * scale, true /* outline */, true /* fill */));
	g->push_back(make_shared<ArcPart>(30 * scale, 1 * scale, 225, 330));
	g->push_back(make_shared<ArcPart>(30 * scale, 1 * scale, 0, 110));
	g->push_back(make_shared<ArcPart>(32 * scale, 1 * scale, 90, 280));
	g->push_back(make_shared<GradientCirclePart>( 12 * scale, 12 * scale, trans, ColorA(1.0, 1.0, 1.0, 1.0)));
	g->push_back(make_shared<ArcPart>(36 * scale, 4 * scale, 45, 180));
	g->push_back(make_shared<FilledArcPart>(44 * scale, 20 * scale, 330, 30));
    g->back()->setDisplayState(IPart::DisplayWarning);
    g->back()->setRotationSpeed(5.0);

    g->push_back(saveCommand);
    g->push_back(make_shared<SetLinewidth>(1.5));
    // g->push_back(make_shared<SetLinecolor>(ColorA(0.4, 0.4, 0.4, 0.2)));
    // g->push_back(make_shared<SetFillcolor>(ColorA(0.2, 0.2, 0.2, 0.2)));
    g->push_back(make_shared<SegmentedFilledArcPart>(22 * scale, 8 * scale, 6, 15));
    g->back()->setDisplayState(IPart::DisplaySecondary);
    g->back()->setFilled(false);
    g->back()->setRotationSpeed(-5.0);
	g->push_back(restoreCommand);

	g->push_back(saveCommand);
	g->push_back(make_shared<SetLinewidth>(2.0 * scale));
	g->push_back(make_shared<RadialLinesPart>(42 * scale, 4 * scale, 6, 220, 280));
    g->back()->setDisplayState(IPart::DisplayCritical);
	g->push_back(restoreCommand);

	return g;
}

//=============================================================================
// App
//=============================================================================

class BasicGizmosApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();

  private:
	GizmoRef  m_gizmos;
	Interface m_interface;
};

void BasicGizmosApp::setup()
{
 	m_interface.setup(this);
	m_gizmos = make_shared<Gizmo>();

	m_gizmos->push_back(make_grids_column());
	m_gizmos->push_back(make_arcs_column());
	m_gizmos->push_back(make_shapes_column());
	m_gizmos->push_back(make_lines_column());
	m_gizmos->push_back(make_miscellany_column());
    m_gizmos->push_back(make_radial_progress_indicator());
    // m_gizmos->push_back(make_fancy_compass(96));
    m_gizmos->push_back(make_framistat());
    m_gizmos->push_back(make_custom_1());

	m_interface.push_back(make_shared<GizmoPane>(0.0f, 0.0f, WIDTH, HEIGHT, m_gizmos));
	glEnable( GL_TEXTURE_2D );
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

/*
void
BasicGizmosApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize( (int)WIDTH, (int)HEIGHT );
	settings->setFrameRate(  30.0f );
	settings->setTitle("Gizmo Catalog");
}
*/

void BasicGizmosApp::mouseDown( MouseEvent event )
{
}

void BasicGizmosApp::update()
{
	m_interface.update();
}

void BasicGizmosApp::draw()
{
	gl::clear();
	m_interface.render();
}

CINDER_APP_NATIVE( BasicGizmosApp, RendererGl )
