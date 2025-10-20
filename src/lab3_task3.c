#include <stdio.h>

// Name: Nisa Yakut
// Student ID: (buraya yaz)

// Function to calculate string length manually
int my_strlen(const char* str) {
  int length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}

// Function to copy string manually
void my_strcpy(char* dest, const char* src) {
  int i = 0;
  while (src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';  // null-terminate destination
}

int main(void) {
  char text[] = "hello";
  int len = my_strlen(text);
  printf("Length: %d\n", len);

  char buffer[100];
  my_strcpy(buffer, text);
  printf("Copied text: %s\n", buffer);

  return 0;
}
