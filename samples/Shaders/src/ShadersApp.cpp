#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include <boost/lexical_cast.hpp>
#include "cinder/Cinder.h"
#include "cinder/DataSource.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Utilities.h"
#include "cinderpane/ext/MovieApp.h"
#include "cinderpane/core/Interface.h"
#include "cinderpane/gizmo/API.h"
#include "cinderpane/gizmo/Util.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace cinderpane::gizmo;
using namespace cinderpane::ext;

static const int WIDTH  = 600;
static const int HEIGHT = 600;

#define MOVIE 0

//=============================================================================
// Gizmo Factory
//=============================================================================

static ColorA  trans    (0.0f, 0.0f, 0.0f, 0.0f);
static ColorA  cyan     (0.0f, 0.7f, 0.8f, 1.0f);
static ColorA  cyan_fill(0.0f, 0.2f, 0.3f, 0.7f);
static PartRef saveCommand      = make_shared<ContextSave>();
static PartRef restoreCommand   = make_shared<ContextRestore>();

// Draw a sim-card shape (just a rectangle with one clipped corner)
void draw_sim(cairo::Context &ctx, const Rectf &rect, int orientation = 0) {
    float cut_rate = 0.20;
    float cut = rect.getWidth() * cut_rate;

    ctx.moveTo(rect.x1, rect.y1);
    ctx.lineTo(rect.x2 - cut, rect.y1);
    ctx.lineTo(rect.x2, rect.y1 + cut);
    ctx.lineTo(rect.x2, rect.y2);
    ctx.lineTo(rect.x1, rect.y2);
    ctx.lineTo(rect.x1, rect.y1);
}

void custom(GizmoContext &context) {
    context.save();
    cairo::Context &c = context.Cairo;

    Rectf r(10, 10, 50, 70);
    for ( int i = 0; i < 10; i++ )
    {
        draw_sim(c, r);
        context.fillPreserve();
        c.setSource(context.LineColor);
        c.stroke();

        context.LineColor -= 0.08;
        context.FillColor -= 0.08;

        r += Vec2f(50,0);
    }
    context.restore();
}


GizmoRef make_gizmo(float x, float y, float scale = 1.0) {
    GizmoRef g = make_shared<Gizmo>(x, y);

    g->push_back(make_shared<SetLinecolor>(cyan));
    g->push_back(make_shared<SetFillcolor>(cyan_fill));

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

    g->push_back(make_shared<CustomFn>(custom));
    return g;
}


//=============================================================================
// App
//=============================================================================

class ShadersApp : public AppNative {
  public:
    void setup();
    void mouseDown( MouseEvent event );
	void keyDown(KeyEvent event);
    void update();
    void draw();

  private:
    void render_framebuffer_shaded();
    void draw_shaded();
    void draw_plain();

    /** Definition of the UI gizmo to draw */
    GizmoRef        m_gizmo;
    /** Framebuffer to render the gizmo into */
    gl::Fbo         m_framebuffer;
    /** Framebuffer to render the results of the shader */
    gl::Fbo         m_framebuffer_shaded;

    gl::GlslProg    m_shader;
    gl::Fbo::Format m_format;
    bool            m_bDrawShaded;
};

void ShadersApp::setup()
{
#if MOVIE
	setupMovie(600);
	startMovie();
#endif

    m_format.enableMipmapping(false);
    m_format.setCoverageSamples(16);
    m_format.setSamples(4);

    m_bDrawShaded        = true;
	m_gizmo              = make_gizmo( WIDTH / 2, HEIGHT / 2, 4.0 );
    m_framebuffer        = gl::Fbo(WIDTH, HEIGHT, m_format);
    m_framebuffer_shaded = gl::Fbo(WIDTH, HEIGHT, m_format);

#if 0
    //	write_xml(cinder::getDocumentsDirectory() + "/gizmo.xml", m_gizmo);
	try {
		m_gizmo = read_xml(cinder::getDocumentsDirectory() + "/gizmo.xml");
	} catch ( const std::exception &ex ) {
		std::cerr << "Failed to load gizmo: " << ex.what() << std::endl;
	}
#endif

    try {
//        m_shader = gl::GlslProg(loadResource("v002.AnalogGlitch.vert"),
//                                loadResource("v002.AnalogGlitch.frag"));
//                m_shader = gl::GlslProg(loadResource("v002.blur.vert"),
//                                        loadResource("v002.blur.frag"));
#ifdef _WIN32
                m_shader = gl::GlslProg(loadFile("passThru_vert.glsl"),
                                        loadFile("gaussianBlur_frag.glsl"));
#else
        m_shader = gl::GlslProg(loadResource("passThru_vert.glsl"),
                                loadResource("gaussianBlur_frag.glsl"));
#endif
    } catch ( const std::exception &ex ) {
        std::cout << ex.what() << std::endl;
    }
}

/*
void ShadersApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize( WIDTH, HEIGHT );
	settings->setFrameRate(  30.0f );
	settings->setTitle("Shader Test");
}
*/

void ShadersApp::mouseDown( MouseEvent event )
{
}

void ShadersApp::keyDown( KeyEvent event )
{
    if ( event.getChar() == 'g' )
        m_bDrawShaded = !m_bDrawShaded;
}

void ShadersApp::update()
{
	if ( m_gizmo )
	{
		m_gizmo->updateGizmo();
		cinderpane::gizmo::render_to_framebuffer(m_framebuffer, m_gizmo);
	}

}

void ShadersApp::draw()
{
    if ( !m_gizmo || !m_framebuffer )
        return;

	gl::setViewport( getWindowBounds() );

    draw_plain();

    if ( m_bDrawShaded )
    {
        render_framebuffer_shaded();
        draw_shaded();
    }

}

void ShadersApp::render_framebuffer_shaded()
{
	m_shader.bind();
	m_shader.uniform( "tex0", 0 );
	m_shader.uniform( "sampleOffset", Vec2f( cos( .0 ),
                                            sin( .0 ) ) * ( 1.0f / getWindowWidth() ) );

	cinderpane::core::render_shader(m_shader, m_framebuffer, m_framebuffer_shaded);
}

void ShadersApp::draw_plain()
{
	gl::clear( ColorA( 1.0, 0.0, 0.0, 1.0 ) );
	gl::pushMatrices();
	gl::setMatricesWindow(WIDTH, HEIGHT, false );
    gl::draw( m_framebuffer.getTexture() );
	gl::popMatrices();
}

void ShadersApp::draw_shaded()
{
    gl::enableAdditiveBlending();
    gl::color( Color::white() );
    gl::draw( m_framebuffer_shaded.getTexture() );
    gl::disableAlphaBlending();
}


CINDER_APP_NATIVE( ShadersApp, RendererGl )
