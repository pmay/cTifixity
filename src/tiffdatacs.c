/*
 * Copyright 2014 British Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * tiffdatacs.c
 *
 * Calculates the MD5 Hash for the image data portion of a TIFF file.
 *
 * Todo:
 *  - Auto detect whether to remove alpha or not (currently alpha is removed)
 *  - Checksum image data or other stuff (use CLI flags? Output multiple MD5s)
 *  - Make output file write optional (use CLI flags)
 *
 *  Created on: 14 Oct 2014
 *      Author: Peter May (Peter.May@bl.uk)
 */

#include <openssl/md5.h>
#include "tiffio.h"

int main(int argc, char* argv[])
{
//	FILE *f1;

	int bytes_per_pixel = 3;		/* 3 bytes/pixel = RGB, 4bytes/pixel=rgba */

	int n;
	MD5_CTX c;
	unsigned char out[MD5_DIGEST_LENGTH];

	MD5_Init(&c);

//	f1 = fopen("file.bin", "wb");


    TIFF* tif = TIFFOpen(argv[1], "r");
    if (tif) {
    	uint32 w, h;		/* image width and height */
    	size_t npixels;		/* # pixels in image */
    	uint32* raster;		/* rgba data */

    	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    	npixels = w * h;

    	raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    	if (raster != NULL) {
    		if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {
    			/* Strip off alpha */
    			size_t count = npixels;
    			unsigned char *src, *dst;
    			src = dst = (unsigned char *) raster;
    			while (count>0){
    				*(dst++) = *(src++);
    				*(dst++) = *(src++);
    				*(dst++) = *(src++);
    				src++;					/* ignore alpha */
    				count--;
    			}


    			//fwrite(raster, bytes_per_pixel, npixels, f1);

    			// Calculate the MD5
    			MD5_Update(&c, raster, (npixels*bytes_per_pixel));
    		}
    		_TIFFfree(raster);
    	}
    	TIFFClose(tif);

    	MD5_Final(out, &c);

    	/* Print out the digest */
    	for(n=0; n<MD5_DIGEST_LENGTH; n++){
			printf("%02x", out[n]);
		}
		printf("\n");
    }
//    fclose(f1);
    return 0;
}
