#include <sys/wait.h>
#include <unistd.h>
// HERE
O_RDONLY
WIFEXITED(status) -> ((status & 0x7f) == 0)

WIFSIGNALED(status) -> ((((status & 0x7f) + 1) >> 1) > 0)

WIFSTOPPED(status) -> ((status & 0xff) == 0x7f)

WEXITSTATUS(status) -> ((status & 0xff00) >> 8)

WTERMSIG(status) -> (status & 0x7f)

WCOREDUMP(status) -> ((status) & 0x80)

WSTOPSIG(status) -> ((status & 0xff00) >> 8)

