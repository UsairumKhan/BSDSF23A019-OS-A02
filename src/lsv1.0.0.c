/*
 * Programming Assignment 02: ls-v1.1.0
 * Adds -l (long listing) support to the base ls program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

extern int errno;

void do_ls(const char *dir, int long_format);
void print_long_format(const char *path, const char *filename);

int main(int argc, char *argv[])
{
    int opt;
    int long_format = 0;

    // Parse options
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
        case 'l':
            long_format = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l] [dir...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // If no directories specified, use "."
    if (optind == argc)
        do_ls(".", long_format);
    else {
        for (int i = optind; i < argc; i++) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i], long_format);
            puts("");
        }
    }

    return 0;
}

void do_ls(const char *dir, int long_format)
{
    DIR *dp;
    struct dirent *entry;
    dp = opendir(dir);
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        if (long_format)
            print_long_format(dir, entry->d_name);
        else
            printf("%s\n", entry->d_name);
    }

    if (errno != 0)
        perror("readdir failed");

    closedir(dp);
}

// Prints a file’s info similar to "ls -l"
void print_long_format(const char *path, const char *filename)
{
    char fullpath[1024];
    struct stat st;

    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, filename);

    if (lstat(fullpath, &st) == -1) {
        perror("stat");
        return;
    }

    // File type
    printf( (S_ISDIR(st.st_mode)) ? "d" :
            (S_ISLNK(st.st_mode)) ? "l" : "-");

    // Permissions
    printf( (st.st_mode & S_IRUSR) ? "r" : "-");
    printf( (st.st_mode & S_IWUSR) ? "w" : "-");
    printf( (st.st_mode & S_IXUSR) ? "x" : "-");
    printf( (st.st_mode & S_IRGRP) ? "r" : "-");
    printf( (st.st_mode & S_IWGRP) ? "w" : "-");
    printf( (st.st_mode & S_IXGRP) ? "x" : "-");
    printf( (st.st_mode & S_IROTH) ? "r" : "-");
    printf( (st.st_mode & S_IWOTH) ? "w" : "-");
    printf( (st.st_mode & S_IXOTH) ? "x" : "-");

    // Link count, owner, group, size, time
    struct passwd *pw = getpwuid(st.st_uid);
    struct group  *gr = getgrgid(st.st_gid);
    char timebuf[32];
    strncpy(timebuf, ctime(&st.st_mtime) + 4, 12);
    timebuf[12] = '\0';

    printf(" %2ld %-8s %-8s %8ld %s %s\n",
        (long)st.st_nlink,
        pw ? pw->pw_name : "unknown",
        gr ? gr->gr_name : "unknown",
        (long)st.st_size,
        timebuf,
        filename);
}
