// Library for interacting with tiff files

enum TagId {
	ImageWidth = 256,
	ImageLength = 257,
	BitsPerSample = 258,
	Compression = 259,
	PhotometricInterpretation = 262,
	ImageDescription = 270,
	StripOffsets = 273,
	Orientation = 274,
	SamplesPerPixel = 277,
	RowsPerStrip = 278,
	StripByteCounts = 279,
	XResolution = 282,
	YResolution = 283,
	PlanarConfiguration = 284,
	ResolutionUnit = 296
};

char *get_tag_name(int number) {
	switch (number) {
		case ImageWidth:
			return "Image Width";
		case ImageLength:
			return "Image Length";
		case Compression:
			return "Compression";
		case PhotometricInterpretation:
			return "Photometric Interpretation";
		case StripOffsets:
			return "Strip Offsets";
		case RowsPerStrip:
			return "Rows Per Strip";
		case StripByteCounts:
			return "Strip Byte Counts";
		case XResolution:
			return "XResolution";
		case YResolution:
			return "YResolution";
		case ResolutionUnit:
			return "Resolution Unit";
		case BitsPerSample:
			return "Bits Per Sample";
		case ImageDescription:
			return "Image Description";
		case PlanarConfiguration:
			return "Planar Configuration";
		case Orientation:
			return "Orientation";
		case SamplesPerPixel:
			return "Samples Per Pixel";
		default:
			return "Unknown";
	}
}

enum CompressionSchemes {
	NO_COMPRESSION = 1,
	MODIFIED_HUFFMAN = 2,
	PACKBITS = 32773
};

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
struct TifDirectory { short entry_count; };

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
  char * pixels;
};
