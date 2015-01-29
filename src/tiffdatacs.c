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
 *  - modularise code (checksumming function)
 *  - Auto detect whether to remove alpha or not (currently alpha is removed)
 *  - Checksum image data or other stuff (use CLI flags? Output multiple MD5s)
 *  - Make output file write optional (use CLI flags)
 *
 *  Created on: 14 Oct 2014
 *      Author: Peter May (Peter.May@bl.uk)
 */

#include <argp.h>
#include <openssl/md5.h>
#include "tiffio.h"
#include "tifixityConfig.h"

const char* argp_program_version = VERSION;
const char* argp_program_bug_address = ISSUES_ADDRESS;

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char* args[1];                /* Args: TIFF */
  char* extract;					/* Extract raster data to binary file */
  /*int verbose;*/					/* The -v flag */
};

/* Argp Options */
static struct argp_option options[] = {
  /* Name, key, arg, flags, doc */
  {"extract", 'e', "OUTFILE", 0, "Extract raster data to OUTFILE"},
  //{"verbose", 0, 0, 0, "Produce verbose output" },
  { 0 }
};

/* Argp Parser */
static error_t parse_opt (int key, char* arg, struct argp_state* state)
{
	struct arguments* arguments = state->input;

	switch (key){
/*	  case 'v':
		  // verbose output selected
		  arguments->verbose = 1;
		  break;*/
	  case 'e':
		  arguments->extract = arg;
		  break;
	  case ARGP_KEY_ARG:
		  if (state->arg_num >= 1) {
			  // too many arguments
			  argp_usage (state);
		  }
		  arguments->args[state->arg_num] = arg;
		  break;
	  case ARGP_KEY_END:
		  if (state->arg_num < 1) {
			  // not enough arguments
			  argp_usage (state);
		  }
		  break;
	  default:
		  return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* Args_doc. Description of the non-optional command-line arguments */
static char args_doc[] = "TIFF";

/* Doc. Program documentation */
static char doc[] = "tifixty -- Calculates the MD5 checksum of the image data (only) within a TIFF file";

/* Argp structure */
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char* argv[])
{
	FILE* extract_out;

	struct arguments arguments;
	arguments.extract = NULL;
//	arguments.verbose = 0;

	int bytes_per_pixel = 3;		/* 3 bytes/pixel = RGB, 4bytes/pixel=rgba */

	int n;
	MD5_CTX c;
	unsigned char out[MD5_DIGEST_LENGTH];

	/* Process arguments */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	/* Initialise binary_out output stream to write
	 * raster data to.
	 */
	if (arguments.extract){
		extract_out = fopen(arguments.extract, "wb");
		if(extract_out==NULL){
			printf("File could not be opened: %d\n", strerror(errno));
			return(1);
		}
	}

	MD5_Init(&c);

    TIFF* tif = TIFFOpen(arguments.args[0], "r");
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

    			/* Write the raster data out */
    			if (extract_out){
    				fwrite(raster, bytes_per_pixel, npixels, extract_out);
    			}

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

    /* Close up the output file if necessary */
    if (extract_out){
    	fclose(extract_out);
    }

    return 0;
}
