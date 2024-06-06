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
