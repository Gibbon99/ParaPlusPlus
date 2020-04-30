/**
	Main header for the FileWatcher class. Declares all implementation
	classes to reduce compilation overhead.

	@author James Wynn
	@date 4/15/2009

	Copyright (c) 2009 James Wynn (james@jameswynn.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#ifndef _FW_FILEWATCHER_H_
#define _FW_FILEWATCHER_H_
#pragma once

#include <string>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <queue>
#include <unordered_map>

namespace FW
{
	/// Type for a string
	typedef std::string String;
	/// Type for a watch id
	typedef unsigned long WatchID;

	// forward declarations
	class FileWatcherImpl;
	class FileWatchListener;

	/// Base exception class
	/// @class Exception
	class Exception : public std::runtime_error
	{
	public:
		Exception(const String& message)
			: std::runtime_error(message)
		{}
	};

	/// Exception thrown when a file is not found.
	/// @class FileNotFoundException
	class FileNotFoundException : public Exception
	{
	public:
		FileNotFoundException()
			: Exception("File not found")
		{}

		FileNotFoundException(const String& filename)
			: Exception("File not found (" + filename + ")")
		{}
	};

	/// Actions to listen for. Rename will send two events, one for
	/// the deletion of the old file, and one for the creation of the
	/// new file.
	namespace Actions
	{
		enum Action
		{
			/// Sent when a file is created or renamed
			Add = 1,
			/// Sent when a file is deleted or renamed
			Delete = 2,
			/// Sent when a file is modified
			Modified = 4
		};
	};
	typedef Actions::Action Action;

	/// Listens to files and directories and dispatches events
	/// to notify the parent program of the changes.
	/// @class FileWatcher
	class FileWatcher
	{
	public:
		///
		///
		FileWatcher();

		///
		///
		~FileWatcher();

		/// Add a directory watch. Same as the other addWatch, but doesn't have recursive option.
		/// For backwards compatibility.
		/// @exception FileNotFoundException Thrown when the requested directory does not exist
		WatchID addWatch(const String& directory, FileWatchListener* watcher);

		/// Add a directory watch
		/// @exception FileNotFoundException Thrown when the requested directory does not exist
		WatchID addWatch(const String& directory, FileWatchListener* watcher, bool recursive);

		/// Remove a directory watch. This is a brute force search O(nlogn).
		void removeWatch(const String& directory);

		/// Remove a directory watch. This is a map lookup O(logn).
		void removeWatch(WatchID watchid);

		/// Updates the watcher. Must be called often.
		void update();

	private:
		/// The implementation
		FileWatcherImpl* mImpl;

	};//end FileWatcher

	/// Basic interface for listening for file events.
	/// @class FileWatchListener
	class FileWatchListener
	{
	public:
		FileWatchListener() {}
		virtual ~FileWatchListener() {}

		/// Handles the action file action
		/// @param watchid The watch id for the directory
		/// @param dir The directory
		/// @param filename The filename that was accessed (not full path)
		/// @param action Action that was performed
		virtual void handleFileAction(WatchID watchid, const String& dir, const String& filename, Action action) = 0;

	};//class FileWatchListener


	struct FileWatcherEvent
	{
		WatchID id;
		String dir;
		String filename;
		Actions::Action action;
	};

	class SingleThreadedPollingFileWatcher
	{
		class Listener : public FileWatchListener
		{
		public:
			inline Listener(SingleThreadedPollingFileWatcher& watcher) : m_watcher(watcher) {}

		public:
			virtual void handleFileAction(WatchID watchid, const String& dir, const String& filename, Action action) override final;

		private:
			SingleThreadedPollingFileWatcher & m_watcher;
		};

		friend class SingleThreadedPollingFileWatcher::Listener;
	public:
		inline SingleThreadedPollingFileWatcher() :
			m_listener(*this)
		{
		}

		inline ~SingleThreadedPollingFileWatcher()
		{
		}

	public:
		inline WatchID addWatch(const String& path, bool recursive)
		{
			WatchID id = m_watcher.addWatch(path, &m_listener, recursive);
			m_watches.insert({
				path,
				id
				});
			return id;
		}

		inline void removeWatch(const String& path)
		{
			if (m_watches.count(path) > 0)
			{
				auto ret = m_watches.at(path);
				m_watches.erase(path);
				m_watcher.removeWatch(ret);
			}
		}

		inline void removeWatch(const WatchID& id)
		{
			for (auto it : m_watches)
			{
				if (it.second == id)
				{
					m_watches.erase(it.first);
					m_watcher.removeWatch(id);
					return;
				}
			}
		}

		inline void update() 
		{
			m_watcher.update();
		}

		inline bool poll(FileWatcherEvent& ev)
		{
			//m_watcher.update();
			if (m_events.size() > 0)
			{
				ev = m_events.front();
				m_events.pop();
				return true;
			}
			return false;
		}

		inline bool get_events(std::queue<FileWatcherEvent>& dest)
		{
			if (m_events.size() > 0)
			{
				dest.swap(m_events);
				return true;
			}
			return false;
		}

	private:
		FileWatcher m_watcher;
		Listener m_listener;
		std::queue<FileWatcherEvent> m_events;
		std::unordered_map<std::string, WatchID> m_watches;
	};

	class PollingFileWatcher
	{
		static void thread(PollingFileWatcher* watcher)
		{
			while (watcher->m_running)
			{
				{
					std::lock_guard<std::mutex> lock(watcher->m_mutex);
					watcher->m_watcher.update();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(watcher->m_updaterate));
			}
		}
	public:
		inline PollingFileWatcher() :
			m_running(true), m_updaterate(50)
		{
			m_thread = std::thread(thread, this);
		}

		inline ~PollingFileWatcher()
		{
			m_running = false;
			m_thread.join();
		}

	public:
		inline WatchID addWatch(const String& path, bool recursive)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_watcher.addWatch(path, recursive);
		}

		inline void removeWatch(const String& path)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_watcher.removeWatch(path);
		}

		inline void removeWatch(const WatchID& id)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_watcher.removeWatch(id);
		}

		inline void update()
		{
			// no-op
		}

		inline bool poll(FileWatcherEvent& ev)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_watcher.poll(ev);
		}

		inline bool get_events(std::queue<FileWatcherEvent>& dest)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_watcher.get_events(dest);
		}

		inline void setThreadFrequency(int ms = 50)
		{
			m_updaterate = ms;
		}

	private:
		bool m_running;
		SingleThreadedPollingFileWatcher m_watcher;
		std::mutex m_mutex;
		std::thread m_thread;
		int m_updaterate;
	};

};//namespace FW

#endif//_FW_FILEWATCHER_H_
