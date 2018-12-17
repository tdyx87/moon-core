/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_OS_EVENTCHANNEL_H_
#define MOON_OS_EVENTCHANNEL_H_

#include <moon/noncopyable.h>
#include <functional>
#include <memory>

namespace moon
{

class EventLoop; 

/**  
  EventChannel对象是对事件以及事件处理的抽象和封装，每个EventChannel对象支持4种不同类型的事件，
  读事件、写事件、关闭事件以及错误事件，并提供了对应事件的处理方法。
  EventChannel底层采用文件描述符来实现，通过对文件描述符读对应读事件，文件描述的写对应写事件，
  文件描述符的关闭对应关闭事件，文件描述出错对应错误事件。

  每个EventChannel对象有且只能有一个EventLoop对象，所有对EventChannel对象的操作必须在同一个EventLoop对象之类。

  每个EventChannel对象可以处于如下三个状态中的任意一个，EventChannel::kNew, EventChannel::kAdded, EventChannel::kDeleted.
  EventChannel::kNew表示对象处于创建状态，尚未加入任何事件监控队列。
  EventChannel::kAdded表示该对象已加入事件监控队列，并且有事件被监控。
  EventChannel::kDeleted表示该对象上已无事件被监控，或者该对象已被移除监控队列。

  可以通过enableReading(),disableReading(),enableWriting(),disableWriting(),disableAll()这5个方法来
  控制设置期望监控的事件。
  调用enableReading()开始监控事件的读事件。
  调用disableReading()停止监控事件的读事件。
  调用enableWriting()开始监控事件的写事件。
  调用disableWriting()停止监控事件的写事件。
  读写事件的监控互不干扰，例如：调用enableReading()开始监控事件的读事件，然后再调用enableWriting()开始监控事件的写事件，
  此时同时监控了事件的读写事件，disableReading()和disableWriting()类似，互不影响。
  在第一次调用enableReading()或enableWriting()时，会将该事件对象加入到监控队列。

  setReadCallback()设置读事件处理方法。
  setWriteCallback()设置写事件处理方法。
  setCloseCallback()当事件关闭处理方法。
  setErrorCallback()设置事件出错处理方法。

  可以调用disableAll()方法停止对事件的监控，该方法返回后将不监控该对象的任何事件。
  如果对该对象管理的事件已全部处理完毕，可调用remove()方法将该事件从循环队列移除。
  remove()方法和disableAll()都停止对对象所有事件的监控，disableAll()只停止监控对象的所有事件，
  并没有将事件对象移除循环队列，remove()方法会停止所有事件的监控，并且将事件对象从监控队列移除。
*/
class EventChannel : noncopyable
{
public:
    /**
      事件回调函数接口定义
    */
    typedef std::function<void()> EventCallback;
	
	EventChannel(int fd, EventLoop* eventLoop);	
	~EventChannel();	

	void handleEvent();
	void setReadCallback(const EventCallback& cb) {mReadCallback = cb;}
	void setWriteCallback(const EventCallback& cb) {mWriteCallback = cb;}
	void setCloseCallback(const EventCallback& cb) {mCloseCallback = cb;}
	void setErrorCallback(const EventCallback& cb) {mErrorCallback = cb;}
       
	void enableReading()      {mCurEvents |= kEventRead; this->update();}	
	void disableReading() { mCurEvents &= ~kEventRead; this->update();}
	void enableWriting() { mCurEvents |= kEventWrite; this->update();}
	void disableWriting() { mCurEvents &= ~kEventWrite; this->update();}
	void disableAll() {mCurEvents = kEventNone; this->update();}
	void remove();

	// 返回true表示当前对象期望有写事件被触发
	bool isWriting()const {return mCurEvents & kEventWrite;}

	// 返回true表示当前对象不期望有任何事件被触发
	bool isNoneEvent()const {return kEventNone == mCurEvents;}   
    
	// 返回该对象管理的描述符
	int getFd()const {return mFd;}

	// 返回被期望监控的事件
    int getCurrentEvents()const {return mCurEvents;}

	// 设置当前被触发的事件
	void setActiveEvents(int events) {mActiveEvents = events;}
    
	// 设置对象的监控状态
	void setState(int state) {mState = state;}

	// 获取对象的监控状态
	int getState()const {return mState;}		

	EventLoop* ownerLoop() { return mEventLoop; }
	void tie(const std::shared_ptr<void>& tie);

private:
	void update();
private:
	static const int kEventNone;
    static const int kEventRead;
    static const int kEventWrite;

	int mFd;            // 描述符(并非一定是socket描述符)
    int mCurEvents;     // 希望监听事件类型
	int mActiveEvents;  // 当前活动的事件
	int mState;         // 通道当前状态
	EventLoop* mEventLoop;

	bool mEventHandling;
	bool mAddedToLoop;
	std::weak_ptr<void> mTie;
	bool mTied;

	EventCallback mReadCallback;
	EventCallback mWriteCallback;
	EventCallback mCloseCallback;
	EventCallback mErrorCallback;
};

}  // ~moon

#endif  // ~
