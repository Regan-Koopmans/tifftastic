#include "result.h"
#include "tiff.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct TifHeader {
  char byte_order[2];
  char magic_number[2];
  int ifd_offset;
};

struct TifDirectory {
  short entry_count;
};

struct TifTag {
  short tag_id;    /* The tag identifier  */
  short data_type; /* The scalar type of the data items  */
  int data_count;  /* The number of items in the tag data  */
  int data_offset; /* The byte offset to the data items  */
};

int render() {
  SDL_Window *window = NULL;
  SDL_Surface *screenSurface = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }
  window = SDL_CreateWindow("tifftastic", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }
  screenSurface = SDL_GetWindowSurface(window);
  SDL_Rect rect = {0, 0, 1, 1};
  SDL_FillRect(screenSurface, NULL,
               SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
  SDL_UpdateWindowSurface(window);
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    SDL_WaitEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
      SDL_DestroyWindow(window);
      SDL_Quit();
      quit = true;
    }
  }
}

char *get_tag_name(int number) {
  switch (number) {
  case 256:
    return "Image Width";
  case 257:
    return "Image Length";
  case 259:
    return "Compression";
  case 262:
    return "Photometric Interpretation";
  case 273:
    return "Strip Offsets";
  case 278:
    return "Rows Per Strip";
  case 279:
    return "Strip Byte Counts";
  case 282:
    return "XResolution";
  case 283:
    return "YResolution";
  case 296:
    return "Resolution Unit";
  default:
    return "Unknown";
  }
}

struct Result read_data() {
  FILE *fptr;
  struct TifHeader header;
  struct TifDirectory directory;
  struct TifTag tag;
  // we assume little-endian by default
  int order = -1;

  fptr = fopen("./example.tif", "rb");
  fread(&header, sizeof(struct TifHeader), 1, fptr);
  printf("Seeking to %i\n", header.ifd_offset);
  fseek(fptr, header.ifd_offset, 0);
  fread(&directory, sizeof(struct TifDirectory), 1, fptr);

  char *data_types[] = {"BYTE", "ASCII", "SHORT", "SHORT", "RATIONAL"};
  for (int i = 0; i < directory.entry_count; i++) {
    fread(&tag, sizeof(struct TifTag), 1, fptr);
    printf("Tag ID: %s\n", get_tag_name(tag.tag_id));
    printf("Data Type: %s\n", data_types[tag.data_type]);
    printf("Value count: %i\n", tag.data_count);
    printf("Data Offset: %i\n", tag.data_offset);
  }
  fclose(fptr);
  return SOME(NULL);
}

int main() {
  struct Result data = read_data();
  if (data.is_error) {
    printf("Could not read tiff file, nested error: %s\n", data.value);
  }
  // render();
  return 0;
}
