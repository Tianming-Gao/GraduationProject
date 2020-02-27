#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <iostream>
using namespace std;

#define _DEF_LIMIT 10 //保证每次扩容的线程不超过十个
#define TIME_OUT 3

class ThreadPool {
	public:
		bool shutdown; //线程池开关
		uint32_t thread_min;//线程池最小线程数
		uint32_t thread_max;//线程池最大线程数
		uint32_t thread_alive;//活着的线程数
		uint32_t thread_busy;//忙线程数
		uint32_t thread_kill;//要杀死的线程数
	public:
		typedef struct {
			void* (*task)(void*);
			void* arg;
		}task_t;
		uint32_t queue_max;//任务队列容量
		uint32_t queue_head;//队列头
		uint32_t queue_tail;//队列尾
		uint32_t queue_cur;//当前位置
		task_t* task;//任务
		pthread_t* arr_pthread_tid;//线程id数组
		pthread_t manage_tid;//管理线程id
		pthread_cond_t not_empty;//不为空
		pthread_cond_t not_full;//不为满
		pthread_mutex_t lock_task;//用于任务的锁
    public:
        static ThreadPool* m_instance;
		static ThreadPool& GetInstance() {
			static ThreadPool m_instance;
            return m_instance;
		}
	private:
		ThreadPool();
	public:
		~ThreadPool();
	public:
		bool InitThreadPool(int max,int min,int queue_max);
		bool DestroyThreadPool();
		bool AddTask(void* (*task)(void*),void* arg);
		static void* ConsumeTask(void*);
		bool if_thread_alive(pthread_t pid);
		static void* ManagerThreadPool(void*);
};

#endif
