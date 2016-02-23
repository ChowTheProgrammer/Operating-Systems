/* Maximum number of pipes that the kernel provides */

#ifndef NPIPE
#define NPIPE       10
#endif

#define PIPESIZE    256

#define PIPE_FREE     0   /* Pipe table entry is available */
#define PIPE_USED     1   /* Pipe table entry is in use */
#define PIPE_LINKED   2   /* Pipe has been linked with two processes */

struct pipent  {
    uint16  pipstate;
    pid32   pipparent;
    pid32   pipreader;
    pid32   pipwriter;
    byte    buffer[PIPESIZE];
    byte    pipmode;
};

extern  struct  pipent  piptab[];
extern  int32   pipcount;

#define isbadppid(x) ((int32)(x) < 0 || (x) >= NPIPE)
