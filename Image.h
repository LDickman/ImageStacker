// FILE:    Image.h
// Authors: T. Wilson, B. Wahl
// Revised: 2-14-2020
//
// CLASS PROVIDED: Image, as part of the namespace, `imagelab`
//
// MEMBER CONSTANTS for the Image class:
//
//   static const int RGB_MAX = 255, the largest color value allowed
//     in the RGB color system
//
// CONSTRUCTORS for the Image class:
//
//   Image(int w=100, int h=100, int r=0, int g=0, int b=0)
//     Sets the format, width, and height for the the new Image object. 
//     Also creates a default image of the designated width and height 
//     that is a 2-D array of Pixel objects where all Pixels have the 
//     RGB color specified by r, g, and b.
//
//     Preconditions: w > 0; h > 0; 0 <= r <= RGB_MAX; 0 <= g <= RGB_MAX; 
//                    0 <= b <= RGB_MAX
//     Postconditions:
//        format is set to "P3".
//        width is set to w.
//        height is set to h.
//        arr is a 2-D array of pixels with width w and height h.
//        Each pixel in the pixel array is set to RGB color (r, g, b)
//
//   Image(const Image& source)
//     Initializes an independent copy of an existing Image object.
//     Postconditions:
//        new Image is initialized to have the same format, width,
//        and height as `source`.
//
//        new Image's array, `arr`, is a 2-D array of pixels with the same
//        width and height as source's array of pixels.
//
//        Each pixel in the copy's pixel array has the same RGB color values
//        as the pixel at the identical coordinates in source's pixel array.
//
//        `arr` for the new Image is stored independently of the source
//        object's pixel array (deep copy).
//
//   Image(istream& in)
//     Reads a PPM image from the stream input. Sets the format, width
//       and height for the new Image object based on the input, and creates 
//       a 2-D array of pixels, which is populated by the RGB values read from 
//       the stream `in`. Pixels in the first row come before pixels in the
//       second row, and so on.
//
//     PPM format in brief:
//        All items in PPM format are separated by whitespace.
//          1st item: 2-character format string
//          2nd item: image width in pixels
//          3rd item: image height in pixels
//          4th item: maximum pixel color (RGB_MAX) ***READ but ignore***
//          5th item: pixel-1 red value
//          6th item: pixel-1 green value
//          7th item: pixel-1 blue value
//          8th item: pixel-2 red value
//          9th item: pixel-2 green value
//            .... and so on until the RGB values have been given
//            for all the pixels in the image
//
//        For example, the following are the first 8 lines from a
//        PPM file that is 600 x 339 pixels.  The RGB values for
//        the first five pixels in the image are shown.
//
//          P3
//          600 339
//          255
//          0 0 0
//          0 10 16
//          86 8 14
//          0 0 0
//          84 6 8
//
//        The PPM files that you will be working with will always be formatted as follows:
//        
//            1st line: file format string
//            2nd line: width height
//            3rd line: RGB_MAX
//            4th line: r g b
//            5th line: r g b
//                .
//                .
//            last line: r g b
//
//        Note that from line 4 to the end of the file, there will be one pixel
//        per line.
//
// Postconditions:
//        - format, width, and height for the new Image object are set based on 
//          values read from the input stream
//
//        - `arr` points to a dynamic 2-D array of pixels which is populated
//          by the pixel values read from the input stream
//
//
// DESTRUCTOR for the Image class:
//
//   ~Image()
//      Deallocates dynamic memory used by an Image to prevent memory leaks.
//      Postcondition: All dynamic memory allocated to the object is deleted,
//        and the `arr` pointer is set to NULL.
//
//
// MODIFIERS for the Image class:
//
//   void setPixel(int row, int col, Pixel pix)
//     The pixel in arr at position (row, col) is set to pix.
//     (Hint: Use the copy constructor.)
//     Preconditions:
//       0 <= row < height; 0 <= col < width
//     Postcondition:
//       the pixel at position (row, col) is pix
//
//   void setPixel(int row, int col, int r, int g, int b);
//     The pixel at position (row, col) is set to the given RGB values.
//     Preconditions:
//       0 <= row < height; 0 <= col < width; 0 <= r, g, b <= RGB_MAX
//     Postcondition:
//       the pixel at position (row, col) has RGB values (r, g, b)
//
//
// ACCESSORS for the Image class:
//
//   Pixel getPixel(int row, int col) const
//     Precondition:
//       (row, col) is a valid position in the array for this image
//     Postcondition:
//       returns the pixel at position (row, col)
//
//   string getFormat() const
//     Postcondition: returns the format of the image
//
//   int getWidth() const
//     Postcondition: returns the width of the image in pixels
//
//   int getHeight() const
//     Postcondition: returns the height of the image in pixels
//
//
// VALUE SEMANTICS for the Image class:
//   The copy constructor may be used with Image objects.
//   The assignment operator may be used with Image objects.
//
//
// OPERATOR OVERLOADING for the Image class:
//
//   Image& operator= (const Image& source)
//     Assignment operator (=) for the Image class.
//     Makes it possible to write code such as `image2 = image1` to make
//        a deep copy of an existing Image object.
//     Note: The assignment operator returns the `this` reference to enable
//        chaining of assignments, as in the following:  im1 = im2 = im3;
//
//   friend std::ostream& operator<< (std::ostream& out, const Image& img)
//     Puts the PPM version of the image into an output stream, `out`.
//     Note: Returns the `out` parameter to allow chaining of stream output.
//
#ifndef IMAGE_H
#define IMAGE_H

namespace imagelab
{
class Image
{
  public:
    // Member constants
    static const int RGB_MAX = 255;

    // Constructors
    Image(int w=100, int h=100, int r=0, int g=0, int b=0);
    Image(const Image& source);
    Image(std::istream& in);

    // Accessors, with in-line implementations provided
    std::string getFormat() const { return format; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Pixel getPixel(int row, int col) const { return arr[row][col]; }

    // Modifiers
    void setPixel(int row, int col, Pixel pix);
    void setPixel(int row, int col, int r, int g, int b);

    // Destructor
    ~Image();

    // Assignment operator
    Image& operator= (const Image& source);

    // Friend declarations
    friend std::ostream& operator<< (std::ostream& out, const Image& image);

  private:
    // Instance variables
    std::string format;  // image format string
    int width;           // width of the image, in pixels
    int height;          // height of the image, in pixels
    Pixel** arr;         // pointer to a 2-dimensional dynamic array of Pixels

    // Private member functions
    void allocate_dynamic_array();
    void clone_dynamic_array(Pixel** ptr, int w, int h);
    void deallocate_dynamic_array();

};  // end Image class definition

// NON-MEMBER FUNCTIONS

// Stream insertion operator (friend)
std::ostream& operator<< (std::ostream& out, const Image& image);

#endif /* IMAGE_H */

}  // namespace imagelab
