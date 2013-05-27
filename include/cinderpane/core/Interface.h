#pragma once
#include <time.h>
#include <sstream>
#include "cinder/app/AppBasic.h"
#include "cinder/Utilities.h"
#include "cinder/DataSource.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinderpane/core/Pane.h"
#include "cinderpane/core/MouseListener.h"
#include "cinderpane/core/KeyListener.h"
#include "cinderpane/core/EventDispatcher.h"
#include "cinderpane/util/vector.h"

/// @addtogroup Core
/// @{

namespace cinderpane {
  namespace core {

    class SaveViewport {
      public:
        SaveViewport() {
            vp = cinder::gl::getViewport();
        }
        ~SaveViewport() {
            cinder::gl::setViewport(vp);
        }
      private:
        cinder::Area vp;
    };

    class SetWindowMatrix {
      public:
        SetWindowMatrix(bool topleft = true) {
            cinder::gl::pushMatrices();
            cinder::gl::setMatricesWindow(cinder::app::getWindowWidth(),
                                          cinder::app::getWindowHeight(),
                                          topleft);
        }
        ~SetWindowMatrix() {
            cinder::gl::popMatrices();
        }
    };

    class SetAlphaBlend {
      public:
        SetAlphaBlend() {
            cinder::gl::enableAlphaBlending();
        }
        ~SetAlphaBlend() {
            cinder::gl::disableAlphaBlending();
        }
    };

    void render_shader(cinder::gl::GlslProg &shader, cinder::gl::Fbo &fboInput, cinder::gl::Fbo &fboOutput)
    {
        using namespace cinder;
        gl::SaveFramebufferBinding binding;
        SaveViewport               viewport;

        fboOutput.bindFramebuffer();
        cinder::gl::setViewport(fboOutput.getBounds());
        fboInput.getTexture().enableAndBind();

        gl::clear(ColorA(0.0, 0.0, 0.0, 0.0));
        gl::draw(fboInput.getTexture());

        shader.unbind();
        fboInput.getTexture().unbind();
    }


    /// <b>Hit Testing and Event Handling</b>
    class Interface
    {
      public:
        Interface()
            : m_app(NULL)
            , m_visible(true)
            , m_useFrameBuffer(true)
            , m_useGlow(false)
        {
        }

        Interface(cinder::app::AppBasic *app)
            : m_app(app)
            , m_visible(true)
            , m_useFrameBuffer(true)
            , m_useGlow(false)
        {
            setup(app);
        }

        ~Interface() {}

        void add(PaneRef pane)
        {
            push_back(pane);

            KeyListenerRef keyListener = dynamic_pointer_cast<IKeyListener>(pane);
            if ( keyListener )
                m_dispatcher.add(keyListener);

            MouseListenerRef mouseListener = dynamic_pointer_cast<IMouseListener>(pane);
            if ( mouseListener )
                m_dispatcher.add(mouseListener);
        }

        bool isVisible() const
        {
            return m_visible;
        }

        void setVisible(bool value)
        {
            if ( m_visible == value )
                return;
            m_visible = value;
        }

        void screenshot()
        {
            time_t t;
            ::time(&t);
            tm * ti = ::localtime(&t);
            char buffer[96];
            ::strftime(buffer, 96, "%Y-%m-%d %H-%M-%S", ti);

            std::ostringstream filepath;
            filepath << cinder::getDocumentsDirectory() << cinder::getPathSeparator()
                     << cinder::app::App::get()->getSettings().getTitle() << "-" << buffer << ".png";

            cinder::writeImage( filepath.str(), cinder::app::copyWindowSurface(), cinder::ImageTarget::Options(), "png" );
        }

        bool onKeyDown(cinder::app::KeyEvent event)
        {
            if ( event.isAltDown() )
            {
                switch ( event.getChar() ) {
                  case 'i':
                      m_visible = !m_visible;
                      return true;
                  case 's':
                      screenshot();
                      return true;
                  case 'g':
                      m_useGlow = !m_useGlow;
                      return true;
                  case 'f':
                      m_useFrameBuffer = !m_useFrameBuffer;
                      return true;
                  case '-': // increase alpha
                  case '=': // decrease alpha
                      return true;
                }
            }
            return false;
        }

