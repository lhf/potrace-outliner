# potrace-outliner

This is a tool for extracting polygonal outlines from bitmap images.

The input is a PBM, PGM, PPM, or BMP bitmap. The output is EPS; it can be easily changed by modifying main.c.

Better results can often be obtained by pre-processing the input with [mkbitmap](https://potrace.sourceforge.net/mkbitmap.html).

All the code and data is from [potrace](http://potrace.sourceforge.net/), except main.c, which is nevertheless loosely based on potracelib_demo.c. The code is subject to GPL.
