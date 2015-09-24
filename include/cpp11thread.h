#ifndef CONNECTION_POOL_H_
#define CONNECTION_POOL_H_
#include "main.h"
#include<mutex>
#include<string>
#include<iostream>
#include"mysqloperation.h"
#include<vector>
#include<functional>
#include<list>
#include<thread>
#include <condition_variable>
#include <assert.h> 
#include <atomic>
#include <memory>
#include <ctime>
#include <chrono>

using namespace std;

namespace edwin
{

	template<class T,class R>
	class Cpp11Thread	
	{  
	public:
		Cpp11Thread():m_b_is_finish(false){}
		~Cpp11Thread()
		{
			m_list_tasks.clear();
		}

		//创建线程或获取任务
		int append(Task<T,R> task)
		{
			if (m_pthread.get() == nullptr)
				m_pthread = shared_ptr<thread>(new thread(std::bind(&Cpp11Thread<T,R>::run,this)));
			size_t tasknum = 0;
			m_list_tasks_mutex.lock();
			m_list_tasks.push_back(task);
			tasknum = m_list_tasks.size();
			if(tasknum == 1)
				m_cond_incoming_task.notify_one();
			m_list_tasks_mutex.unlock();
			return tasknum;
		}
		//运行线程
		void join()
		{
			terminate();
			if (m_pthread.get() != nullptr)
			{
				m_pthread->join();
			}
		}
		//等待直到线程没有任务可以运行
		void wait_for_idle()
		{
			chrono::duration<int,milli> milliseconds(200);
			while(load())
				this_thread::sleep_for(milliseconds);
		}
		//中止线程
		void terminate()
		{
			m_b_is_finish=false;
			m_cond_incoming_task.notify_one();
		}
		//加载到的任务数目
		size_t load()
		{
			size_t tasknum;
			m_list_tasks_mutex.lock();
			tasknum = m_list_tasks.size();
			m_list_tasks_mutex.unlock();
			return tasknum;
		}
		//分离线程
		void thread_detach()
		{
			m_pthread->detach();
		}
	protected:
		atomic< bool> 		m_b_is_finish;
		list<Task<T,R>> m_list_tasks;
		mutex				m_list_tasks_mutex; 	
		shared_ptr<thread>	m_pthread;
		mutex				m_cond_mutex;			
		condition_variable	m_cond_incoming_task;
		//执行
		void run()
		{
			Task<T,R> curr_task;
			while (!m_b_is_finish)
			{

			    bool bHasTasks = false;
			    m_list_tasks_mutex.lock();
			    if (!m_list_tasks.empty())
			    {
			    	bHasTasks = true;
			        curr_task = *m_list_tasks.begin();
			    }
			    m_list_tasks_mutex.unlock();
			    //doing task
			    if (bHasTasks)
			    {
			    	curr_task.func(curr_task.arg);
			        m_list_tasks_mutex.lock();
			        m_list_tasks.pop_front();
			        m_list_tasks_mutex.unlock();
			    }
			    if (!load())
			    {
			    	chrono::duration<int,milli> timeout(5000);
			        unique_lock<mutex> m_cond_locker(m_cond_mutex);
			        if (m_cond_locker.mutex())
			        {
			        	m_cond_incoming_task.wait_for(m_cond_locker,timeout);//m_cond_incoming_task.wait(m_cond_locker);
			        }
			    }
			}
		}

	};
	
	
	

}
#endif


