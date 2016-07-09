#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

/*

#define MAGIC_NUMBER 0x1234

struct pool_s {
	size_t size;
	struct pool_s *next;
	int magic_number; //A Magic Number is for Validing Memory Header
	uint8_t space[]; //Flexible Array Member
};

static struct pool_s *_free = NULL;
static size_t _size=0;
static bool _init=false;

int mem_manage_init(unsigned char *pool, size_t size)
{
	if (!pool)
		return -1;

	if (_init)
		return -1;

	_size=size;

	_free=(struct pool_s *)pool;
	_free->size = size - sizeof(struct pool_s);
	_free->next=NULL;
	_free->magic_number=MAGIC_NUMBER;

	_init=true;
	return 0;
}

#define ALLOC_BREAK_MARGIN (5*sizeof(struct pool_s))

void *alloc(size_t size)
{
	struct pool_s *ptr;
	struct pool_s *best = NULL;
	struct pool_s **pBest = &_free;

	size = (size + sizeof(struct pool_s) - 1) / sizeof(struct pool_s);
	size *=  sizeof(struct pool_s);

	for (ptr=_free; ptr; ptr=ptr->next) {
		if (ptr->size < size)
			continue;
		if (!best)
			best=ptr;
		if (best->size <= ptr->size)
			continue;
		*pBest=&(best->next);
		best=ptr;
	}

	if (!best)
		return NULL;

	size += sizeof(struct pool_s);
	if (best->size > (size + ALLOC_BREAK_MARGIN)) {
		ptr = best->space + (best->size - size);
		best->size -= size;
		best=ptr;
	} else {
		*pBest=best->next;
	}

	return (void *)(best->space);
}

void *getMetaPtr(void *ptr)
{
	return (((struct pool_s *)ptr) - 1);
}

int release(void *ptr)
{
	struct pool_s *chunk = getMetaPtr(ptr);
	if (chunk->magic_number != MAGIC_NUMBER)
		return -1;
	struct pool_s *p, *prev;
	bool done=false;
	for (p=_free; p; prev=p, p=p->next) {
		if ((p->space + p->size) == chunk) {
			p->size += chunk->size;
			chunk=p;
			done=true;
			continue;
		}
		if ((chunk->space + chunk->size) == p) {
			prev->next = p->next;
			chunk->size += p->size;
			done=true;
			continue;
		}
	}

	if (done)
		return 0;

	prev->next=chunk;
	chunk->next=NULL;
	return 0;
}
*/

