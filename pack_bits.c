#include <stdio.h>
#include <stdlib.h>

size_t expected_length(unsigned char hexArray[], int current_length) {
  /*
   Compute the expected length of a resulting array after performing PackBits
   decompression. This is necessary in order to allocate a suffciently large
   buffer. PackBits is not efficient in all cases, and in some unlucky cases
   the compressed array can be larger than the decompressed array.
   */
  int i = 0;
  size_t expected_length = 0;
  while (i < current_length) {
    int hex = hexArray[i];
    if (hex > 128) {
      hex = 256 - hex;
      // Repeat the byte immediately after this one
      // 256 - hex times
      for (int j = 0; j <= hex; ++j) {
	expected_length += 1;
      }
      i++;
    }
    else if (hex < 128) {
      // Treat the next hex bytes as literal
      for (int j = 0; j <= hex; ++j) {
	expected_length += 1;
      }
      i += hex + 1;
    }
    i++;
  }
  return expected_length;
}

unsigned char * unpack(unsigned char hexArray[], int current_length, size_t expected_length) {
  /*
   Decompress a PackBits array
   */
  int i = 0;
  unsigned char * output = (unsigned char*) malloc(expected_length * sizeof(char));
  int output_ptr = 0;
  while (i < current_length) {
    int hex = hexArray[i];
    if (hex > 128) {
      hex = 256 - hex;
      // Repeat the byte immediately after this one
      // 256 - hex times
      for (int j = 0; j <= hex; j++) {
	output[output_ptr] = hexArray[i+1];
	output_ptr += 1;
      }
      i++;
    }
    else if (hex < 128) {
      // Treat the next hex bytes as literal
      for (int j = 0; j <= hex; j++) {
	output[output_ptr] = hexArray[i+j+1];
	output_ptr += 1;
      }
      i += hex + 1;
    }
    i++;
  }
  return output;
}
