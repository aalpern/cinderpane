#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"
#include "cinderpane/gizmo/API.h"
#include "cinderpane/gizmo/Util.h"
#include "cinderpane/core/Interface.h"
#include "cinderpane/core/TextPane.h"
#include "cinderpane/gizmo/GizmoPane.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace cinderpane;
using namespace cinderpane::gizmo;
using namespace cinderpane::ext;
using namespace cinderpane::core;

#define MOVIE 0
static const int WIDTH   = 1280;
static const int HEIGHT  = 720;

static ColorAf cyan_dark        = ColorAf(0.0f, 0.60f, 0.70f, 0.6f);
static PartRef saveCommand      = make_shared<ContextSave>();
static PartRef restoreCommand   = make_shared<ContextRestore>();

//=============================================================================
// Gizmo
//=============================================================================

static const int GIZMO_WIDTH  = 256;
static const int GIZMO_HEIGHT = 256;

GizmoRef make_gizmo(float scale = 1.0)
{
    GizmoRef g = make_shared<Gizmo>();

    static ColorA  trans (0.0f, 0.0f, 0.0f, 0.0f);

    g->push_back(make_shared<CirclePart>(28 * scale, true /* outline */, true /* fill */));
    g->push_back(make_shared<ArcPart>(30 * scale, 1 * scale, 225, 330));
    g->push_back(make_shared<ArcPart>(30 * scale, 1 * scale, 0, 110));
    g->push_back(make_shared<ArcPart>(32 * scale, 1 * scale, 90, 280));
    g->push_back(make_shared<GradientCirclePart>( 12 * scale, 12 * scale, trans, ColorA(1.0, 1.0, 1.0, 1.0)));
    g->push_back(make_shared<ArcPart>(36 * scale, 4 * scale, 45, 180));
    g->push_back(make_shared<FilledArcPart>(44 * scale, 20 * scale, 330, 30));
    g->back()->setDisplayState(IPart::DisplayHighlight);
    g->back()->setRotationSpeed(8.0);

    g->push_back(saveCommand);
    g->push_back(make_shared<SetLinewidth>(1.5));
    g->push_back(make_shared<SegmentedFilledArcPart>(22 * scale, 8 * scale, 6, 15));
    g->back()->setDisplayState(IPart::DisplayDisabled);
    g->back()->setRotationSpeed(-8.0);
    g->push_back(restoreCommand);

    g->push_back(saveCommand);
    g->push_back(make_shared<SetLinewidth>(2.0 * scale));
    g->push_back(make_shared<RadialLinesPart>(42 * scale, 4 * scale, 6, 220, 280));
    g->back()->setDisplayState(IPart::DisplayCritical);
    g->push_back(restoreCommand);

    return g;
}

GizmoRef make_cube_gizmo(GizmoRef original)
{
    GizmoRef g = make_shared<Gizmo>();
    g->insert(g->begin(), original->begin(), original->end());
    g->push_back(make_shared<RectanglePart>(GIZMO_HEIGHT / 2));
    return g;
}

static IPart::DisplayState state = IPart::DisplayNormal;
float padding = 12;

void custom(GizmoContext &ctx)
{
    float x_center = WIDTH / 2;

    ctx.save();
    ctx.Cairo.setLineWidth(2.0);

    // Start in center, then line to mid-point of left-most circular display
    ctx.Cairo.moveTo(x_center, HEIGHT - padding);
    ctx.Cairo.lineTo(padding + GIZMO_WIDTH / 2, HEIGHT - padding);

    // Draw most of a circle
    Vec2f center(padding + GIZMO_WIDTH / 2, HEIGHT - padding - (GIZMO_HEIGHT / 2));
    ctx.Cairo.arc(center, GIZMO_HEIGHT / 2, cinder::toRadians(90.0f), cinder::toRadians(45.0f));
    Vec2f point = cinderpane::ext::perimeter(center, GIZMO_HEIGHT / 2, cinder::toRadians(45.0f));
    // Vec2f nextPoint = point + Vec2f(16, 16);

    ctx.Cairo.lineTo(WIDTH - point.x, point.y);
    center.x = WIDTH - center.x;
    ctx.Cairo.arc(center, GIZMO_HEIGHT / 2, cinder::toRadians(135.0), cinder::toRadians(90.0f));

    ctx.Cairo.closePath();

    ctx.draw(true, true, ::state);
    ctx.restore();
}

GizmoRef make_custom()
{
    GizmoRef g = make_shared<Gizmo>();
    g->push_back(make_shared<CustomFn>(custom));
    return g;
}

GizmoRef make_reticle()
{
    GizmoRef g = make_shared<Gizmo>(WIDTH / 2, HEIGHT / 2);

    g->push_back(make_shared<RectanglePart>(WIDTH / 10, HEIGHT / 10, true, true));
    g->push_back(make_shared<GridPart>(Vec2f(WIDTH / 10, HEIGHT / 10), 2, 2, false, false));


    return g;
}


