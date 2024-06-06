#include "result.h"
#include "tiff.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

/*
 * The tiff header appears right at the start of the file.
 * The header gives us a byte order for the file, and a
 * magic number of 42 to indicate that this is indeed a
 * tiff file. An offset tells us where the first TifDirectory is
 */
struct TifHeader {
	short byte_order;
	short magic_number;
	int ifd_offset;
};

/*
*/
struct TifDirectory {
	short entry_count;
};

/*
 * A tiff tag is one attribute associated with an image layer
 * in the tiff file. This could be, for instance, the width
 * of the layer, or the compression scheme used
 */
struct TifTag {
	short tag_id;    /* The tag identifier  */
	short data_type; /* The scalar type of the data items  */
	int data_count;  /* The number of items in the tag data  */
	int data_offset; /* The byte offset to the data items  */
};

struct ImageData {
	int width;
	int height;
};

int render(struct ImageData * data) {
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
		return 1;
	}
	window = SDL_CreateWindow("tifftastic", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, data->width,
			data->height, SDL_WINDOW_SHOWN);
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
	struct TifTag *tags = malloc(sizeof(struct TifTag) * directory.entry_count);
	for (int i = 0; i < directory.entry_count; i++) {
		fread(&tag, sizeof(struct TifTag), 1, fptr);
		tags[i] = tag;
	}

	int width = 0;
	int height = 0;
	for (int i = 0; i < directory.entry_count; i++) {
		if (tags[i].tag_id == ImageWidth) {
			width = tags[i].data_offset;
		}

		if (tags[i].tag_id == ImageLength) {
			height = tags[i].data_offset;
		}

		printf("\t%s (%i) = %i\n", get_tag_name(tags[i].tag_id), tags[i].tag_id, tags[i].data_offset);

	}

	struct ImageData *data = malloc(sizeof(struct ImageData));
	data->width = width;
	data->height = height;
	fclose(fptr);
	free(tags);
	return SOME(data);
}

int main() {
	struct Result result = read_data();
	if (result.is_error) {
		printf("Could not read tiff file, nested error: %s\n", result.value);
	}
	render(result.value);
	free(result.value);
	return 0;
}
