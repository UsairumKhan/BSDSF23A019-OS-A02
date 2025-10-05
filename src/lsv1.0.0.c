/*
* Programming Assignment 02: lsv1.2.0
* Feature 3: Detailed listing (-l)
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

void do_ls(const char *dir, int detailed);
void print_details(const char *path, const struct stat *info);

int main(int argc, char const *argv[])
{
    int detailed = 0;
    const char *path = ".";

    // Check for -l flag
    if (argc > 1 && strcmp(argv[1], "-l") == 0)
    {
        detailed = 1;
        if (argc > 2)
            path = argv[2];
    }
    else if (argc > 1)
    {
        path = argv[1];
    }

    printf("Directory listing of %s:\n", path);
    do_ls(path, detailed);
    return 0;
}

void do_ls(const char *dir, int detailed)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        if (!detailed)
        {
            printf("%s\n", entry->d_name);
        }
        else
        {
            char fullpath[1024];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, entry->d_name);
            struct stat info;

            // Use lstat() instead of stat()
            if (lstat(fullpath, &info) == 0)
                print_details(entry->d_name, &info);
            else
                perror("lstat");
        }
    }

    if (errno != 0)
        perror("readdir failed");

    closedir(dp);
}

void print_details(const char *name, const struct stat *info)
{
    // File type
    char type = '-';
    if (S_ISDIR(info->st_mode)) type = 'd';
    else if (S_ISLNK(info->st_mode)) type = 'l';

    // Permissions
    char perm[10];
    snprintf(perm, sizeof(perm), "%c%c%c%c%c%c%c%c%c",
             (info->st_mode & S_IRUSR) ? 'r' : '-',
             (info->st_mode & S_IWUSR) ? 'w' : '-',
             (info->st_mode & S_IXUSR) ? 'x' : '-',
             (info->st_mode & S_IRGRP) ? 'r' : '-',
             (info->st_mode & S_IWGRP) ? 'w' : '-',
             (info->st_mode & S_IXGRP) ? 'x' : '-',
             (info->st_mode & S_IROTH) ? 'r' : '-',
             (info->st_mode & S_IWOTH) ? 'w' : '-',
             (info->st_mode & S_IXOTH) ? 'x' : '-');

    printf("%c%s %5ld %s\n", type, perm, (long)info->st_size, name);
}
