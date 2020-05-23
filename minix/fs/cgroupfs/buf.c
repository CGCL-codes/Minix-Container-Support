/* Cgroupfs - buf.c - input/output buffer for read/write calls */

#include "head.h"

static char *buf;
static size_t left, used;
static off_t skip;

/*
 * Initialize the buffer for fresh use.  The output is to be stored into 'ptr'
 * which is BUF_SIZE bytes in size, since that is the size we requested.  Due
 * to the way vsnprintf works, we cannot use the last byte of this buffer.  The
 * first 'start' bytes of the produced output are to be skipped.  After that, a
 * total of 'len' bytes are requested.
 */
void buf_init(char * ptr, size_t len, off_t start)
{

	buf = ptr;
	skip = start;
	left = MIN(len, BUF_SIZE - 1);
	used = 0;
}


/*
 * Load data to the destination place
 */
void buf_write(char * data, char * ptr, size_t len, off_t start)
{
	// if (left == 0)
	// 	return;
	
	if(start > 0) {
		
		ptr += start;
	}

	memcpy(ptr, data, len);
} 


/*
 * Add arbitrary data to the end of the buffer.
 * data - data to be read
 * len - total length of data
 */
void buf_read(char * data, size_t len)
{

	if (left == 0)
		return;

	if (skip > 0) {
		if (skip >= (ssize_t)len) {
			skip -= len;

			return;
		}

		data += skip;
		len -= skip;
		skip = 0;
	}

	if (len > left)
		len = left;

	memcpy(&buf[used], data, len);

	used += len;
	left -= len;
}

/*
 * Return the resulting number of bytes produced, not counting the trailing
 * null character in the buffer.
 */
ssize_t buf_result(void)
{

	return used;
}