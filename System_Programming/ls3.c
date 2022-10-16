/*
  - 전체 파일의 개수 출력
  - 전체 파일의 크기의 합을 출력
  - "./ls3  -t  dir" : dir 디렉토리를 ls2형식으로 보여주되, 시간 순으로 정렬해서 출력
  - "./ls3  -b  dir" : dir 디렉토리를 ls2형식으로 보여주되, 파일의 크기 준으로 정렬해서 출력

* 제출물
  - 소스 코드를 파일, 실행화면 캡쳐한 이미지 파일을 하나로 압축해서 업로드
*/

#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DIRECTORY_SIZE MAXNAMLEN

int count_total = 0;
long size_total = 0;

void swap(struct stat *a, struct stat *b) {
  struct stat tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

void swapf(char *a, char *b) {
  char *tmp = (char *)malloc(sizeof(char) * 255);
  strcpy(tmp, a);
  strcpy(a, b);
  strcpy(b, tmp);
  free(tmp);
}

void sortStat_t(char **files, struct stat *stats, int *iopt, int n) {
  for (int i = 0; i < n - 1; i++) {
    int tmp = i;
    for (int j = i + 1; j < n; j++) {
      if (stats[tmp].st_mtime < stats[j].st_mtime) {
        tmp = j;
      }
    }
    if (i != tmp) {
      swap(&stats[i], &stats[tmp]);
      swapf(files[i], files[tmp]);
      int itmp;
      itmp = iopt[i];
      iopt[i] = iopt[tmp];
      iopt[tmp] = itmp;
    }
  }
}

void sortStat_b(char **files, struct stat *stats, int *iopt, int n) {
  for (int i = 0; i < n - 1; i++) {
    int tmp = i;
    for (int j = i + 1; j < n; j++) {
      if (stats[tmp].st_size < stats[j].st_size) {
        tmp = j;
      }
    }
    if (i != tmp) {
      swap(&stats[i], &stats[tmp]);
      swapf(files[i], files[tmp]);
      int itmp;
      itmp = iopt[i];
      iopt[i] = iopt[tmp];
      iopt[tmp] = itmp;
    }
  }
}

char fileType(mode_t mode) {
  if (S_ISREG(mode))
    return ('-');
  else if (S_ISDIR(mode))
    return ('d');
  else if (S_ISCHR(mode))
    return ('c');
  else if (S_ISBLK(mode))
    return ('b');
  else if (S_ISLNK(mode))
    return ('l');
  else if (S_ISFIFO(mode))
    return ('p');
  else if (S_ISSOCK(mode))
    return ('s');
}

char *mode_to_letters(mode_t mode) {
  static char str[10] = "----------";
  count_total ++;
  if (S_ISDIR(mode))
    str[0] = 'd';
  if (S_ISCHR(mode))
    str[0] = 'c';
  if (S_ISBLK(mode))
    str[0] = 'd';

  if (mode & S_IRUSR)
    str[1] = 'r';
  if (mode & S_IWUSR)
    str[2] = 'w';
  if (mode & S_IXUSR)
    str[3] = 'x';

  if (mode & S_IRGRP)
    str[4] = 'r';
  if (mode & S_IWGRP)
    str[5] = 'w';
  if (mode & S_IXGRP)
    str[6] = 'x';

  if (mode & S_IROTH)
    str[7] = 'r';
  if (mode & S_IWOTH)
    str[8] = 'w';
  if (mode & S_IXOTH)
    str[9] = 'x';
  return (str);
}

void printStat(char **file, struct stat *stats, int *iopt, bool *opt, int n,
               int total) {
  if (opt[1] == 1)
    //printf("total : %d\n", total / 2);
  for (int i = 0; i < n; i++) {
    struct stat tmp = stats[i];
    if (opt[1] == 1) {
      printf("%s ", mode_to_letters(tmp.st_mode));
      printf("%3ld ", tmp.st_nlink);
      printf("%s %s ", getpwuid(tmp.st_uid)->pw_name,
             getgrgid(tmp.st_gid)->gr_name);
      printf("%9ld ", tmp.st_size);
      printf("%.12s ", ctime(&tmp.st_mtime) + 4);
      size_total += tmp.st_size;
      
    }
    if (fileType(tmp.st_mode) == 'l') {
      char symfile[255] = {0};
      readlink(file[i], symfile, 255);
      printf("%s -> %s\n", file[i], symfile);
    } else {
      printf("%s\n", file[i]);
    }
  }
}

void ls(char *pathname, bool *opt) {
  struct dirent *entry;
  DIR *dirp;
  char filename[1024];
  struct stat stats[1024];
  char *files[1024];
  int iopt[1024];
  struct stat statbuf;
  int idx = 0;
  int total = 0;
  if ((dirp = opendir(pathname)) == NULL || chdir(pathname) == -1) {
    fprintf(stderr, "opendir or chdir error | path : %s", pathname);
    exit(1);
  }
  while ((entry = readdir(dirp)) != NULL) {
    if (entry->d_ino == 0)
      continue;
    strcpy(filename, entry->d_name);
    if (filename[0] == '.')
      continue;
    if (lstat(filename, &statbuf) == -1) {
      fprintf(stderr, "stat error\n");
      exit(1);
    }
    total += statbuf.st_blocks;
    files[idx] = (char *)malloc(sizeof(char) * 255);
    strcpy(files[idx], filename);
    stats[idx] = statbuf;
    iopt[idx] = entry->d_ino;
    idx++;
  }
  if (opt[0] == 1)
  {
    sortStat_b(files, stats, iopt, idx);
  }
  if (opt[2] == 1) {
    sortStat_t(files, stats, iopt, idx);
  }
  printStat(files, stats, iopt, opt, idx, total);
  closedir(dirp);
}

int main(int argc, char **argv) {
  
  char *pathname;
  pathname = malloc(1024);
  bool opt[3] = {0};
  struct stat statbuf;

  if (argc == 1) {
    if (getcwd(pathname, 1024) == NULL) {
      fprintf(stderr, "getcwd error\n");
      exit(1);
    }
    opt[1] = 1;
    ls(pathname, opt);

  } else if (argc == 2) {
    if (argv[1][0] == '-') {
      opt[1] = 1;
      for (int i = 1; i < strlen(argv[1]); i++) {
        char tmp = argv[1][i];
        if (tmp == 'b')
          opt[0] = 1;
        else if (tmp == 't')
          opt[2] = 1;
      }

      if (getcwd(pathname, 1024) == NULL) {
        fprintf(stderr, "getcwd error\n");
        exit(1);
      }
      ls(pathname, opt);
    } else {
      ls(pathname, opt);
    }
  } else {
    if (argv[1][0] == '-') {
      opt[1] = 1;
      for (int i = 1; i < strlen(argv[1]); i++) {
        char tmp = argv[1][i];
        if (tmp == 'b')
          opt[0] = 1;
        else if (tmp == 't')
          opt[2] = 1;
      }
      for (int i = 2; i < argc; i++) {
        pathname = argv[i];
        ls(pathname, opt);
      }
    } else {
      for (int i = 1; i < argc; i++) {
        ls(argv[i], opt);
      }
    }
  }
  printf("Total_File : %d\n", count_total);
  printf("Total_Size : %ld\n", size_total);
  return 0;
}
