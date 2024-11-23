#include "result.h"
#include "tiff.h"
#include "pack_bits.c"
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

int render(struct ImageData *data) {
  SDL_Window *window = NULL;
  SDL_Surface *screenSurface = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }
  window = SDL_CreateWindow("tifftastic", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, data->width, data->height,
                            SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }
  screenSurface = SDL_GetWindowSurface(window);
  
  
  SDL_FillRect(screenSurface, NULL,
               SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

  for (int i = 0; i < data->width * data->height; i++) {
    SDL_Rect rect = {i % data->width, i / data->width, 1, 1};
    SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, data->pixels[i], data->pixels[i], data->pixels[i]));
  }
  
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
  return 0;
}

struct Result read_data(char * filename) {
  FILE *fptr;
  struct TifHeader header;
  struct TifDirectory directory;
  struct TifTag tag;
  // we assume little-endian by default
  int order = -1;

  fptr = fopen("./example.tif", "rb");
  fread(&header, sizeof(struct TifHeader), 1, fptr);
  fseek(fptr, header.ifd_offset, 0);
  fread(&directory, sizeof(struct TifDirectory), 1, fptr);

  char *data_types[] = {"BYTE", "ASCII", "SHORT", "SHORT", "RATIONAL"};
  struct TifTag *tags = malloc(sizeof(struct TifTag) * directory.entry_count);
  for (int i = 0; i < directory.entry_count; i++) {
    fread(&tag, sizeof(struct TifTag), 1, fptr);
    tags[i] = tag;
  }

  int width = 0;
  int height = 0;
  int rows_per_strip = 0;
  int strip_count = 0;
  int * strip_offsets = 0;
  int * strip_bytes = 0;
  for (int i = 0; i < directory.entry_count; i++) {
    if (tags[i].tag_id == ImageWidth) {
      width = tags[i].data_offset;
    }

    if (tags[i].tag_id == ImageLength) {
      height = tags[i].data_offset;
    }

    if (tags[i].tag_id == RowsPerStrip) {
      rows_per_strip = tags[i].data_offset;
    } 

    if (tags[i].tag_id == StripOffsets) {
      strip_count = tags[i].data_count;
      fseek(fptr, tags[i].data_offset, 0);
      strip_offsets = malloc(sizeof(int) * strip_count);
      fread(strip_offsets, sizeof(int), strip_count , fptr);
    }

    if (tags[i].tag_id == StripByteCounts) {
      strip_count = tags[i].data_count;
      fseek(fptr, tags[i].data_offset, 0);
      strip_bytes = malloc(sizeof(int) * strip_count);
      fread(strip_bytes, sizeof(int), strip_count , fptr);
    }
  }
  
  int total = 0;
  for (int i = 0; i < strip_count; i++) {
    fseek(fptr, strip_offsets[i], 0);
    unsigned char * other = malloc(strip_bytes[i]);
    fread(other, sizeof(char), strip_bytes[i], fptr);
    total += expected_length(other, strip_bytes[i]);
    free(other);
  }

  unsigned char * pixels = malloc(total);
  int total_pointer = 0;
  // First read the packed bits into a buffer, then expand
  for (int i = 0; i < strip_count; i++) {
    fseek(fptr, strip_offsets[i], 0);
    unsigned char * other = malloc(strip_bytes[i]);
    fread(other, sizeof(char), strip_bytes[i], fptr);
    size_t expected = expected_length(other, strip_bytes[i]);
    unsigned char * unpacked = unpack(other, strip_bytes[i], expected);
    for (int j = 0; j < expected; j++) {
      pixels[total_pointer] = unpacked[j];
      total_pointer += 1;
    }
    free(other);
    free(unpacked);
  }
  
  struct ImageData *data = malloc(sizeof(struct ImageData));
  data->width = width;
  data->height = height;
  data->pixels = pixels;

  fclose(fptr);
  free(tags);
  free(strip_offsets);
  free(strip_bytes);
  return SOME(data);
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    printf("Please provide a file path to open\n");
    return 0;
  }
  struct Result result = read_data(argv[1]);
  if (result.is_error) {
    printf("Could not read tiff file, nested error: %s\n", result.value);
  }
  render(result.value);
  free(result.value);
  return 0;
}