GizmoRef make_fancy_compass(double radius)
{
    double outer = radius;

    // Inset the main parts a bit
    radius -= 12;
    GizmoRef g = make_shared<Gizmo>();
    g->push_back(saveCommand);

    // A subtle gradient around the edge
    g->push_back(make_shared<GradientCirclePart>(radius, radius / 2, cyan_dark, ColorA(0, 0, 0, 0)));
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
    g->back()->setDisplayState(IPart::DisplayCritical);
    g->push_back(make_shared<FilledArcPart>(outer + 20, 6, 0, 90));
    g->back()->setRotationSpeed(9);
    g->back()->setFilled(true);
    g->back()->setDisplayState(IPart::DisplayWarning);

    g->push_back(restoreCommand);
// g->storeXML(App::get()->console());


    return g;
}

//=============================================================================
// App
//=============================================================================

class ThreeDeeApp : public AppNative {
  public:
    void setup();
    // void prepareSettings(Settings *settings);
    // void resize( ResizeEvent event );
    void mouseDown( MouseEvent event );
    void update();
    void draw();

  private:
    /** Definition of the UI gizmo to draw */
    GizmoRef     m_gizmo;
    /** Definition of the UI gizmo to draw */
    GizmoRef     m_gizmo_cube;
    /** GL texture to render the gizmo into */
    gl::Texture  m_texture;
    /** The camera to view the world through */
    CameraPersp  m_camera;

    Interface m_interface;

    Matrix44f m_cube_rotation;

};

void ThreeDeeApp::setup()
{
    setWindowSize( WIDTH, HEIGHT );
    setFrameRate(  30.0f );
    getWindow()->setTitle("ThreeDee Test");

    m_interface.setup(this);

    m_gizmo         = make_gizmo(2.5);
    m_gizmo_cube    = make_cube_gizmo(m_gizmo);
    m_camera.lookAt( Vec3f(3, 2, -3), Vec3f::zero() );
    m_cube_rotation.setToIdentity();

    m_interface.add(make_shared<GizmoPane>(Vec2f(0, 0), Vec2f(WIDTH, HEIGHT), make_custom()));
    m_interface.add(make_shared<GizmoPane>(Vec2f(padding, HEIGHT - GIZMO_HEIGHT - padding ),
                                           Vec2f(GIZMO_WIDTH, GIZMO_HEIGHT),
                                           make_fancy_compass((GIZMO_HEIGHT / 2) * .75)));
    m_interface.add(make_shared<GizmoPane>(Vec2f(WIDTH - padding - GIZMO_WIDTH, HEIGHT - GIZMO_HEIGHT - padding),
                                           Vec2f(GIZMO_WIDTH, GIZMO_HEIGHT),
                                           m_gizmo));
    m_interface.add(make_shared<GizmoPane>(Vec2f(500, 300), Vec2f(WIDTH / 10.0f, HEIGHT / 10.0f), make_reticle()));

    glEnable( GL_TEXTURE_2D );
    gl::enableDepthRead();
    gl::enableDepthWrite();
}

/*
  void ThreeDeeApp::resize( ResizeEvent event )
  {
  // now tell our Camera that the window aspect ratio has changed
  m_camera.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );

  // and in turn, let OpenGL know we have a new camera
  gl::setMatrices( m_camera );
  }
*/

void ThreeDeeApp::mouseDown( MouseEvent event )
{
}

void ThreeDeeApp::update()
{
    if ( !m_gizmo )
        return;
    m_interface.update();
    m_gizmo->updateGizmo();
    m_texture = render_to_texture(GIZMO_WIDTH, GIZMO_HEIGHT, m_gizmo_cube, true /* center */, false /* alpha */);

    // Rotate the cube by .03 radians around an arbitrary axis
    m_cube_rotation.rotate( Vec3f( 1.1, 0.9, 1 ), 0.05f );
}

void ThreeDeeApp::draw()
{
    if ( !m_texture )
        return;

    gl::enableAlphaBlending();
    // gl::clear( ColorAf( 0.2, 0.2, 0.2, 0.0 ) );
    gl::clear();

    // Draw the cube
    gl::enableDepthRead();
    gl::enableDepthWrite();
    m_texture.bind();
    gl::pushMatrices();
    gl::multModelView( m_cube_rotation );
    gl::drawCube( Vec3f::zero(), Vec3f(2.0f, 2.0f, 2.0f ));
    gl::popMatrices();
    m_texture.unbind();

    // Draw the interface
    m_interface.render();
}

CINDER_APP_NATIVE( ThreeDeeApp, RendererGl )
