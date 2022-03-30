// Image.cpp
// Implementation file for Lab 3: Image Class

// Name: Lauren Dickman
// Date: 2/14/2020

// INVARIANT for the Image class:
//   Images store a 2-D array of pixels indexed by their (row,col)-coordinates
//   such that row is the ROW index and col is the COLUMN index.  Specifically,
//   pixels are indexed FIRST by their row-coordinate and THEN by their
//   column-coordinate.
//   For example,
//      - the upper-right corner is row = 0, col = width-1:  arr[0][width-1]
//      - the lower-left corner is row = height-1, col = 0:  arr[height-1][0]

// MEMBER CONSTANTS
//    static const int MAX_RGB = 255;

// INSTANCE VARIABLES
//    string format;       // image format specifier
//    int width;           // width of the image, in pixels
//    int height;          // height of the image, in pixels
//    Pixel ** arr;        // pointer to a 2-dimensional dynamic array of Pixels

// REMINDER about our suggested order of implementation:
//   - allocate_dynamic_array, basic constructor
//   - setPixel functions
//   - deallocate_dynamic_array, destructor (test using Valgrind)
//   - stream input CONSTRUCTOR
//   - clone_dynamic_array, copy constructor
//   - assignment operator==
//
//  PROVIDED: stream output operator>>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include "Pixel.h"
#include "Image.h"

using namespace std;

namespace imagelab
{
    // allocate_dynamic_array (private member function)
    //   Allocates dynamic memory for member variable `arr`.
    // Postcondition: 
    //   - The instance variable `arr` has been initialized as a new
    //      dynamic 2-d array of Pixels with `height` rows and `width` 
    //      columns.
    void Image::allocate_dynamic_array()
    {
    	arr = new Pixel*[height];
        for (int row = 0; row < height; row++)
        {
          	arr[row] = new Pixel[width];
        }
    }

    // clone_dynamic_array (private member function)
    //   Sets member variable `arr` to point to a clone of the given
    //     2-d array of Pixels.
    // Precondition: 
    //   - `ptr` is a pointer to an array of row-pointers for a 2-d array
    //       of Pixels; `ptr` represents a dynamic array with width `w` > 0 
    //       and height `h` > 0
    // Postconditions:
    //   - The `width` and `height` instance variables have been
    //      set to `w` and `h` respectively.
    //   - The instance variable arr has been initialized as a new
    //      dynamic 2-d array of Pixels identical to the array of
    //      Pixels pointed to by `ptr`.
    //   - The original array pointed to by `ptr` is UNCHANGED.
    void Image::clone_dynamic_array(Pixel** ptr, int w, int h)
    {
    	height = h;
    	width = w; 
    	allocate_dynamic_array();
    	for (int r = 0; r < height; r++)
    	{
    		for (int c = 0; c < width; c++)
    		{
    			arr[r][c] = ptr[r][c];
    		}
    	}
    }

    // deallocate_dynamic_array (private member function)
    //   Deallocates the dynamic memory being used by member variable `arr`.
    // Postcondition:
    //   - All dynamic memory for the array pointed to by `arr` has been
    //      deallocated.
    void Image::deallocate_dynamic_array()
    {
        for (int row = 0; row < height; row++)
    	{
        	delete [] arr[row];
    	}
    	delete [] arr;
    }

    // Basic constructor
    // Preconditions: w > 0; h > 0; 0 <= r <= MAX_RGB; 0 <= g <= MAX_RGB;
    //                     0 <= b <= MAX_RGB
    // Postconditions:
    //    - format is set to "P3"
    //    - width is set to the value of w
    //    - height is set to the value of h
    //    - arr is a 2-D array of pixels with width w and height h
    //    - each pixel in `arr` is set to the same RGB color, (r, g, b)
    // NOTE: Remember to pay attention to the class invariant!
    Image::Image(int w, int h, int r, int g, int b)
    {
        format = "P3"; 
        height = h;
        width = w;
        allocate_dynamic_array();
        for (int row = 0; row < height; row++)
    	{
        	for (int col = 0; col < width; col++)
        	{
           		arr[row][col].set(r, g, b); 
        	}
    	}   
    }

    // Copy constructor
    // Initializes a new Image object to be a clone of the `source` image.
    // NOTE: Remember to pay attention to the class invariant!
    Image::Image(const Image& source)
    {
        format = source.getFormat(); 
        height = source.getHeight();
        width = source.getWidth();
        clone_dynamic_array(source.arr, width, height);        
    }

