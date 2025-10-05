/*
* Programming Assignment 02: lsv1.0.0 (Feature 2)
* Added -a (show hidden) and -l (show file size)
* Usage Examples:
*       ./bin/ls
*       ./bin/ls -a
*       ./bin/ls -l
*       ./bin/ls -a -l /home
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

extern int errno;

// updated function prototype
void do_ls(const char *dir, int show_all, int long_list);

int main(int argc, char const *argv[])
{
    int show_all = 0;   // for -a
    int long_list = 0;  // for -l

    // check for -a and -l flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0)
            show_all = 1;
        else if (strcmp(argv[i], "-l") == 0)
            long_list = 1;
    }

    // if no directory is given, or only flags are used
    if (argc == 1 || (argc == 2 && (show_all || long_list)))
        do_ls(".", show_all, long_list);
    else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-')  // skip flags
                continue;
            printf("Directory listing of %s :\n", argv[i]);
            do_ls(argv[i], show_all, long_list);
            puts("");
        }
    }
    return 0;
}

void do_ls(const char *dir, int show_all, int long_list)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL) {
        fprintf(stderr, "Cannot open directory : %s\n", dir);
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL) {
        // skip hidden files unless -a is used
        if (!show_all && entry->d_name[0] == '.')
            continue;

        if (long_list) {
            struct stat fileStat;
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
            if (stat(path, &fileStat) == 0)
                printf("%10ld  %s\n", fileStat.st_size, entry->d_name);
            else
                printf("%s\n", entry->d_name);
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    if (errno != 0)
        perror("readdir failed");

    closedir(dp);
}
