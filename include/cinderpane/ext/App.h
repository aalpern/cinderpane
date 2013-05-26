#pragma once
#include <string>
#include "cinder/app/AppBasic.h"
#include "cinderpane/cinder/ThreadedMovieWriter.h"

namespace cinderpane {
	namespace ext {

		/// An extended app base class, so I don't have to write the
		/// same code over and over again for each project.
		///
		/// Provides support for rendering a quicktime movie of your
		/// Cinder app. Once started, the movie can finish after a
		/// specified number of frames, or continue recording until
		/// the application exits.
		///
		/// The movie will be finalized when the application
		/// exits. Recording of the movie can be re-enabled at any
		/// time during the lifetime of the application.
		///
		/// Rendering is done using the ThreadedMovieWriter wrapper,
		/// so that movie recording will not slow down the main
		/// thread.

		class CinderpaneApp
			: public cinder::app::AppBasic
		{
		  public:
			CinderpaneApp()
                : cinder::app::AppBasic()
				, m_movieFrameCount(-1)
				, m_movieEnabled(false) {}
			virtual ~CinderpaneApp() {}

			void setupMovie(unsigned long frameCount, bool chooseSettings = false)
			{
				m_movieFrameCount = frameCount;
				if ( m_moviePath == "" )
					m_moviePath = getSaveFilePath();
				if ( chooseSettings )
					cinder::qtime::MovieWriter::getUserCompressionSettings(&m_movieFormat);
				m_movie.setWriter(cinder::qtime::MovieWriter(m_moviePath, getWindowWidth(), getWindowHeight(), m_movieFormat));
				m_movie.start();
			}

			void startMovie()
			{
				m_movieEnabled = true;
			}

			void stopMovie()
			{
				m_movieEnabled = false;
			}

			virtual void update()
			{
				doUpdate();
			}

			virtual void draw()
			{
				doDraw();
				if ( m_movieEnabled )
				{
					m_movie.addFrame( copyWindowSurface() );
					if ( (m_movieFrameCount > 0) && (m_movie.getNumFrames() >= m_movieFrameCount) )
						m_movieEnabled = false;
				}
			}

			virtual void shutdown()
			{
				m_movie.finish();
				m_movie.stop();

				doShutdown();
			}

			virtual void doUpdate() = 0;
			virtual void doDraw() = 0;
			virtual void doShutdown() {}

		  protected:
			bool                               m_movieEnabled;
			ThreadedMovieWriter                m_movie;
			cinder::qtime::MovieWriter::Format m_movieFormat;
			std::string                        m_moviePath;
			long                               m_movieFrameCount;
		};

	}
}
