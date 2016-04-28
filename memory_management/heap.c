/*
 * heap.c
 *
 *  Created on: 27-Apr-2016
 *      Author: abhiarora
 */
#include <stddef.h>
#include <stdbool.h>

#define MAGIC_NUMBER 0x1234

struct pool_t {
	size_t size;
	struct pool_t *next;
	int magic_number;
};

static struct pool_t *_free = NULL;
static size_t _size=0;
static bool _init=false;

int mem_manage_init(unsigned char *pool, size_t size)
{
	if (!pool)
		return -1;

	if (_init)
		return -1;

	_size=size;
	_free=(struct pool_t *)pool;
	_free->size = size - sizeof(struct pool_t);
	_free->next=NULL;
	_free->magic_number=MAGIC_NUMBER;

	_init=true;
	return 0;
}

void *alloc(size_t size)
{
	struct pool_t *ptr;
	struct pool_t *best=NULL;
	struct pool_t *pBest=NULL;

	size = (size + sizeof(struct pool_t) - 1) / sizeof(struct pool_t);
	size *=  sizeof(struct pool_t);

	for (ptr=_free; ptr; ptr=ptr->next) {
		if (ptr->size < size)
			continue;
		if (!best)
			best=ptr;
		if (best->size <= ptr->size)
			continue;
		pBest=best;
		best=ptr;
	}
	if (!best)
		return NULL;

	if (best->size > (size + sizeof(struct pool_t))) {
		ptr = best + (best->size - (size+sizeof(struct pool_t)));
		best->size-=size+sizeof(struct pool_t);
		best=ptr;
	} else {
		pBest->next=best->next;

	}
	best->next=NULL;

	return (void *)(best++);
}

void *getMetaPtr(void *ptr)
{
	return (((struct pool_t *)ptr) - 1);
}

int release(void *ptr)
{
	struct pool_t *chunk = getMetaPtr(ptr);
	if (chunk->magic_number != MAGIC_NUMBER)
		return -1;
	struct pool_t *p, *prev;
	bool done=false;
	for (p=_free; p; prev=p, p=p->next) {
		if ((p+p->size) == chunk) {
			p->size += chunk->size;
			chunk=p;
			done=true;
			continue;
		}
		if ((chunk+chunk->size) == p) {
			prev->next = p->next;
			chunk->size+=p->size;
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

