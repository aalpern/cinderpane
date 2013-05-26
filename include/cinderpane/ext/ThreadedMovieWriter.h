#pragma once
#include "cinderpane/util/queue.h"
#include "cinder/qtime/MovieWriter.h"

namespace cinderpane {
	namespace ext {

		/// A class which wraps some extremely simple threading around
		/// a Cinder MovieWriter. MovieWriter provides a very simple
		/// interface for rendering out a quicktime movie, but it's
		/// entirely synchronous, which slows down your live
		/// animation. By handling the QuickTime I/O on a separate
		/// thread, we can render a movie without slowing down the main
		/// Cinder rendering loop.
		///
		/// This class simply wraps an existing MovieWriter instance,
		/// providing a background thread for handling the calls to
		/// MovieWriter and a simple FIFO queue for queueing frames
		/// and commands from the main Cinder thread to the movie
		/// thread.
		///
		/// Slightly better would be to have this simply be a set of
		/// tasks to run on a generic thread pool, but that's more
		/// than a 10 minute hack.

		class ThreadedMovieWriter
		{
		  public:
			/// Construct an uninitialized ThreadedMovieWriter. The
			/// underlying Cinder MovieWriter object must be set later
			/// with setWriter() before use.
			ThreadedMovieWriter() {}

			/// Construct a ThreadedMovieWriter wrapping an existing
			/// instance of MovieWriter which has been initialized
			/// elsewhere.
			ThreadedMovieWriter(cinder::qtime::MovieWriter writer)
				: m_writer(writer) {}

			/// Construct a ThreadedMovieWriter and instantiate the
			/// underlying MovieWriter object with the supplied
			/// parameters. The signature is the same as that of the
			/// construct for MovieWriter.
			ThreadedMovieWriter(const std::string &path, int32_t width, int32_t height,
								const cinder::qtime::MovieWriter::Format &format
								= cinder::qtime::MovieWriter::Format())
				: m_writer(cinder::qtime::MovieWriter(path, width, height, format)) {}

			~ThreadedMovieWriter()
			{
				finish();
				stop();
			}

            void setWriter(cinder::qtime::MovieWriter writer)
            {
                m_writer = writer;
            }

			void start()
			{
				m_thread = boost::thread(boost::ref(*this));
			}

			void finish()
			{
				m_queue.enqueue(Command(Finish));
			}

			void stop()
			{
				m_queue.enqueue(Command(Stop));
				m_thread.join();
			}

			void addFrame(cinder::Surface frame)
			{
				m_queue.enqueue(Command(Add, frame));
			}

			uint32_t getNumFrames() const
			{
				if ( m_writer )
					return m_writer.getNumFrames();
				return 0;
			}

			void operator()()
			{
				while ( true ) {
					Command cmd;
					m_queue.dequeue(cmd);
					switch ( cmd.type ) {
					  case None:
						  break;
					  case Add:
						  m_writer.addFrame(cmd.surface);
						  break;
					  case Finish:
						  m_writer.finish();
						  break;
					  case Stop:
						  m_writer.finish();
						  return;
					}
				}
			}

		  private:
			enum CommandType {
				None, Add, Finish, Stop
			};
			struct Command {
				CommandType     type;
                cinder::Surface surface;

                Command()
                    : type(None) {}
                Command(CommandType type)
					: type(type) {}
				Command(CommandType type, cinder::Surface surface)
					: type(type), surface(surface) {}
			};

			cinder::qtime::MovieWriter         m_writer;
			cinderpane::concurrent_queue<Command> m_queue;
			boost::thread                      m_thread;
		};

	}
}
