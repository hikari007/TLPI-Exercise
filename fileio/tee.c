#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

static char buf[1024];

int 
main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s [-a] file", argv[0]);

    bool hasAppendOpt = false;
    if (argc == 3 && getopt(argc, argv, ":a:") != -1)
        hasAppendOpt = true;

    int fd;
    int openFlags = O_CREAT | O_WRONLY;
    int filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    if (hasAppendOpt)
        fd = open(argv[2], openFlags | O_APPEND, filePerms);
    else
        fd = open(argv[1], openFlags | O_TRUNC, filePerms);

    if (fd == -1)
        errExit("open file failed");

    ssize_t sz;
    while ((sz = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (write(fd, buf, sz) != sz)
            fatal("write file");
        if (write(STDOUT_FILENO, buf, sz) != sz)
            fatal("write stdout");
    }

    if (sz == -1)
        errExit("read");
    if (close(fd) == -1)
        errExit("close");

    exit(EXIT_SUCCESS);
}