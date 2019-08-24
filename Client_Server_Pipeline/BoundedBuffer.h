#ifndef BoundedBuffer_h
#define BoundedBuffer_h

#include <stdio.h>
#include <queue>
#include <string>
#include <pthread.h>
#include <vector>

using namespace std;

class BoundedBuffer
{
	private:
		queue<vector<char>> q;
		int cap;
		int push_cert;
		int pop_cert;
		int bb_size;
		pthread_mutex_t push_mtx;
		pthread_mutex_t pop_mtx;
		pthread_mutex_t m;
		
		pthread_cond_t overflow;
		pthread_cond_t underflow;

	public:
		BoundedBuffer(int _cap)
		{
			pthread_mutex_init(&m, NULL);
			pthread_cond_init(&overflow,NULL);
			pthread_cond_init(&underflow, NULL);
			cap = _cap;
			pop_cert = 0;
			push_cert = _cap;
			
		}
		~BoundedBuffer()
		{
			pthread_mutex_destroy(&m);
			pthread_mutex_destroy(&pop_mtx);
			pthread_mutex_destroy(&push_mtx);
			pthread_cond_destroy(&overflow);
			pthread_cond_destroy(&underflow);
		}

		void push(char* data, int len)
		{
			pthread_mutex_lock(&push_mtx);
			vector<char> v(data, data+len);
			
			
			//push_cert--;
			while(q.size() == cap)
				pthread_cond_wait(&overflow, &push_mtx);
			
			//pthread_mutex_unlock(&push_mtx);
			//pthread_mutex_lock(&m);
			q.push(v);
			//pthread_mutex_unlock(&m);
			//pthread_mutex_lock(&pop_mtx);
			//pop_cert++;
			// underflow
			//if(pop_cert <= 0)
			pthread_cond_signal(&underflow);
			
			pthread_mutex_unlock(&push_mtx);
			
			/*
			vector<char> v(data, data+len);
			pthread_mutex_lock(&push_mtx);
			push_cert--;
			if(q.size() == cap)
				pthread_cond_wait(&overflow, &push_mtx);
			pthread_mutex_unlock(&push_mtx);
			pthread_mutex_lock(&m);
			q.push(v);
			pthread_mutex_unlock(&m);
			pthread_mutex_lock(&pop_mtx);
			pop_cert++;
			// underflow
			if(pop_cert <= 0)
				pthread_cond_signal(&underflow);
			pthread_mutex_unlock(&pop_mtx);
			*/
		}

		vector<char> pop()
		{
			
			pthread_mutex_lock(&pop_mtx);
			vector<char> t;
				
			//pop_cert--;
			
			while(q.size() == 0)
				pthread_cond_wait(&underflow, &pop_mtx);
			//pthread_mutex_unlock(&pop_mtx);
			//pthread_mutex_unlock(&m);
			t = q.front();
			q.pop();
			//pthread_mutex_unlock(&m);
			//pthread_mutex_lock(&push_mtx);
			//push_cert++;
			//overflow
			//if(push_cert <= 0)
			pthread_cond_signal(&overflow);
			pthread_mutex_unlock(&pop_mtx);
			//*/
			
			return t;
		}
};

#endif /* BoundedBuffer_ */
