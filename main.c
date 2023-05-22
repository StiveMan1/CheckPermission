#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>


int exist_user_id = 0;
int exist_group_id = 0;
unsigned int user_id;
unsigned int group_id;

/*
 * Function for parsing arguments of program
 */
const char *get_args(int argc, char *argv[]) {
    const char *path_;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            struct passwd *user_data = getpwnam(argv[++i]);
            user_id = user_data->pw_uid;
            exist_user_id = 1;
            if (!exist_group_id) {
                group_id = user_data->pw_gid;
                exist_group_id = 1;
            }
        } else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
            struct group *group_data = getgrnam(argv[++i]);
            group_id = group_data->gr_gid;
            exist_group_id = 1;
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
            path_ = argv[++i];
    }
    return path_;
}


/*
 * This function created for checking access to the file/dir
 * If it available to write inside current path file/dir it will print this path into the console
 */
void get_file_stats(const char *path) {
    if (path == NULL) return; // Skip Nullptr dir path
    if (memcmp(path, "/proc", 5) == 0 || memcmp(path, "/sys", 4) == 0) return; // Skip file/dir starting with /proc and /sys"
    struct stat file_stats;
    if (stat(path, &file_stats) < 0) return;

    // Check access to write into the file/dir in current path
    if ((file_stats.st_mode & S_IXGRP) && (file_stats.st_mode & S_ISVTX)) return;
    if ((file_stats.st_mode & S_IWOTH) ||                                                                               // Access for other to write
        (exist_user_id && (file_stats.st_mode & S_IWUSR) && file_stats.st_uid == user_id) ||                            // Access for user  to write
        (exist_group_id && (file_stats.st_mode & S_IWGRP) && file_stats.st_gid == group_id)) {                          // Access for group to write
        if ((file_stats.st_mode & S_IFMT) == S_IFREG)       // Print file full path
            printf("f %s\n", path);
        else if ((file_stats.st_mode & S_IFMT) == S_IFDIR)  // Print dir  full path
            printf("d %s\n", path);
    }

    if ((file_stats.st_mode & S_IFMT) == S_IFDIR) {         // Working with dir
        if ((file_stats.st_mode & S_IROTH) ||                                                                           // Access for other to read dir
            (exist_user_id && (file_stats.st_mode & S_IRUSR) == S_IRUSR && file_stats.st_uid == user_id) ||             // Access for user  to read dir
            (exist_group_id && (file_stats.st_mode & S_IRGRP) == S_IRGRP && file_stats.st_gid == group_id))             // Access for group to read dir
            get_files(path);
    }
}

/*
 * Recursive function for check recursively access for files and dirs
 */
void get_files(const char *dir_path) {
    if (dir_path == NULL) return; // Skip Nullptr dir path

    DIR *d = opendir(dir_path);
    if (d == NULL) return;

    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue; // Skip dir entities linking for current dir and priveous one

        char path_buf[strlen(dir->d_name) + strlen(dir_path) + 2]; // Creating buffer for full path for dir entity
        sprintf(path_buf, "%s/%s", dir_path, dir->d_name);

        get_file_stats(path_buf); // Checking access for new dir and calling recursion
    }

    closedir(d);
}

int main(int argc, char *argv[]) {
    const char *path_ = get_args(argc - 1, argv + 1);
    get_file_stats(path_);
    return 0;
}
