// Library for interacting with tiff files

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

enum CompressionSchemes {
  NO_COMPRESSION = 1,
  MODIFIED_HUFFMAN = 2,
  PACKBITS = 32773
};
