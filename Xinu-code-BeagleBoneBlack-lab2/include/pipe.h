/* Maximum number of pipes that the kernel provides */

#ifndef NPIPE
#define NPIPE       10
#endif

#define PIPESIZE    256

#define PIPE_FREE     0   /* Pipe table entry is available */
#define PIPE_OPENED     1   /* Pipe table entry is in use */
#define PIPE_JOINED   2   /* Pipe has been linked with two processes */

#define END_VALID       -1

struct pipent  {
    uint16  pipstate;
    pid32   pipparent;
    pid32   pipreader;
    pid32   pipwriter;
    sid32   pipsem;
    byte    buffer[PIPESIZE];
    int32   pipbufs;
    int32   pipbufc;
    byte    pipmode;
};

extern  struct  pipent  piptab[];

#define isbadppid(x) ((int32)(x) < 0 || (x) >= NPIPE)
