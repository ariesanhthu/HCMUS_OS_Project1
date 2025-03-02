#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*
Hàm lấy tên tệp từ đường dẫn (path): duyệt từ cuối đến khi gặp '/'
  memmove: hàm sao chép nội dung, đảm bảo sao chép an toàn ngay cả khi bộ nhớ có vùng trùng lặp
  memset: hàm điền phẩn còn lại của mảng buf bằng ký tự ' '
*/
char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
  return buf;
}


void find(char* path, char* file_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if(strlen(path) + 1 + DIRSIZ + 1 > 512){
    fprintf(2, "find: path too long\n");
    return;
  }

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: path %s does not exist\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: unknown path %s\n", path);
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    if (st.type == T_FILE){
      if (strcmp(fmtname(buf), file_name) == 0) {
        printf("%s\n", buf);
      }
    } else if (st.type == T_DIR){
      if (strcmp(fmtname(buf), ".") != 0 && strcmp(fmtname(buf), "..") != 0) {
        int fd2 = open(buf, 0);
        find(buf, file_name);
        close(fd2);
      }
    }
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if(argc < 2 || argc > 4){
    printf("[ERROR] Cu phap: find [path] [expression]\n");
    exit(1);
  } else{
    find(argv[1], argv[2]);
    exit(0);
  }
}