    // Stream input constructor
    // Initializes a new Image object by reading a PPM image from the input
    // stream, `in`.
    // Postconditions:
    //    - format, width, and height for the new Image object
    //      are set based on values read from the input stream
    //    - `arr` points to a 2-D array of pixels which is populated
    //        by the pixel values read from the input stream
    // NOTE: Remember to pay attention to the class invariant!
    Image::Image(std::istream& in)
    {
    	int junk, r, g, b;
        in >> format;
        in >> width;
        in >> height;
        in >> junk;
        in >> r;
        in >> g;
        in >> b;
        allocate_dynamic_array();
        for (int row = 0; row < height; row++)
    	{
        	for (int col = 0; col < width; col++)
        	{
           		arr[row][col].set(r, g, b);
           		in >> r;
        		in >> g;
        		in >> b; 
        	}
    	}   
    }

    // MODIFIERS

    // setPixel, 3 argument version; takes row, col, pix
    // Preconditions: 0 <= row <= height; 0 <= col <= width
    // Postconditions: pixel at arr[row][col] is set to pix
    void Image::setPixel(int row, int col, Pixel pix)
    {
        if ((0 <= row ) && (0 <= col))
        {
        	arr[row][col] = pix; 
        }
    }

    // setPixel, 5 argument version; takes row, col, r, g, b
    // Preconditions: 0 <= row <= height; 0 <= col <= width;
    //                     0 <= r, g, b <= MAX_RGB
    // Postconditions: pixel at arr[row][col] has RGB values (r, g, b)
    void Image::setPixel(int row, int col, int r, int g, int b)
    {
    	int MAX_RGB = 255;
    	if ((0 <= row) && (0 <= col))
    	{
    		if ((0 <= r && r <= MAX_RGB) && (0 <= b && b <= MAX_RGB) && (0 <= g && g <= MAX_RGB))
    		{
        		arr[row][col].set(r, g, b);
    		}
        }
    }

    // DESTRUCTOR, ~Image
    // Deallocates memory used by an Image object, to prevent memory leaks.
    // Postcondition: all dynamic memory allocated to the object is deleted
    // Postcondition: `arr` is set to NULL
    // DESTRUCTOR
    Image::~Image()
    {
      	deallocate_dynamic_array();
      	arr = NULL; 
    }

    // Assignment operator overload (member function)
    // Postcondition: this image has the same format, width and height as
    //    `source`
    // Postcondition: this image has a pixel array `arr` which is a *deep copy*
    //     of the pixel array for `source`.
    // Postcondition: the two pixel arrays are independent (unless `source` and
    //     `this` are literally the same object), but they store the exact same
    //     pattern of pixels
    Image& Image::operator= (const Image& source)
    {
        // start with a self-assignment check (early return of *this)
    	if (this == &source)
    		return *this;
        // deallocate dynamic memory for this image
    	deallocate_dynamic_array();
    	// copy the data from source to this, reallocating dynamic
        // memory to match the dimensions of source
    	format = source.getFormat(); 
        height = source.getHeight();
        width = source.getWidth();
        clone_dynamic_array(source.arr, width, height);
        for (int r = 0; r < height; r++)
    	{
    		for (int c = 0; c < width; c++)
    		{
    			arr[r][c] = source.arr[r][c];
    		}
    	}
        // return the "this" object reference so we can chain the operator
        return *this;
    }

    // OUTPUT STREAM OPERATOR -- FRIEND -- PROVIDED CODE
    // Puts the image into an output stream, in PPM format.
    //    1st item: 2-character format string
    //    2nd item: image width in pixels
    //    3rd item: image height in pixels
    //    4th item: maximum pixel color
    //    Subsequent items:  the RGB values for each pixel, row-by-row
    //
    // For example, a very small image with 16 pixels and max color
    // value of 255 might look like the following:
    //    P3
    //    4 4
    //    255
    //    0  0  0
    //    0  0  0
    //    0  0  0
    //    15  0 15
    //    0  0  0
    //    0 15  7
    //    0  0  0
    //    0  0  0
    //    0  0  0
    //    0  0  0
    //    0 15  7
    //    0  0  0
    //    15  0 15
    //    0  0  0
    //    0  0  0
    //    0  0  0
    //
    std::ostream& operator<< (std::ostream &out, const Image &image)
    {
        int h = image.getHeight();
        int w = image.getWidth();

        out << image.getFormat() << '\n'
             << w << " " << h << '\n'
             << Image::RGB_MAX << '\n';

        for (int row = 0; row < h; row++)
            for (int col = 0; col < w; col++)
                out << image.getPixel(row, col) << '\n';

        return out;     // return out for chaining
    }
}  // namespace imagelab
