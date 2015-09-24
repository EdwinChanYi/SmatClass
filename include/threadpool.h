/*
 * threadpool.h
 *
 *  Created on: Jul 28, 2015
 *      Author: miller
 */
#ifndef MYCODE_INCLUDE_THREADPOOL_H_
#define MYCODE_INCLUDE_THREADPOOL_H_
#include <vector>
#include <cassert>
#include "cpp11thread.h"
#include <memory>
#include <algorithm>
#include <chrono>
#include <ctime>
#include"main.h"
using namespace std;
using edwin::Cpp11Thread;
namespace edwin
{
	template <class T,class R>
	class ThreadPool
	{
	protected:

			int m_n_threads;
			//vector contains all the threads
			vector<shared_ptr<Cpp11Thread<T,R>>> m_vec_threads;
	public:
			ThreadPool(int nThreads):m_n_threads(nThreads)
			{
				assert(nThreads>0 && nThreads<=512);
				for (int i = 0; i< nThreads ;i++)
					m_vec_threads.push_back(shared_ptr<Cpp11Thread<T,R>>(new Cpp11Thread<T,R>()));
			}
			~ThreadPool()
			{

			}
			void terminate()
			{
				for_each(m_vec_threads.begin(),m_vec_threads.end(),[this](std::shared_ptr<Cpp11Thread<T,R>> & item)
			    {
					item->terminate();
			    });
			}
			void wait_for_idle()
			{
				int n_tasks = 0;
			    do
			    {
			    	chrono::duration<int,milli> milliseconds(200);
			    	if (n_tasks)
			    		this_thread::sleep_for(milliseconds);
			        n_tasks = 0;
			        for_each(m_vec_threads.begin(),m_vec_threads.end(),[this,&n_tasks](std::shared_ptr<Cpp11Thread<T,R>> & item)
			        {
			        	n_tasks += item->load();
			        });
			    }while (n_tasks);
			}
			//将线程加入调度
			void join()
			{
				for_each(m_vec_threads.begin(),m_vec_threads.end(),[this](std::shared_ptr<Cpp11Thread<T,R>> & item)
			    {
					item->terminate();
			        item->join();
			    });
			}
			//total threads;
			size_t count(){return m_vec_threads.size();}
			//int get_thread_tasks(int n) { return (n>=m_vec_threads.size())?0:m_vec_threads[n]->get_task_nums(); }
			void append(Task<T,R> task)
			{

				int nIdx = -1;
			    unsigned int nMinLoad = -1;
			    for (unsigned int i=0;i<m_n_threads;i++)
			    {
			    	if (nMinLoad> m_vec_threads[i]->load())
			        {
			    		nMinLoad = m_vec_threads[i]->load();
			            nIdx = i;
			         }
			    }
			    assert(nIdx>=0 && nIdx<m_n_threads);

			    m_vec_threads[nIdx]->append(task);
			}

			size_t load(int n)
			{
			    return (n>=m_vec_threads.size()&&n<0)?0:m_vec_threads[n]->load();
			}

	};

}

#endif /* MYCODE_INCLUDE_THREADPOOL_H_ */
