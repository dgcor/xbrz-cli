//
//  xbrz.cpp
//  xBRZ
//
//  Created by T!T@N on 04.24.16.
//  Modified by Brian Reavis on 04.15.17.
//

#include <iostream>
#include "xbrzlib/xbrz.h"
#include <sstream>

#define cimg_use_png
#define cimg_use_jpeg
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

template <uint32_t N> inline
unsigned char getByte(uint32_t val) { return static_cast<unsigned char>((val >> (8 * N)) & 0xff); }

bool is_valid_scale(const char* scale) {
	bool output = false;
	if (0 == std::strcmp(scale, "2")) output = true;
	if (0 == std::strcmp(scale, "3")) output = true;
	if (0 == std::strcmp(scale, "4")) output = true;
	if (0 == std::strcmp(scale, "5")) output = true;
	if (0 == std::strcmp(scale, "6")) output = true;
	return output;
}

int str_ends_with(const char * str, const char * suffix) {
	if (str == NULL || suffix == NULL ) return 0;
	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);
	if (suffix_len > str_len) return 0;
	return 0 == strncmp(str + str_len - suffix_len, suffix, suffix_len);
}

int main(int argc, const char * argv[]) {
	if (argc < 4) {
		std::cerr << "Usage: xbrz [input] [scale] [output] [-quality 0-100]" << std::endl;
		std::cerr << "The input is assumed to be in the Grayscale, RGB or RGBA color space" << std::endl;
		return EXIT_FAILURE;
	}
	const char* filename = argv[1];
	const char* requestedScale = argv[2];
	const char* outname = argv[3];

	int quality = 100;
	if (argc == 6) {
		quality = std::stoi(argv[5]);
		if (quality < 0 || quality > 100) {
			std::cerr << "Quality must be between 0 and 100" << std::endl;
			return EXIT_FAILURE;
		}
	}

	if (!is_valid_scale(requestedScale)) {
		std::cerr << "Scale must be 2, 3, 4, 5 or 6" << std::endl;
		return EXIT_FAILURE;
	}

	char* err;
	int scale = (int)strtol(requestedScale, &err, 10);
	if (!(!*err)) {
		std::cerr << "Scale factor " << err << " could not be parsed" << std::endl;
		return EXIT_FAILURE;
	}

	CImg<uint32_t> image(filename);
	int width = image.width();
	int height = image.height();
	int depth = image.depth();
	int spectrum = image.spectrum();
	bool isRGBA = spectrum == 4;

	uint32_t* p_raw = new uint32_t[width * height];
	// RGBA to ARGB
	for(uint32_t y = 0; y < (uint32_t)width; y++) {
		for(uint32_t x = 0; x < (uint32_t)height; x++) {
			uint32_t r = image(y, x, 0, 0);
			uint32_t g = image(y, x, 0, 1);
			uint32_t b = image(y, x, 0, 2);
			uint32_t a = 0;
			if (isRGBA) a = image(y, x, 0, 3);
			p_raw[x*width + y] = (a << 24) | (r << 16) | (g << 8) | b;
		}
	}

	uint32_t* p_output = new uint32_t[scale * scale * height * width];

	xbrz::scale(
				scale,
				p_raw,
				p_output,
				width,
				height,
				isRGBA? xbrz::ColorFormat::ARGB : xbrz::ColorFormat::RGB
				);

	delete[] p_raw;

	CImg<uint32_t> output(width * scale, height * scale, depth, spectrum);
	// ARGB to RGBA
	for(uint32_t y = 0; y < (uint32_t)output.width(); y++) {
		for(uint32_t x = 0; x < (uint32_t)output.height(); x++) {
			uint32_t pixel = p_output[x * width * scale +  y];

			switch (spectrum) {
				case 1:
					output(y, x, 0) = getByte<2>(pixel);    /* R */
					break;
				case 2:
					output(y, x, 0) = getByte<2>(pixel);    /* R */
					output(y, x, 1) = getByte<1>(pixel);    /* G */
					break;
				case 3:
					output(y, x, 0) = getByte<2>(pixel);    /* R */
					output(y, x, 1) = getByte<1>(pixel);    /* G */
					output(y, x, 2) = getByte<0>(pixel);    /* B */
					break;
				case 4:
					output(y, x, 0) = getByte<2>(pixel);    /* R */
					output(y, x, 1) = getByte<1>(pixel);    /* G */
					output(y, x, 2) = getByte<0>(pixel);    /* B */
					output(y, x, 3) = getByte<3>(pixel);    /* A */
					break;
				default:
					break;
			}
		}
	}
	delete[] p_output;

	if (str_ends_with(outname, ".jpg") || str_ends_with(outname, ".jpeg")) {
		output.save_jpeg(outname, quality);
	} else {
		output.save(outname);
	}

	std::cout << "Wrote " << outname << std::endl;
	return EXIT_SUCCESS;
}
