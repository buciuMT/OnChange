#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

void get_status(int count, char **files, long long int res[]) {
  struct stat s;
  for (int i = 0; i < count; i++) {
    stat(files[i], &s);
    res[i] = s.st_mtim.tv_sec;
  }
}

int main(int argc, char **argv) {
  int cmd_start = argc + 1, files_len = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp("--", argv[i])) {
      cmd_start = i;
      break;
    }
    files_len += strlen(argv[i]) + 1;
  }

  int cmd_replace = argc + 1, cmd_len = 0, cmd_ptr = 0;

  for (int i = cmd_start + 1; i < argc; i++) {
    if (!strcmp("!", argv[i])) {
      cmd_len += files_len + 1;
      continue;
    }
    cmd_len += strlen(argv[i]) + 1;
  }

  char cmd_buffer[cmd_len];

  for (int i = cmd_start + 1; i < argc; i++) {
    int len = strlen(argv[i]);
    if (len == 1 && !strcmp("!", argv[i])) {
      for (int j = 1; j < cmd_start; j++) {
        len = strlen(argv[j]);
        memcpy(cmd_buffer + cmd_ptr, argv[j], len);
        cmd_buffer[cmd_ptr += len] = ' ';
        cmd_ptr++;
      }
      continue;
    }

    memcpy(cmd_buffer + cmd_ptr, argv[i], len);
    cmd_buffer[cmd_ptr += len] = ' ';
    cmd_ptr++;
  }

  cmd_buffer[cmd_ptr] = '\0';

  long long int tbuf1[argc - cmd_start], tbuf2[argc - cmd_start];
  long long int *last = tbuf1, *current = tbuf2;

  char **files = argv + 1;
  int file_count = cmd_start - 1;

  get_status(file_count, files, last);

  for (;;) {
    get_status(file_count, files, current);
    for (int i = 0; i < file_count; i++)
      if (current[i] > last[i]) {
        printf("\n[UPDATE]: `%s`\n\n", cmd_buffer);
        system(cmd_buffer);
        get_status(file_count, files, last);
      }
  }
  return 0;
}
