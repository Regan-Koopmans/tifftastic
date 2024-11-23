#include <stdio.h>
#include <stdlib.h>

int expected_length(unsigned char hexArray[], int current_length) {    
  int i = 0;
  int expected_size = 0;
  while (i < current_length) {
    int hex = hexArray[i];
    if (hex > 128) {
      hex = 256 - hex;
      // Repeat the byte immediately after this one
      // 256 - hex times
      for (int j = 0; j <= hex; j++) {
	expected_size += 1;
      }
      i++;
    }
    else if (hex < 128) {
      // Treat the next hex bytes as literal
      for (int j = 0; j <= hex; j++) {
	expected_size += 1;
      }
      i += hex + 1;
    }
    i++;
  }
  return expected_size;
}

unsigned char * unpack(unsigned char hexArray[], int current_length, int expected_legnth) {
  int i = 0;
  unsigned char * output = malloc(expected_length);
  int output_ptr = 0;
  int expected_size = 0;
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
