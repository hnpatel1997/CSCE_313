/* 
    File: my_allocator.cpp
*/
#include "BuddyAllocator.h"
#include <math.h>
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length)
{
	int new_bbs = 0;
	int new_tml = 0;
	block_size = _basic_block_size;
	total_memory = _total_memory_length;
	mem = new char[_total_memory_length];
	// Finding the power of the numbers with base 2, to find the levels needed
	new_bbs = pf2(_basic_block_size);
	new_tml = pf2(_total_memory_length);
	freelist_levels = new_tml - new_bbs + 1;
	// Creates a Blockheader of the biggest size and initialize it
	BlockHeader* header = (BlockHeader*) mem;
	header->block_size = _total_memory_length;
	header->is_used = false;
	// Give the vector its size and set the first level to point to the initialized blockheader 
	LinkedList* list = NULL;
	for(int i = 0; i < freelist_levels; i++)
	{
		list = new LinkedList();
		freelist.push_back(list);
	}
	total_free_memory = header->block_size;
	freelist[freelist_levels-1]->insert(header);
}

BuddyAllocator::~BuddyAllocator ()
{
	for (int i = 0; i < freelist.size(); i++)
   {
		LinkedList* list = freelist[i];
		delete list;
   }
	freelist.clear();
	delete mem;
}

char* BuddyAllocator::alloc(int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
	//Find the length required
	//cout << "Allocated!" << endl;
	int random = 0;
	int n_length = _length + sizeof(BlockHeader); // Actual size required
	int a = block_size;
	int x = 0;
	while(x == 0)
	{
		if(n_length <= a)
		{
			n_length = a;
			x = 1;
		}
		else
		{
			a = a*2;
		}
	}
	if(n_length > total_free_memory) // If required mem is more then memory available
	{
		//cout << "Error: Out of memory" << endl;
		return 0;
	}
	// Find the level required
	int n_length_pow = pf2(n_length) - pf2(block_size);
	if(n_length_pow < 0) // Required memory is greater than total memory
		return 0;
	int new_size = 0;
	// Return the size of the linkedlist for the block required
	new_size = freelist[n_length_pow]->get_size();
	if(new_size == 0) // If the level doesn't have the required block goes to bigger block
	{
		int temp_size = 0;
		int new_length = n_length_pow+1; // Go to next level
		while(random == 0)
		{
			// If the level above has a block
			temp_size = freelist[new_length]->get_size();
			if(temp_size > 0)
			{
				BlockHeader* block;
				block = freelist[new_length]->get_head();
				int temp_s = pf2(block->block_size) - pf2(n_length);
				if(temp_s > 0) // Splits the block and stores the not-used one(s) back to the freelist
				{
					random = 1; // Break the loop, we found a block
					while(block->block_size != n_length)
						split(block);
					freelist[n_length_pow]->remove(block);
					total_free_memory = total_free_memory - block->block_size;
					block->is_used = 1;
					return ((char*)block) + sizeof(BlockHeader);
				}
			}
			else
			{
				if(new_length == (freelist_levels-1))
				{
					cout << "Error: out of memory:" << endl;
					return 0;
				}
				else
					new_length++; // Go to next level
			}
		}
	}
	else
	{
		BlockHeader* block = NULL;
		block = freelist[n_length_pow]->get_head();
		freelist[n_length_pow]->remove(block);
		block->is_used = 1;
		total_free_memory = total_free_memory - block->block_size;
		return ((char*)block) + sizeof(BlockHeader);
	}
}

int BuddyAllocator::free(char* _a) {
	//cout << "Freed!" << endl;
	/* Same here! */
	int free_level = 0;
	int free_level_1 = 0;
	int temp_size = 0;
	int r = 0;
	char* free_block;
	free_block = _a - sizeof(BlockHeader);
	BlockHeader* header = (BlockHeader*) free_block;
	if (((header->block_size) % 128) != 0)
	{
		//cout << "Error: Memory Location" << endl;
		return 0;
	}
	free_level = pf2(header->block_size) - pf2(block_size);
	header->is_used = 0;
	free_level_1 = free_level;
	while(r == 0)
	{
		temp_size = freelist[free_level_1]->get_size();
		if(temp_size > 0)
		{
			BlockHeader* buddy_block;
			buddy_block = freelist[free_level_1]->get_head();
			if(arebuddies(header, buddy_block))
			{
				total_free_memory = total_free_memory - header->block_size;
				header = merge(header, buddy_block);
				free_level_1++;
			}
		}
		else
		{
			r = 1;
			freelist[free_level_1]->insert(header);
			total_free_memory = total_free_memory + header->block_size;
			temp_size = 0;
		}
	}
	return 0;
}

void BuddyAllocator::debug()
{
	cout << "Total Memory: " << total_memory << endl;
	cout << "Basic Memory: " << block_size << endl;
	cout << "FreeList Levels: " << freelist_levels << endl;
	cout << "Total Free Memory: " << total_free_memory << endl;
}

BlockHeader* BuddyAllocator::getbuddy (BlockHeader * addr)
{
	BlockHeader* buddyBlock = (BlockHeader*)((((char*)addr - mem) ^ (addr->block_size)) + mem);
	return buddyBlock;
}
	// given a block address, this function returns the address of its buddy 
	
bool BuddyAllocator::arebuddies (BlockHeader* block1, BlockHeader* block2)
{
	BlockHeader* buddy1 = NULL;
	BlockHeader* buddy2 = NULL;
	buddy1 = getbuddy(block1);
	buddy2 = getbuddy(block2);
	if((char*)block1 == (char*)buddy2) // See if getting its buddy is the same
	{
		if((char*)block2 == (char*)buddy1)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
	// checks whether the two blocks are buddies are not

BlockHeader* BuddyAllocator::merge (BlockHeader* block1, BlockHeader* block2)
{
	
	int new_level = 0;
	BlockHeader* header = NULL;
	if(block1->block_size == block2->block_size)
	{
		new_level = pf2(block1->block_size) - pf2(block_size);
		if((char*)block1 < (char*)block2) // If block1 is before
		{
			freelist[new_level]->remove(block2);
			block1->block_size = (block1->block_size) * 2;
			return block1;
		}
		else // If block2 is before
		{
			freelist[new_level]->remove(block2);
			block2->block_size = (block2->block_size) * 2;
			return block2;
		}
	}
}
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

BlockHeader* BuddyAllocator::split (BlockHeader* block)
{
	int new_level = 0;
	new_level = pf2(block->block_size) - pf2(block_size);
	freelist[new_level]->remove(block);
	BlockHeader* newBlock = (BlockHeader*)(((char*)block) + (block->block_size/2));
	block->block_size = (block->block_size/2);
	newBlock->block_size = block->block_size;
	freelist[new_level-1]->insert(newBlock);
	freelist[new_level-1]->insert(block);
	return block;
}
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected

int BuddyAllocator::pf2(int n) // Finds the log2 of given value
{
	int r = 0;
	while(n != 1)
	{
		n = n/2;
		r++;
	}
	return r;
}

