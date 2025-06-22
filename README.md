# Image Manipulator

This image manipulation tool works on two particular image formats, standard PPM and SBU (a custom format with further details below). The program provides 3 key features:
- Conversion of image pixels from PPM to SBU formatting (compression) or vice versa.
- Copying of a certain image pixel region, in either format, onto another destination region of an image.
- Printing text onto a certain region of a PPM or SBU image with custom font type and size options.

## SBU Image Format
The formatting of the SBU image file is as follows
```
SBU
[width in pixels] [length in pixels]
[number of colors present] [color 1 in RGB] [color 2 in RGB] ...
[pixel colors] ...
```
The colors are listed in order of their appearance from left to right, top to bottom of the image pixels. The colors are also 0-indexed in row two. Pixel colors are shown differently depending on whether or not there is a continuous row of pixels of the same color. As an example, a 3-by-3 SBU image in the following format

```
Red    Red    Red

Blue   Green  Red

Blue   Blue   Green
```
would be represented as the following SBU image file
```
SBU
3 3
3 255 0 0 0 0 255 0 255 0
*3 0 2 1 0 *2 2 1
```
- Above, the color red shows up 3 times in a row, so we represent that using *3, for three continuous pixels, followed by the color index in row two, which is 0.
- The color green always appears once discontinuously, so we represent that using just the color index 2 for the respective pixel.

## Command Line Interface
```
prog -i path_to_input_img -o path_to_output_img [-c row,col,row_length,col_length] [-p row,col] [-r "text","path_to_font.txt",font_size,row,col]
```
- -c : Region in pixels to copy. Must be present if -p option present.
- -p : Region to paste onto using starting pixel location. Must be present if -c option present. Out of bounds pixels will be discarded.
- -r : Place "text" onto image from indicated starting pixel using the font file and size.
