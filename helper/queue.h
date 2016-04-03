#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <cstddef>
#include <string.h>

#define QUEUE_DEF(t,n,s) t __buf_##n[s]; \
	queue n = queue(__buf_##n, s);


template <class _elem_t>
class queue {
protected:
	_elem_t *data;
	int front;
	int rear;
	int qLen;

public:
	queue() = delete;
	queue(_elem_t *data, int len);
	virtual int enque(_elem_t *data);
	virtual int deque(_elem_t *data);
	virtual bool isEmpty();
	virtual size_t getUsedSpace();
	virtual size_t getFreeSpace();

};

template <class _elem_t>
queue<_elem_t>::queue(_elem_t *buf, int len)
{
	this->data = buf;
	this->front = this->rear = 0;
	this->qLen = len;
}

#include <cstddef>



template <class _elem_t>
int queue<_elem_t>::enque(_elem_t *enq)
{
	if (this->front == (this->qLen - 1))
		return -1;

	if (!enq)
		return -1;

	memcpy(this->data + this->front, enq, sizeof(_elem_t));

	this->front++;
	return 0;
}

template <class _elem_t>
int queue<_elem_t>::deque(_elem_t *deqData)
{
	if (this->front == this->rear) {
		this->front = this->rear = 0;
		return -1;
	}

	if (!deqData)
		return -1;

	memcpy(deqData, this->data + this->rear, sizeof(_elem_t));
	this->rear++;
	return 0;
}

template <class _elem_t>
bool queue<_elem_t>::isEmpty()
{
	if(this->front != this->rear)
		return false;

	this->front = this->rear = 0;
	return true;
}

template <class _elem_t>
size_t queue<_elem_t>::getUsedSpace()
{
	return (this->front - this->rear);

}

template <class _elem_t>
size_t queue<_elem_t>::getFreeSpace()
{
	return (this->qLen - this->front);
}


#endif /* HELPERS_QUEUE_H_ */
