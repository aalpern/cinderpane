#pragma once
#include <queue>
#include <boost/thread.hpp>

namespace cinderpane {

	/// Based on Anthony Williams' boost-based thread safe-queue
	/// implementation using locks and condition variables. Not quite
	/// as performant under high load as using a lockless queue, but
	/// that's harder to do portably, and easy to swap in if needed.
	///
	/// http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

	template<typename Data>
	class concurrent_queue
	{
	  public:
		typedef Data value_type;

		void enqueue(value_type const& data)
		{
			boost::mutex::scoped_lock lock(m_mutex);
			m_queue.push(data);
			lock.unlock();
			m_condvar.notify_one();
		}

		bool empty() const
		{
			boost::mutex::scoped_lock lock(m_mutex);
			return m_queue.empty();
		}

		bool try_dequeue(value_type& popped_value)
		{
			boost::mutex::scoped_lock lock(m_mutex);

			if ( m_queue.empty() )
			{
				return false;
			}

			popped_value = m_queue.front();
			m_queue.pop();
			return true;
		}

		void dequeue(value_type& popped_value)
		{
			boost::mutex::scoped_lock lock(m_mutex);

			while ( m_queue.empty() )
			{
				m_condvar.wait(lock);
			}

			popped_value = m_queue.front();
			m_queue.pop();
		}

	  private:
		std::queue<value_type>    m_queue;
		mutable boost::mutex      m_mutex;
		boost::condition_variable m_condvar;
	};

}
