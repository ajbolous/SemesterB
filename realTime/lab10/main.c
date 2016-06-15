/* lab10.c - xmain, prntr */
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "bufpool.h"

#define NULL 0

struct	bpool bptab[NBPOOLS];		/* Buffer pool table		*/
int	nbpools;		/* current number of pools	*/

SYSCALL poolinit()
{
	nbpools = 0;
	return(OK);
} // poolinit()

int mkpool(int	bufsiz, int numbufs)
{
	int	ps;
	int	poolid;
	char	*where;


	bufsiz = (bufsiz + 1) & ~1;	/* round up		*/

	if (bufsiz<BPMINB ||
		bufsiz>BPMAXB ||
		numbufs<1 ||
		numbufs>BPMAXN ||
		nbpools >= NBPOOLS ||
		(where = malloc((bufsiz + sizeof(int))*numbufs)) == NULL)
	{
		printf("mkpool Error");
		return(SYSERR);
	}

	poolid = nbpools++;
	bptab[poolid].bpnext = (int *)where;
	bptab[poolid].bpsize = bufsiz;
	bufsiz += sizeof(int);
	for (numbufs--; numbufs > 0; numbufs--, where += bufsiz)
		*((char **)where) = bufsiz + (char *)where;
	*((int *)where) = (int)NULL;
	return(poolid);
} // mkpool
SYSCALL freebuf(char *buf)
{
	int	ps;
	int	poolid;
	int	*bf;
	void    **ptr;

	if (!buf)
		return(SYSERR);
	bf = (int *)buf;
	poolid = *(--bf);
	if (poolid < 0 || poolid >= nbpools
		) {
		return(SYSERR);
	}
	ptr = (void *)bf;
	*ptr = (void *)bptab[poolid].bpnext;
	bptab[poolid].bpnext = bf;
	return(OK);
} // freebuf

char *getbuf(int poolid)
{
	int	ps;
	int	*buf;
	void    **ptr;

	if (poolid < 0 || poolid >= nbpools)
		return(0);
	buf = bptab[poolid].bpnext;
	if (!buf)
		return(0);
	ptr = (void **)buf;
	bptab[poolid].bpnext = (int *)(*ptr);
	*buf++ = poolid;
	return((char *)buf);
}//getbuf
SYSCALL buffered_memory_init(unsigned int memory_size, int buffer_size, int max_parts)
{
	int poolid, numofbufs;
	if (memory_size % buffer_size)
		numofbufs = (memory_size / buffer_size) + 1;
	else
		numofbufs = memory_size / buffer_size;
	//(memory_size %buffer_size ? numofbufs = memory_size / buffer_size + 1 : numofbufs = memory_size / buffer_size);
	poolid = mkpool(buffer_size, numofbufs + max_parts);
	if (poolid == SYSERR)
		return SYSERR;
	printf("$$%d$$\n", poolid);
	bptab[poolid].max = max_parts; // we added this variable to the struct to know how mony parts we want to be in this pool
	return poolid;
}

void* get_buffered_memory(int nbytes, int poolid)
{
	int * controller;
	int i;

	if (bptab[poolid].max > 0)
		bptab[poolid].max--;
	else
	{
		printf("There is no enough memory to allocate in this pool\n");
		return 0;
	}
	controller = (int*)getbuf(poolid);
	controller[0] = poolid;
	controller[1] = (nbytes / bptab[poolid].bpsize);
	for (i = 2; i < controller[1] + 2; i++)
		controller[i] = (int*)getbuf(poolid);

	return controller;
}

void free_buffered_memory(void *ptr)
{
	int * tmp;
	int poolid, numofbufs, i;

	tmp = ptr;
	numofbufs = tmp[1];
	poolid = tmp[0];
	if (poolid<0 || poolid >NBPOOLS)
	{
		printf("Error , the poolid is illegal.\n");
		return;
	}
	bptab[poolid].max++;
	for (i = 2; i < numofbufs + 2; i++)
		freebuf((void *)tmp[i]);
	freebuf(ptr);
}

float get_buffered_float(void *ptr, int i)
{
	int* controller, *tmp;
	int buf, Offset, bufferSize;
	controller = ptr;
	bufferSize = bptab[controller[0]].bpsize;

	if ((i < 0) || (i > bufferSize * controller[1]))
	{
		printf("Illigal position input");
		exit(0);
	}
	buf = (i * 4) / bufferSize;
	Offset = ((i * 4) - bufferSize*buf) - 4;
	tmp = (int*)controller[buf + 2];
	return ((float)*(tmp + Offset));

}

void set_buffered_float(void *ptr, int i, float value)
{
	int* controller, *tmp;
	int buf, Offset, bufferSize;
	controller = ptr;
	bufferSize = bptab[controller[0]].bpsize;

	if ((i < 0) || (i > bufferSize * controller[1]))
	{
		printf("Illigal position input");
		return;
	}
	buf = (i * 4) / bufferSize;
	Offset = ((i * 4) - bufferSize*buf) - 4;
	tmp = (int*)controller[buf + 2];
	*(tmp + Offset) = value;
}



char output[2000];
int main()
{

	void *puffptrs[4];
	int poolid;
	char *sptr, *sptr2;
	void *floatbuffer;
	float x, sum;
	int i, n;
	char temp[20];

	strcpy(output, "");
	poolinit();
	poolid = buffered_memory_init(960, 64, 10);

	printf("poolid =%d\n", poolid);
	//getchar();
	if (poolid == SYSERR)
	{
		printf("Poolid == SYSERR\n");
		return -1;
	} // if 
	for (i = 0; i < 4; i++)
		puffptrs[i] = get_buffered_memory(100, poolid);

	free_buffered_memory(puffptrs[1]);
	floatbuffer = get_buffered_memory(200, poolid);

	n = 40;
	for (i = 0; i < n; i++)
	{
		x = i*10.0;
		set_buffered_float(floatbuffer, i, x);
	} // for

	//n=4;
	printf("Input in reverse order:\n");
	strcpy(output, "");
	for (i = n - 1; i > 0; i--)
	{
		x = get_buffered_float(floatbuffer, i);
		sprintf(temp, "  %5.2f ", x);
		strcat(output, temp);
	} // for

	printf(output);
	printf("\nPress any key to continue\n");
	getchar();

	strcpy(output, "");

	printf("\nPartial sums reverse order:\n");
	sum = 0;
	for (i = n - 1; i > 0; i--)
	{
		x = get_buffered_float(floatbuffer, i);
		sum += x;
		sprintf(temp, "  %5.2f ", sum);
		strcat(output, temp);

	} // for

	printf(output);
	printf("\nPress any key to continue\n");
	getchar();

	return 0;

} // xmain

