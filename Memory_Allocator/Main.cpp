#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include "Ackerman.h"
#include "BuddyAllocator.h"

int main(int argc, char ** argv) {

	int bvalue = /*128*/; int svalue = /*1024*1024*128*/; // Input values
	int c = 0;
	// getopt
	while ((c = getopt(argc, argv, "b:s:")) != -1)
	switch (c)
	{
		case 'b':
			bvalue = strtol(optarg, NULL, 10);
			break;
		case 's':
			svalue = strtol(optarg, NULL, 10);
			break;
		default:
			bvalue = 128;
			svalue = 1024*5;
	}

  // create memory manager
  BuddyAllocator * allocator = new BuddyAllocator(bvalue, svalue);
  cout << endl;
  /*allocator->debug();
  cout << endl;
  char* a =  allocator->alloc(450);
  cout << endl;
  allocator->debug();
  cout << endl;
  allocator->free(a);
  cout << endl;*/
  allocator->debug();
  cout << endl;

  // test memory manager
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test. 
  
  // destroy memory manager
  delete allocator;
}