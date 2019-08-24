#include "common.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
#include "common.h"
#include "HistogramCollection.h"
#include "FIFOreqchannel.h"

#include <cstring>
using namespace std;


class patient_thread_args
{
	public:
	   /* put all the arguments for the patient threads */
		int n;
		BoundedBuffer* bb;
		int patient;
};

class worker_thread_args
{
	public:
	   /* put all the arguments for the worker threads */
	   BoundedBuffer* bb;
	   FIFORequestChannel* ch;
	   HistogramCollection* histogram;
	   pthread_mutex_t* hist_mtx;
};

void * patient_thread_function(void *arg)
{
    /* What will the patient threads do? */
	cout << "......DATA POINTS REQUEST......" << endl;
	double time = 0;
	patient_thread_args *patient_thread;
	patient_thread = (patient_thread_args*) arg;
	
	for(int i = 0; i < patient_thread->n; i++)
	{
		datamsg msg1(patient_thread->patient,time,1);
		//cout << "Patient: " << patient_thread->patient << endl;
		patient_thread->bb->push((char*)& msg1,sizeof(datamsg));
		time = time + 0.004;
		//cout << "Patient Function" << endl;
	}
	pthread_exit(NULL);
}

void *worker_thread_function(void *arg)
{
    /* Functionality of the worker threads */
    worker_thread_args* worker_thread;
	worker_thread = (worker_thread_args*) arg;
	while(true)
	{
		MESSAGE_TYPE quit = QUIT_MSG;
		vector<char> v = worker_thread->bb->pop();
		char* c_data = v.data();
		datamsg* data = (datamsg*) c_data;
		//cout << "Worker Function" << endl;
		worker_thread->ch->cwrite(v.data(), v.size());
		if(v.size() == sizeof(MESSAGE_TYPE) && v.at(0) == quit)
		{
			//cout << "Quit in Worker" << endl;
			delete worker_thread->ch;
			break;
		}
		else
		{
			int person = data->person;
			char* response = worker_thread->ch->cread();
			double ecg_num = *((double*) response);
			//cout << "Person:" << person << endl;
			//cout << "ECG value:" << ecg_num << endl;
			pthread_mutex_lock(worker_thread->hist_mtx);
			worker_thread->histogram->update(data->person, ecg_num);
			pthread_mutex_unlock(worker_thread->hist_mtx);
			//cout << "Size:" << worker_thread->histogram->size();
		}
	}
	pthread_exit(NULL);
}

/*





*/

class binary_file_patient
{
	public:
		int m;
		string n;
		string f;
		BoundedBuffer* bb;
		FIFORequestChannel* ch;
};

class binary_file_worker
{
	public:
		int m;
		string f;
		BoundedBuffer* bb;
		FIFORequestChannel* ch;
};

void * binary_patient(void *arg)
{
	cout << "......FILE REQUEST......" << endl;
    // What will the patient threads do?
	binary_file_patient *patient_binary;
	patient_binary = (binary_file_patient*) arg;
	
	string name = "1.csv";
	
	char* buffer1;
	filemsg fmsg(0,0);
	char* buffer = new char[sizeof(filemsg)+name.length()+1];
	*(filemsg*) buffer = fmsg;
	strcpy(buffer+sizeof(filemsg),name.c_str());
	/*patient_binary->ch->cwrite(buffer, sizeof(filemsg)+name.length()+1);
	__int64_t length = *(__int64_t*) patient_binary->ch->cread();
	__int64_t max = patient_binary->m;
	while(length > 0)
	{
		__int64_t mini = min(max,length);
		length -= mini;
		((filemsg*)buffer)->length = mini;
		patient_binary->ch->cwrite(data, sizeof(filemsg)+name.length()+1);
		((filemsg*)buffer)->offset += mini;
		buffer1 = worker_binary->ch->cread();
		patient_binary->bb->push(buffer1, mini);
	}*/
	
	//patient_binary->bb->push(buffer, sizeof(filemsg)+name.length()+1);
	
	
	pthread_exit(NULL);
}

