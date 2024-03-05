#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 


void save_to_file(char *input_format, char *output_format, FILE *input_file, FILE *output_file){
	if (input_format[0] == output_format[0]){
		char s[10] = "";
		while(fscanf(input_file, "%s", s) > 0){
			fprintf(output_file, "%s ", s);
		}
	}
	else if (input_format[0] == 's'){
		fprintf(output_file, "%s\n", "P3");
		unsigned int width, height, colors;
		char s[10] = "";
		fscanf(input_file, "%s", s);
		fscanf(input_file, "%u", &width);
		fscanf(input_file, "%u", &height);
		fscanf(input_file, "%u", &colors);
		//printf("%u\n", colors);
		fprintf(output_file, "%u %u\n", width, height);
		fprintf(output_file, "%u\n", 255);
		unsigned int color_table[colors][3];
		for (unsigned int i = 0; i < colors; i++){
			fscanf(input_file, "%u %u %u", &color_table[i][0], &color_table[i][1], &color_table[i][2]);
			//printf("%u %u %u ", color_table[i][0], color_table[i][1], color_table[i][2]);
		}
		char str[10] = "";
		while(fscanf(input_file, "%s", str) > 0){
			unsigned int pixel = 0;
			if(str[0] == '*'){
				unsigned int repetitions = strtol(strtok(str, "*"), NULL, 10);
				fscanf(input_file, "%s", str);
				pixel = strtol(str, NULL, 10);
				//printf("*%u %u ", repetitions, pixel);

				for (unsigned int j = 0; j < repetitions; j++){
					fprintf(output_file, "%u %u %u ", color_table[pixel][0], color_table[pixel][1], color_table[pixel][2]);
					//printf("%u %u %u ", color_table[pixel][0], color_table[pixel][1], color_table[pixel][2]);
				}
			}else{
				pixel = strtol(str, NULL, 10);
				//printf("%u ", pixel);
				fprintf(output_file, "%u %u %u ", color_table[pixel][0], color_table[pixel][1], color_table[pixel][2]);
			}
		}
	}else{
		fprintf(output_file, "%s\n", "SBU");
		unsigned int width = 0, height = 0, colorstream = 0;
		char s[10] = "";
		fscanf(input_file, "%s", s);
		fscanf(input_file, "%u", &width);
		fscanf(input_file, "%u", &height);
		fscanf(input_file, "%u", &colorstream);
		fprintf(output_file, "%u %u\n", width, height);
		unsigned int color_table[width * height][3], counter = 0;
		unsigned int r = 0,g = 0,b = 0;
		unsigned int pixels[width][height], x_counter = 0, y_counter = 0;
		fscanf(input_file, "%u %u %u", &r,&g,&b);
		color_table[0][0] = r;
		color_table[0][1] = g;
		color_table[0][2] = b;
		counter++;
		printf("%u %u %u\n", color_table[0][0],color_table[0][1],color_table[0][2]);
		pixels[0][0] = 0;
		y_counter++;
		while(fscanf(input_file, "%u %u %u", &r,&g,&b) > 0){
			unsigned int pixel_number = -1;
			int index_found = 0;
			
			for (unsigned int i = 0; i < counter; i++){
				if (r == color_table[i][0] && g == color_table[i][1] && b == color_table[i][2])
				{
					pixel_number = i;
					index_found = 1;
				}
			}
			if(index_found == 0){
				color_table[counter][0] = r;
				color_table[counter][1] = g;
				color_table[counter][2] = b;
				pixel_number = counter;
				counter++;
			}
			if (y_counter == height){
				y_counter = 0;
				x_counter++;
			}
			pixels[x_counter][y_counter] = pixel_number;
			y_counter++;
		}

		fprintf(output_file, "%u\n", counter);
		for (unsigned int i = 0; i < counter; i++){
			fprintf(output_file, "%u %u %u ", color_table[i][0],color_table[i][1],color_table[i][2]);
		}
		int repetitions = 1;
		unsigned int last_pixel = pixels[0][0];
		for (unsigned int m = 0; m < width; m++){
			for (unsigned int k = 0; k < height; k++){
				if(m == 0 && k == 0){
					continue;
				}
				if (k == 0){
					last_pixel = pixels[m-1][height-1];
				}
				else{
					last_pixel = pixels[m][k-1];
				}
				if(m == 0){
					//printf("%u %u\n", last_pixel, pixels[m][k]);
				}
				if(pixels[m][k] == last_pixel)
					repetitions++;
				else{
					if(repetitions > 1){
						fprintf(output_file, "*%d %u ", repetitions, last_pixel);
						//printf("*%d %u ", repetitions, last_pixel);
						repetitions = 1;
					}else{
						fprintf(output_file, "%u ", last_pixel);
						//printf("%u ", last_pixel);
					}
				}
			}	
		}
		if(repetitions > 1){
				fprintf(output_file, "*%d %u ",repetitions, pixels[width - 1][height - 1]);
				repetitions = 1;
			}else{
				fprintf(output_file, "%u ", pixels[width - 1][height - 1]);
			}
	}
}
void copy_pixels(unsigned int *copy_info, char *input_format, FILE *file,int *pixels){
	unsigned int row = copy_info[0], col = copy_info[1], copy_width = copy_info[2], copy_height = copy_info[3];
	//printf("%u %u %u %u\n", row, col, copy_width, copy_height);
	if (input_format[0] == 's'){
		unsigned int width, height, colors;
		char s[10] = "";
		fscanf(file, "%s", s);
		fscanf(file, "%u", &width);
		fscanf(file, "%u", &height);
		fscanf(file, "%u", &colors);
		unsigned int color_table[colors][3];
		for (unsigned int i = 0; i < colors; i++){
			fscanf(file, "%u %u %u", &color_table[i][0], &color_table[i][1], &color_table[i][2]);
		}
		char str[15] = "";
		unsigned int current_row = 0, current_col = 0, counter = 0;
		while(fscanf(file, "%s", str) > 0){
			unsigned int pixel = 0;
			if(str[0] == '*'){
				unsigned int repetitions = strtol(strtok(str, "*"), NULL, 10);
				fscanf(file, "%s", str);
				pixel = strtol(str, NULL, 10);
				for (unsigned int j = 0; j < repetitions; j++){
					if (current_col == width){
						current_col = 0;
						current_row++;
					}
					counter++;
					//printf("%u %u\n", current_row, current_col);
					if (current_row >= row && current_row < row + copy_height && current_col >= col && current_col < col + copy_width){
						*pixels++ = pixel;
						//printf("%u\n", counter);
						//printf("%u ", *(pixels-1));
					}
					current_col++;
				}
			}else{
				pixel = strtol(str, NULL, 10);
				if (current_col == width){
						current_col = 0;
						current_row++;
				}
				counter++;
				//printf("%u %u\n", current_row, current_col);
				if (current_row >= row && current_row < row + copy_height && current_col >= col && current_col < col + copy_width){
					*pixels++ = pixel;
					//printf("%u\n", counter);
					//printf("%u ", *(pixels-1));
				}
				current_col++;
			}
			
		}
	}else{
		unsigned int width = 0, height = 0, colorstream = 0;
		char s[10] = "";
		fscanf(file, "%s", s);
		fscanf(file, "%u", &width);
		fscanf(file, "%u", &height);
		fscanf(file, "%u", &colorstream);
		unsigned int color_table[width * height][3], counter = 0;
		unsigned int r = 0,g = 0,b = 0;
		unsigned int x_counter = 0, y_counter = 0;
		fscanf(file, "%u %u %u", &r,&g,&b);
		color_table[0][0] = r;
		color_table[0][1] = g;
		color_table[0][2] = b;
		counter++;
		y_counter++;
		while(fscanf(file, "%u %u %u", &r,&g,&b) > 0){
			unsigned int pixel_number = -1;
			int index_found = 0;
			for (unsigned int i = 0; i < counter; i++){
				if (r == color_table[i][0] && g == color_table[i][1] && b == color_table[i][2])
				{
					pixel_number = i;
					index_found = 1;
				}
			}
			if(index_found == 0){
				color_table[counter][0] = r;
				color_table[counter][1] = g;
				color_table[counter][2] = b;
				pixel_number = counter;
				counter++;
			}
			if (y_counter == width){
				y_counter = 0;
				x_counter++;
			}
			if (x_counter >= row && x_counter < row + copy_height && y_counter >= col && y_counter < col + copy_width){
				*pixels++ = pixel_number;
			}
			y_counter++;
		}
	}
}
void paste_pixels(unsigned int *paste_info, unsigned int paste_height, unsigned int paste_width, char *input_format, FILE *file,int *pixels, char *file_name){
	unsigned int row = paste_info[0], col = paste_info[1];
	if (input_format[0] == 's'){
		unsigned int width, height, colors;
		char s[10] = "";
		fscanf(file, "%s", s);
		fscanf(file, "%u", &width);
		fscanf(file, "%u", &height);
		fscanf(file, "%u", &colors);
		unsigned int color_table[colors][3];
		for (unsigned int i = 0; i < colors; i++){
			fscanf(file, "%u %u %u", &color_table[i][0], &color_table[i][1], &color_table[i][2]);
		}
		char str[10] = "";
		unsigned int current_row = 0, current_col = 0;
		unsigned int image_pixels[height][width];
		unsigned int *image_pixel = &image_pixels[0][0];
		unsigned int color_occurences[colors];
		unsigned int new_color_index[colors];
		unsigned int new_color_table[colors][3];
		unsigned int new_colors = 0;
	

		while(fscanf(file, "%s", str) > 0){
			//printf("%d\n", 1);
			unsigned int pixel = 0;
			if(str[0] == '*'){
				unsigned int repetitions = strtol(strtok(str, "*"), NULL, 10);
				fscanf(file, "%s", str);
				pixel = strtol(str, NULL, 10);
				for (unsigned int j = 0; j < repetitions; j++){
					if (current_col == width){
						current_col = 0;
						current_row++;
					}
					//printf("%u %u\n", current_row, current_col);
					if (current_row >= row && current_row < row + paste_height && current_col >= col && current_col < col + paste_width){
						*image_pixel++ = *pixels++;
					}
					else{
						*image_pixel++ = pixel;
					}
					if(color_occurences[*(image_pixel - 1)] == 0){
						color_occurences[*(image_pixel - 1)]++;
						new_color_table[new_colors][0] = color_table[*(image_pixel - 1)][0];
						new_color_table[new_colors][1] = color_table[*(image_pixel - 1)][1];
						new_color_table[new_colors][2] = color_table[*(image_pixel - 1)][2];
						new_color_index[*(image_pixel - 1)] = new_colors;
						*(image_pixel - 1) = new_colors;
						new_colors++;
					}else{
						color_occurences[*(image_pixel - 1)]++;
						*(image_pixel - 1) = new_color_index[*(image_pixel - 1)];
					}
					current_col++;
				}
			}else{
				pixel = strtol(str, NULL, 10);
				if (current_col == width){
						current_col = 0;
						current_row++;
				}
				//printf("%u %u\n", current_row, current_col);
				if (current_row >= row && current_row < row + paste_height && current_col >= col && current_col < col + paste_width){
						*image_pixel++ = *pixels++;
						//printf("%u ", *(image_pixel-1));

				}
				else{
					*image_pixel++ = pixel;
					//printf("%u ", *(image_pixel-1));

				}
				if(color_occurences[*(image_pixel - 1)] == 0){
						color_occurences[*(image_pixel - 1)]++;
						new_color_table[new_colors][0] = color_table[*(image_pixel - 1)][0];
						new_color_table[new_colors][1] = color_table[*(image_pixel - 1)][1];
						new_color_table[new_colors][2] = color_table[*(image_pixel - 1)][2];
						new_color_index[*(image_pixel - 1)] = new_colors;
						*(image_pixel - 1) = new_colors;
						new_colors++;
					}else{
						color_occurences[*(image_pixel - 1)]++;
						*(image_pixel - 1) = new_color_index[*(image_pixel - 1)];
					}
				current_col++;
			}
		}
		//printf("10\n");
				//printf("%u\n", new_colors);

			fclose(file);
			file = fopen(file_name, "w");
			fprintf(file, "%s\n", "SBU");
			fprintf(file, "%u ", width);
			fprintf(file, "%u\n", height);
			fprintf(file, "%u\n", new_colors);
			for (unsigned int i = 0; i < new_colors; i++){
				fprintf(file, "%u %u %u ", new_color_table[i][0], new_color_table[i][1], new_color_table[i][2]);
			}
			int repetitions = 1;
			unsigned int last_pixel = image_pixels[0][0];
			for (unsigned int m = 0; m < height; m++){
				for (unsigned int k = 0; k < width; k++){
					if(m == 0 && k == 0){
						continue;
					}
					if (k == 0){
						last_pixel = image_pixels[m-1][width-1];
					}
					else{
						last_pixel = image_pixels[m][k-1] ;
					}
					if(image_pixels[m][k] == last_pixel)
						repetitions++;
					else{
						if(repetitions > 1){
							fprintf(file, "*%d %u ", repetitions, last_pixel);
							//printf("*%d %u ", repetitions, last_pixel);
							repetitions = 1;
						}else{
							fprintf(file, "%u ", last_pixel);
							//printf("%u ", last_pixel);
						}
					}
				}	
			}
			if(repetitions > 1){
				//h
				fprintf(file, "*%d %u ",repetitions, image_pixels[height - 1][width - 1]);
				//printf("*%d %u ", repetitions, last_pixel);
				repetitions = 1;
			}else{
				fprintf(file, "%u ", image_pixels[height - 1][width - 1]);
				//printf("%u ", last_pixel);
			}
		fclose(file);
		file = fopen(file_name, "r");
	}	
}
int main(int argc, char **argv) {
	/*for (int i = 0; i < argc; i++){
		printf("%s\n", argv[i]);
	}*/
    extern char *optarg;
	extern int optind;
	char *input_file, *output_file, *path_to_font;
	unsigned int *copy_info = malloc(16), width = 0, height = 0;
	unsigned int *paste_info = malloc(8);
    int c, c_parameters = 0, p_parameters = 0, r_parameters = 0;
    int iflag = 0, oflag = 0, unknown_flag = 0, cflag = 0, pflag = 0, rflag = 0;
    while ((c = getopt(argc, argv, "i:o:c:p:r:")) != -1){
		switch (c) {
		case 'i':
			input_file = optarg;
			iflag++;
			break;
		case 'o':
			output_file = optarg;
			oflag++;
			break;
		case 'c':
			cflag++;
			char* ctoken = strtok(optarg, ",");
			copy_info[0] = strtol(ctoken, NULL, 10);
			c_parameters++;
			while(ctoken != NULL){
				ctoken = strtok(NULL, ",");
				if(c_parameters == 1  && ctoken != NULL){
					copy_info[1] = strtol(ctoken, NULL, 10);
				}
				if(c_parameters == 2  && ctoken != NULL){
					copy_info[2] = strtol(ctoken, NULL, 10);
				}
				if(c_parameters == 3  && ctoken != NULL){
					copy_info[3] = strtol(ctoken, NULL, 10);
				}
				c_parameters++;
			};
			c_parameters--;
			break;
		case 'p':
			pflag++;
			char* ptoken = strtok(optarg, ",");
			paste_info[0] = strtol(ptoken, NULL, 10);
			p_parameters++;
			while(ptoken != NULL){
				ptoken = strtok(NULL, ",");
				if(p_parameters == 1 && ptoken != NULL){
					paste_info[1] = strtol(ptoken, NULL, 10);
				}
				p_parameters++;
			};
			p_parameters--;
			break;
		case 'r':
			rflag++;
			char* rtoken = strtok(optarg, ",");
			do{
				r_parameters++;
				if (r_parameters == 2){
					path_to_font = rtoken;
				}
				rtoken = strtok(NULL, ",");
			} while(rtoken != NULL);
			//printf("%d\n", r_parameters);
			break;
		case '?':
			unknown_flag++;
			break;
		}
		
	}

    if (iflag == 0) {
        return MISSING_ARGUMENT;
	}
	
    if (oflag == 0) {
        return MISSING_ARGUMENT;
	}
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			if(argv[i][1] != 'o' && argv[i][1] != 'i' && argv[i][1] != 'p' && argv[i][1] != 'r' && argv[i][1] != 'c')
				continue;
			if (i + 1 == argc){
				return MISSING_ARGUMENT;
			}else if (argv[i + 1][0] == '-'){
				return MISSING_ARGUMENT;
			}
		}
	}

	if (unknown_flag != 0)
		return UNRECOGNIZED_ARGUMENT;
	if (oflag > 1 || iflag > 1 || cflag > 1 || pflag > 1 || rflag > 1)
    	return DUPLICATE_ARGUMENT;
	FILE *fpi = fopen(input_file, "r");
	if (iflag > 0 && fpi == NULL)
		return INPUT_FILE_MISSING;
	
	FILE *fpo = fopen(output_file, "w");
	if (oflag > 0 && fpo == NULL)
		return OUTPUT_FILE_UNWRITABLE;
	
	if (pflag != 0 && cflag == 0) {
        return C_ARGUMENT_MISSING;
	}
	if (cflag > 0 && c_parameters != 4){
		return C_ARGUMENT_INVALID;
	}
	if (pflag > 0 && p_parameters != 2){
		return P_ARGUMENT_INVALID;
	}
	if (rflag > 0 && r_parameters != 5){
		return R_ARGUMENT_INVALID;
	}
	if (rflag > 0){
		FILE *fpr = fopen(path_to_font, "r");
		if(fpr == NULL){
			return R_ARGUMENT_INVALID;
		}else{
		fclose(fpr);
		}
	}
	strtok(input_file, ".");
	char* input_format = strtok(NULL, ".");
	strtok(output_file, ".");
	char* output_format = strtok(NULL, ".");
	if(cflag){
		char s[10];
		fscanf(fpi, "%s", s);
		fscanf(fpi, "%u", &width);
		fscanf(fpi, "%u", &height);
		if (copy_info[2] + copy_info[1] > width){
			copy_info[2] = width - copy_info[1];
		}
		if (copy_info[3] + copy_info[0] > height){
			copy_info[3] = height - copy_info[0];
		}
		//printf("%u %u\n", copy_info[2], copy_info[3]);
		rewind(fpi);
		int copied_pixels[copy_info[3]][copy_info[2]];
		int *pixels = &copied_pixels[0][0];
		copy_pixels(copy_info, input_format, fpi, pixels);
		pixels = &copied_pixels[0][0];
		// for(unsigned int i = 0; i < (copy_info[3] * copy_info[2]); i++)
		// 	printf("%u ", *pixels++);
		// pixels = &copied_pixels[0][0];
		rewind(fpi);
		if (pflag) {
			paste_pixels(paste_info, copy_info[3], copy_info[2], input_format, fpi, pixels, input_file);
		}
	}
	free(copy_info);
	free(paste_info);
	save_to_file(input_format, output_format, fpi, fpo);
	fclose(fpi);
	fclose(fpo);
	return 0;
	
}

