/* bufpool.h */

#define SYSCALL int
#define SYSERR -1
#define OK 1


#ifndef	NBPOOLS
#define	NBPOOLS	5			/* Maximum number of pools	*/
#endif

#ifndef	BPMAXB
#define	BPMAXB	512			/* Maximum buffer length	*/
#endif

#define	BPMINB	2			/* Minimum buffer length	*/

#ifndef	BPMAXN
#define	BPMAXN	100			/* Maximum buffers in any pool	*/
#endif

struct	bpool	{			/* Description of a single pool	*/
	int	bpsize;			/* size of buffers in this pool	*/
	int	*bpnext;		/* pointer to next free buffer	*/
	int max;		/* Max Parts, when = 0, cant allocate buffers any more	*/
};				/*  currently in THIS pool	*/


/*extern	char	*getbuf(int poolid);
extern	SYSCALL	freebuf(char *buf);
extern int mkpool(int  bufsiz, int numbufs);
extern SYSCALL poolinit();
extern SYSCALL buffered_memory_init(unsigned int memory_size, int block_size, int max_parts);
extern void *get_buffered_memory(int nbytes, int poolid);
extern void free_buffered_memory(void *ptr);
extern float get_buffered_float(void *ptr, int i);
extern void set_buffered_float(void *ptr, int i, float value);*/