void *binary_worker(void *arg)
{
	/*binary_file_worker* worker_binary;
	worker_binary = (binary_file_worker*) arg;
	ofstream file;
	file.open(worker_binary->f);
	while(true)
	{
		vector<char> v = worker_binary->bb->pop();
		char* data = v.data();
		if(*v.data() == FILE_MSG)
		{
			
		}
	}*/
	
    /* Functionality of the worker threads */
	char* buffer1;
    binary_file_worker* worker_binary;
	worker_binary = (binary_file_worker*) arg;
	MESSAGE_TYPE quit = QUIT_MSG;
	string name = worker_binary->f;
	vector<char> v = worker_binary->bb->pop();
	char* data = v.data();
	filemsg* buffer = (filemsg*) v.data();
	//cout << "Length(Data):" << buffer->length << endl;
	worker_binary->ch->cwrite(data, sizeof(filemsg)+name.length()+1);
	__int64_t length = *(__int64_t*) worker_binary->ch->cread();
	//cout << "Length:" << length << endl;
	//cout << "Offset:" << data->offset << endl;
	__int64_t max = worker_binary->m;
	ofstream file;
	file.open(worker_binary->f);
	while(length > 0)
	{
		//cout << "Copying....." << endl;
		__int64_t mini = min(max,length); // Useful towards the end of the file
		length -= mini; // Keeps track of data still to transfer
		((filemsg*)data)->length = mini;
		worker_binary->ch->cwrite(data, sizeof(filemsg)+name.length()+1);
		((filemsg*)data)->offset += mini;
		buffer1 = worker_binary->ch->cread();
		file.write(buffer1,mini); // Write data to new file byte by byte (256)
	}
	file.close();
	
	pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
    int n = 0;    //default number of requests per "patient"
    int p = 2;     // number of patients [1,15]
    int w = 500;    //default number of worker threads
    int b = 50; 	// default capacity of the request buffer, you should change this default
	int m = MAX_MESSAGE; 	// default capacity of the file buffer
	string f = "out.csv";
    srand(time_t(NULL));
    
	int input_opt = 0;
	while((input_opt = getopt(argc, argv, "n:p:w:b:f:")) != -1)
	{
		switch (input_opt)
		{
			case 'n':
				n = atoi(optarg);
				break;
			case 'p':
				p = atoi(optarg);
				break;
			case 'w':
				w = atoi(optarg);
				break;
			case 'b':
				b = atoi(optarg);
				break;
			case 'f':
				f = atoi(optarg);
				break;
		}
	}
	
    int pid = fork();
    if (pid == 0)
	{
		// modify this to pass along m
        execl ("dataserver", "dataserver", (char *)NULL);
        
    }
    else
	{
		cout << "Creating FIFORequestChannel......" << endl;
		FIFORequestChannel* chan = new FIFORequestChannel("control", FIFORequestChannel::CLIENT_SIDE);
		BoundedBuffer request_buffer(b);
		HistogramCollection hc;
		pthread_mutex_t hist_mtx;
		pthread_mutex_init(&hist_mtx, NULL);
		
		
		
		struct timeval start, end;
		gettimeofday (&start, 0);
		
		if(n != 0)
		{
			/* Start all threads here */
			cout << "Start all threads here" << endl;
			pthread_t patient_threads[p];
			pthread_t worker_threads[w];
			patient_thread_args pt[p];
			worker_thread_args wt[w];
			
			
			
			// Patient Threads
			cout << "Start Patient threads here" << endl;
			for(int i = 0; i < p; i++)
			{
				pt[i].n = n;
				pt[i].patient = i + 1;
				pt[i].bb = &request_buffer;
				hc.add(new Histogram(10,-2,2));
				pthread_create(&patient_threads[i], NULL, patient_thread_function, (void*) &pt[i]);
				//wt[i].hist_mtx = hist_mtx;
				
			}
			
			// Worker Threads
			cout << "Start Worker threads here" << endl;
			for(int j = 0; j < w; ++j)
			{
				//hc.add(new Histogram(20,-3,3));
				wt[j].bb = &request_buffer;
				wt[j].histogram = &hc;
				wt[j].hist_mtx = &hist_mtx;
				MESSAGE_TYPE* new_channel_request = new MESSAGE_TYPE(NEWCHANNEL_MSG);
				chan->cwrite((char*) new_channel_request, sizeof(MESSAGE_TYPE));
				char* new_chan = chan->cread();
				wt[j].ch = new FIFORequestChannel(new_chan, FIFORequestChannel::CLIENT_SIDE);
				pthread_create(&worker_threads[j], NULL, worker_thread_function, (void*) &wt[j]);
			}
			
	
			// Join all threads here
			
			// Patient Join
			//cout << "Patient Join" << endl;
			for(int k = 0; k < p; ++k)
				pthread_join(patient_threads[k], NULL);
			
			// Quit
			//cout << "Quit" << endl;
			for(int l = 0; l < w; ++l)
			{
				//cout << "Main Quit" << endl;
				MESSAGE_TYPE quit = QUIT_MSG;
				request_buffer.push((char*) &quit, sizeof(MESSAGE_TYPE));
			}
			
			// Worker Join
			//cout << "Worker Join" << endl;
			for(int mu = 0; mu < w; ++mu)
				pthread_join(worker_threads[mu], NULL);
		}
		else
		{
			pthread_t patient_binary_file[p];
			pthread_t worker_binary_file[w];
			binary_file_patient pt[p];
			binary_file_worker wt[w];
			
			for(int i = 0; i < p; i++)
			{
				pt[i].n = "1";
				pt[i].m = m;
				pt[i].f = f;
				pt[i].bb = &request_buffer;
				pthread_create(&patient_binary_file[i], NULL, binary_patient, (void*) &pt[i]);
			}
			
			for(int j = 0; j < w; ++j)
			{
				wt[j].f = f;
				wt[j].m = m;
				wt[j].bb = &request_buffer;
				MESSAGE_TYPE* new_channel_request = new MESSAGE_TYPE(NEWCHANNEL_MSG);
				chan->cwrite((char*) new_channel_request, sizeof(MESSAGE_TYPE));
				char* new_chan = chan->cread();
				wt[j].ch = new FIFORequestChannel(new_chan, FIFORequestChannel::CLIENT_SIDE);
				pthread_create(&worker_binary_file[j], NULL, binary_worker, (void*) &wt[j]);
			}
			
			for(int k = 0; k < p; ++k)
				pthread_join(patient_binary_file[k], NULL);
			
			for(int l = 0; l < w; ++l)
			{
				MESSAGE_TYPE quit = QUIT_MSG;
				request_buffer.push((char*) &quit, sizeof(MESSAGE_TYPE));
			}
			
			for(int mu = 0; mu < w; ++mu)
				pthread_join(worker_binary_file[mu], NULL);
		}
		
		gettimeofday (&end, 0);
		hc.print ();
		int secs = (end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec)/(int) 1e6;
		int usecs = (int)(end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec)%((int) 1e6);
		cout << "Took " << secs << " seconds and " << usecs << " micor seconds" << endl;

		MESSAGE_TYPE q = QUIT_MSG;
		chan->cwrite ((char *) &q, sizeof (MESSAGE_TYPE));
		cout << "All Done!!!" << endl;
		delete chan;
	}
    
}