        void setup(cinder::app::AppBasic *app)
        {
            m_app = app;

            m_format.enableMipmapping(false);
            m_format.enableDepthBuffer(false);
            m_format.setCoverageSamples(16);
            m_format.setSamples(4);
            m_framebuffer        = cinder::gl::Fbo(app->getWindowWidth(), app->getWindowHeight(), m_format);
            m_framebuffer_shaded = cinder::gl::Fbo(app->getWindowWidth(), app->getWindowHeight(), m_format);

            app->registerKeyDown(      this,          &Interface::onKeyDown);
            app->registerResize(       this,          &Interface::onResize);
            app->registerMouseDown(    &m_dispatcher, &EventDispatcher::onMouseDown);
            app->registerMouseUp(      &m_dispatcher, &EventDispatcher::onMouseUp);
            app->registerMouseWheel(   &m_dispatcher, &EventDispatcher::onMouseWheel);
            app->registerMouseMove(    &m_dispatcher, &EventDispatcher::onMouseMove);
            app->registerMouseDrag(    &m_dispatcher, &EventDispatcher::onMouseDown);
            app->registerKeyDown(      &m_dispatcher, &EventDispatcher::onKeyDown);
            app->registerKeyUp(        &m_dispatcher, &EventDispatcher::onKeyUp);
            app->registerTouchesBegan( &m_dispatcher, &EventDispatcher::onTouchesBegan);
            app->registerTouchesMoved( &m_dispatcher, &EventDispatcher::onTouchesMoved);
            app->registerTouchesEnded( &m_dispatcher, &EventDispatcher::onTouchesEnded);

            try {
#ifdef _WIN32
                m_glow_shader = cinder::gl::GlslProg(cinder::loadFile("passThru_vert.glsl"),
                                                     cinder::loadFile("gaussianBlur_frag.glsl"));
#else
                m_glow_shader = cinder::gl::GlslProg(m_app->loadResource("passThru_vert.glsl"),
                                                     m_app->loadResource("gaussianBlur_frag.glsl"));
#endif
            } catch ( const std::exception &ex ) {
                std::cout << ex.what() << std::endl;
            }
        }

        bool onResize(cinder::app::ResizeEvent event)
        {
            return false;
        }

        void update()
        {
            if ( !m_visible )
                return;

            for ( iterator i = begin(); i != end(); i++ )
            {
                PaneRef pane = *i;
                if ( pane )
                    pane->updatePane();
            }
        }

        void render()
        {
            if ( !m_visible )
                return;

            if ( m_useFrameBuffer )
            {
                render_framebuffer();
                if ( m_useGlow )
                    render_framebuffer_shaded();
                render_framebuffer_to_screen();
            } else {
                render_plain();
            }
        }

        CINDERPANE_DELEGATE_VECTOR(PaneVector, m_panes)

        private:
        void render_framebuffer()
        {
            cinder::gl::SaveFramebufferBinding binding;
            SaveViewport                       viewport;

            m_framebuffer.bindFramebuffer();
            cinder::gl::setViewport(m_framebuffer.getBounds());
            cinder::gl::clear(cinder::ColorA(0.0, 0.0, 0.0, 0.0));

            render_plain();
            m_framebuffer.unbindFramebuffer();
        }

        void render_framebuffer_shaded()
        {
            if ( !m_glow_shader || !m_app )
                return;

            m_glow_shader.bind();
            m_glow_shader.uniform( "tex0", 0 );
            m_glow_shader.uniform( "sampleOffset", Vec2f( cos( .0 ),
                                                          sin( .0 ) ) * ( 1.0f / m_app->getWindowWidth() ) );

            SetWindowMatrix matrix;
            render_shader(m_glow_shader, m_framebuffer, m_framebuffer_shaded);
        }


        void render_framebuffer_to_screen()
        {
            cinder::gl::enableAlphaBlending();
            cinder::gl::disableDepthRead();
            cinder::gl::pushMatrices();
            cinder::gl::setMatricesWindow(cinder::app::getWindowWidth(), cinder::app::getWindowHeight(), false );

            cinder::gl::draw( m_framebuffer.getTexture(), Vec2f::zero() );


            if ( m_useGlow )
            {
                SetWindowMatrix matrix;
                cinder::gl::enableAdditiveBlending();
                cinder::gl::color( cinder::Color::white() );
                cinder::gl::draw( m_framebuffer_shaded.getTexture(), Vec2f(0.0f, 0.0f));
            }

            cinder::gl::popMatrices();
            cinder::gl::disableAlphaBlending();
            cinder::gl::enableDepthRead();
        }

        void render_plain()
        {
            cinder::gl::disableDepthRead();
            cinder::gl::disableDepthWrite();
            SetWindowMatrix matrix;
            SetAlphaBlend   blend;
            draw();
        }

        void draw()
        {
            for ( iterator i = begin(); i != end(); i++ )
            {
                PaneRef pane = *i;
                if ( pane )
                {
                    pane->drawPane();
                }
            }
        }

        bool                    m_useGlow;
        bool                    m_useFrameBuffer;
        cinder::app::AppBasic * m_app;
        PaneVector              m_panes;
        bool                    m_visible;
        EventDispatcher         m_dispatcher;
        cinder::gl::Fbo         m_framebuffer;
        cinder::gl::Fbo         m_framebuffer_shaded;
        cinder::gl::Fbo::Format m_format;
        cinder::gl::GlslProg    m_glow_shader;
    };
  }
}

/// @}
