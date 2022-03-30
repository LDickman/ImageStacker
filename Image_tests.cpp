// Image_tests.cpp
// Unit tests for Image class, CS 223 Lab 3
// B. Wahl and T. Wilson
// Revised 2-14-2020
//
// ******************************************************************
// NOTE 1: Most of the provided tests are commented out. Once you get
//   the first constructor (and helper function) and getters implemented
//   and passing the first three provided tests, UNCOMMENT the set of
//   tests for the next function you will implement.
//
//   Reminder about our suggested order of implementation:
//     - allocate_dynamic_array, basic constructor
//     - setPixel functions 
//     - deallocate_dynamic_array, destructor (test using Valgrind)
//     - stream input CONSTRUCTOR
//     - clone_dynamic_array, copy constructor
//     - assignment operator==
//
// ******************************************************************
// UNCOMMENT each tests below when to are ready to work start
// implementing the corresponding function.
//
// ******************************************************************
//  Lauren Dickman
//  2/14/2020
//
// NOTE 2: Students need to add three test implementations to this file
//   (scroll to bottom, just above main, to see the declarations):
//
//   - TEST(TestAssignmentOperator, STU_assignSmallerImageToBiggerImage)
//
//   - TEST(TestAssignmentOperator, STU_assignBiggerImageToSmallerImage)
//
//   - TEST(TestCopyConstructor, STU_basicOperation)
// ******************************************************************

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "Pixel.h"        // provides the Pixel class
#include "Image.h"        // provides the Image class
#include "gtest/gtest.h"  // declares the testing framework

using namespace std;
using namespace imagelab;

namespace {

static const int RGB_MAX = imagelab::Image::RGB_MAX;

// basic constructor, accessors
TEST(TestBasicConstructorAndGetters, PRVD_DefaultConstructor)
{
    int height = 100;
    int width = 100;
    Image im1;
    Pixel black;
    EXPECT_EQ(width, im1.getWidth());
    EXPECT_EQ(height, im1.getHeight());
    EXPECT_EQ("P3", im1.getFormat());
    EXPECT_EQ(black, im1.getPixel(0, 0));
    EXPECT_EQ(black, im1.getPixel(height - 1, width - 1));
    EXPECT_EQ(black, im1.getPixel(0,  width - 1));
    EXPECT_EQ(black, im1.getPixel(height - 1,  0));
    EXPECT_EQ(black, im1.getPixel(13, 79));
}

TEST(TestBasicConstructorAndGetters, PRVD_red5_green6_blue7_width8_height9)
{
    int height = 9;
    int width = 8;
    Image im1(width, height, 5, 6, 7);
    Pixel pix(5, 6, 7);
    EXPECT_EQ(width, im1.getWidth());
    EXPECT_EQ(height, im1.getHeight());
    EXPECT_EQ("P3", im1.getFormat());    
    EXPECT_EQ(pix, im1.getPixel(0, 0));
    EXPECT_EQ(pix, im1.getPixel(0, width - 1));
    EXPECT_EQ(pix, im1.getPixel(height - 1, 0));
    EXPECT_EQ(pix, im1.getPixel(height - 1, width - 1));
    EXPECT_EQ(pix, im1.getPixel(2, 6));
}

TEST(TestBasicConstructorAndGetters, PRVD_redMax_greenMax_blueMax_width1_height1)
{
    int height = 1;
    int width = 1;
    Image im1(width, height, 5, 6, 7);
    Pixel pix(5, 6, 7);
    EXPECT_EQ(1, im1.getWidth());
    EXPECT_EQ(1, im1.getHeight());
    EXPECT_EQ("P3", im1.getFormat());
    EXPECT_EQ(pix, im1.getPixel(0, 0));  // (0, 0) is the only pixel
}

// // setPixel, 3 arguments -- setPixel(int x, int y, Pixel pix)
// 
TEST(TestSetPixelThreeArgs, PRVD_checkForAliasingProblems)
{
    int height = 9;
    int width = 8;
    Image im1(width, height, 5, 6, 7);
    Pixel pix1(0, 20, RGB_MAX);
    Pixel pix2 = pix1;  // clone of pix1
    im1.setPixel(3, 5, pix1);
    // changing pix1 should NOT change im1
    pix1.set(9, 9, 9);
    EXPECT_EQ(pix2, im1.getPixel(3, 5));
}

TEST(TestSetPixelThreeArgs, PRVD_checkCornersOfImageAreSet)
{
    int height = 9;
    int width = 8;
    Image im1(width, height, 5, 6, 7);
    Pixel pix(0, 20, RGB_MAX);
    im1.setPixel(0, 0, pix);
    im1.setPixel(height-1, width-1, pix);
    im1.setPixel(0, width-1, pix);
    im1.setPixel(height-1, 0, pix);
    EXPECT_EQ(pix, im1.getPixel(0, 0));
    EXPECT_EQ(pix, im1.getPixel(height-1, width-1));
    EXPECT_EQ(pix, im1.getPixel(0, width-1));
    EXPECT_EQ(pix, im1.getPixel(height-1, 0));
}

// // // setPixel, 5 arguments
TEST(TestSetPixelFiveArgs, PRVD_checkMiddleAndCornersOfImageAreSet)
{
    int height = 9;
    int width = 8;
    Image im1(width, height, 5, 6, 7);
    Pixel pix(0, 20, RGB_MAX);
    // set five pixels to color (0, 20, RGB_MAX)
    im1.setPixel(0, 0, 0, 20, RGB_MAX);
    im1.setPixel(height-1, width-1, 0, 20, RGB_MAX);
    im1.setPixel(0, width-1, 0, 20, RGB_MAX);
    im1.setPixel(height-1, 0, 0, 20, RGB_MAX);
    im1.setPixel(3, 2, 0, 20, RGB_MAX);
    EXPECT_EQ(pix, im1.getPixel(0, 0));
    EXPECT_EQ(pix, im1.getPixel(height-1, width-1));
    EXPECT_EQ(pix, im1.getPixel(0, width-1));
    EXPECT_EQ(pix, im1.getPixel(height-1, 0));
    EXPECT_EQ(pix, im1.getPixel(3, 2));
}

// // stream input constructor
TEST(TestStreamInputConstructor, PRVD_use_image_n44f_001)
{
    // Read in an image file to create an Image.
    // Check that image format, width, and height are correctly set.  
    // Also check that the first three and the last three pixels of the
    // image are correct.

    // Contents of n44f_001.ppm:
    // P3
    // 600 339
    // 255
    // 0 0 0
    // 0 10 16
    // 86 8 14
    // etc...

    string imagefilestr =
       "/usr/local/share/images/images_noisy/n44f/n44f_001.ppm";
    cout << "            -reading image " << imagefilestr << endl;
    ifstream imagefile(imagefilestr.c_str());
    Image image(imagefile);
    imagefile.close();

    // cout << "    -checking image format and dimensions" << endl;
    EXPECT_EQ("P3", image.getFormat());
    EXPECT_EQ(600, image.getWidth());
    EXPECT_EQ(339, image.getHeight());

    // cout << "    -checking first three pixels of image" << endl;
    EXPECT_EQ(Pixel(0, 0, 0), image.getPixel(0, 0));
    EXPECT_EQ(Pixel(0, 10, 16), image.getPixel(0, 1));
    EXPECT_EQ(Pixel(86, 8, 14), image.getPixel(0, 2));

    // cout << "    -checking last three pixels of image" << endl;
    int w = 600;
    int h = 339;
    EXPECT_EQ(Pixel(255, 100, 0), image.getPixel(h-1, w-3));
    EXPECT_EQ(Pixel(17, 118, 0), image.getPixel(h-1, w-2));
    EXPECT_EQ(Pixel(255, 0, 0), image.getPixel(h-1, w-1));
}

TEST(TestStreamInputConstructor, PRVD_use_image_orion_010)
{
    // Read in an image file to create an Image.
    // Check that image format, width, and height
    // are correctly set.  Also check that the first three
    // and the last three pixels of the the image are correct.

    // P3
    // 600 600
    // 255
    // 0 0 16
    // 0 0 0
    // 14 0 16
    // etc...

    string imagefilestr =
       "/usr/local/share/images/images_noisy/orion/orion_010.ppm";
    cout << "            -reading image " << imagefilestr << endl;
    ifstream imagefile(imagefilestr.c_str());
    Image image(imagefile);
    imagefile.close();

    // cout << "    -checking image format and dimensions" << endl;
    EXPECT_EQ("P3", image.getFormat());
    EXPECT_EQ(600, image.getWidth());
    EXPECT_EQ(600, image.getHeight());

    // cout << "    -checking first three pixels of image" << endl;
    EXPECT_EQ(Pixel(0, 0, 16), image.getPixel(0, 0));
    EXPECT_EQ(Pixel(0, 0, 0), image.getPixel(0, 1));
    EXPECT_EQ(Pixel(14, 0, 16), image.getPixel(0, 2));

    // cout << "    -checking last three pixels of image" << endl;
    int h = 600;
    int w = 600;
    EXPECT_EQ(Pixel(0, 0, 18), image.getPixel(h-1, w-3));
    EXPECT_EQ(Pixel(0, 4, 16), image.getPixel(h-1, w-2));
    EXPECT_EQ(Pixel(0, 4, 0), image.getPixel(h-1, w-1));
}

// // stream output operator<<
TEST(TestStreamOutputOperator, PRVD_basicOperation)
{
    // Read in an image file to create an Image, then write that
    // image out to a new file. USE THE LINUX COMMAND LINE to check
    // for differences in the two files.

    string imagefilestr =
       "/usr/local/share/images/images_noisy/n44f/n44f_001.ppm";
    cout << "            -reading image " << imagefilestr << endl;
    ifstream imagefile(imagefilestr.c_str());
    Image image(imagefile);
    imagefile.close();

    string outfilestr = "testimage.ppm";
    cout << "            -writing image to " << outfilestr << endl;
    ofstream outfile(outfilestr.c_str());
    outfile << image;
    outfile.close();

    cout << "\nIMPORTANT!"<< endl;
    cout << "\nTo check for differences between the original image file \n";
    cout << "and the newly created image file, please run the following \n";
    cout << "command:\n" << endl;
    cout << "    diff " << imagefilestr << " " << outfilestr << endl << endl;
    cout << "If there are no differences, your stream output operator\n";
    cout << "is correct! If there are differences, there's an error in \n";
    cout << "the implementation of your stream output. Use the 'head'\n";
    cout << "and 'tail' commands to inspect the two files and help you \n";
    cout << "figure out what is wrong.\n";
    cout << "\nAs a final test, you should use an image viewer to try to\n";
    cout << "view " << outfilestr << "." << endl;
    cout << "-----------------------------------------" << endl << endl;
}

TEST(TestStreamOutputOperator, PRVD_chaining)
{
    stringstream out;   // simulated output stream
    string correct;     // correct output

    // 1. Set up Image and store correct output
    int height = 2;
    int width = 1;
    Image im1(width, height, 20, 40, 60);
    Pixel pix(20, 40, 60);

    // store correct result for `outstream << im1 << im1`
    // print WIDTH before HEIGHT

    correct = "P3\n1 2\n255\n20 40 60\n20 40 60\n";
    correct = correct + correct;

    // 2. Execute (put sequence into output stream)
    out << im1 << im1;

    // 3. Assert (check that contents of output stream are correct)
    EXPECT_EQ(correct, out.str());
}

// assignment operator=
TEST(TestAssignmentOperator, PRVD_selfAssignmentIsOK)
{
    int height = 9;
    int width = 8;
    Image im1(width, height, 5, 6, 7);
    Pixel pix(5, 6, 7);
    im1 = im1;  // self assignment
    EXPECT_EQ(width, im1.getWidth());
    EXPECT_EQ(height, im1.getHeight());
    EXPECT_EQ("P3", im1.getFormat());
    EXPECT_EQ(pix, im1.getPixel(0, 0));
    EXPECT_EQ(pix, im1.getPixel(height-1, width-1));
    EXPECT_EQ(pix, im1.getPixel(0, width-1));
    EXPECT_EQ(pix, im1.getPixel(height-1, 0));
    EXPECT_EQ(pix, im1.getPixel(3, 2));
}

// copy constructor
TEST(TestCopyConstructor, STU_basicOperation)
{
    // STUDENT: please provide code for this test
    int height = 55;
    int width = 50;
    Pixel pix(30, 90, 70);
    Image im1(width, height, 30, 90, 70);
    Image im2 = im1;
    EXPECT_TRUE(im1.getFormat() == im2.getFormat());

    im1.setPixel(0, 0, 0, 0, 0); // (0,0) is black
    im1.setPixel((height - 1), (width - 1), 255, 0, 0); // (height - 1, width - 1) is red
    im1.setPixel(0, (width - 1), 0, 255, 0); // (0, width - 1) is green
    im1.setPixel((height - 1), 0, 255, 255, 255); // (height - 1, 0) is White

    EXPECT_TRUE(im1.getWidth() == im2.getWidth());
    EXPECT_TRUE(im1.getHeight() == im2.getHeight());
    EXPECT_FALSE(im1.getPixel(0, 0) == im2.getPixel(0, 0));
    EXPECT_FALSE(im1.getPixel(0, width - 1) == im2.getPixel(0, width - 1));
    EXPECT_FALSE(im1.getPixel(height - 1, 0) == im2.getPixel(height - 1, 0));
    EXPECT_FALSE(im1.getPixel(height - 1, width - 1) == im2.getPixel(height - 1, width - 1));
}

TEST(TestAssignmentOperator, PRVD_chaining)
{
    int height = 9;
    int width = 8;
    // make a 72-pixel image with RGB = (5,6,7)
    Image im1(width, height, 5, 6, 7);
    Pixel pix(5, 6, 7);
    // set a few distinctive pixels
    im1.setPixel(0, 0, 255, 0, 0); // (0,0) is red
    im1.setPixel(4, 6, 0, 255, 0); // (4,6) is green
    im1.setPixel(5, 2, 0, 0, 255); // (5,2) is blue

    // make a 24-pixel all-black image
    Image im2(8, 3);

    // make a 15-pixel all-white image
    Image im3(3, 5, 255, 255, 255);

    // execute the chained assignment
    im3 = im2 = im1;

    // ASSERT correct dimensions before continuing on...
    ASSERT_EQ(im2.getHeight(), height);
    ASSERT_EQ(im3.getHeight(), height);
    ASSERT_EQ(im2.getWidth(), width);
    ASSERT_EQ(im3.getWidth(), width);

    // check some pixels in im2
    EXPECT_EQ(Pixel(255, 0, 0), im2.getPixel(0, 0));
    EXPECT_EQ(pix, im2.getPixel(height-1, width-1));
    EXPECT_EQ(pix, im2.getPixel(0, width-1));
    EXPECT_EQ(pix, im2.getPixel(height-1, 0));
    EXPECT_EQ(pix, im2.getPixel(3, 2));
    EXPECT_EQ(Pixel(0, 255, 0), im2.getPixel(4, 6));
    EXPECT_EQ(Pixel(0, 0, 255), im2.getPixel(5, 2));
    // check some pixels in im3
    EXPECT_EQ(Pixel(255, 0, 0), im3.getPixel(0, 0));
    EXPECT_EQ(pix, im3.getPixel(height-1, width-1));
    EXPECT_EQ(pix, im3.getPixel(0, width-1));
    EXPECT_EQ(pix, im3.getPixel(height-1, 0));
    EXPECT_EQ(pix, im3.getPixel(3, 2));
    EXPECT_EQ(Pixel(0, 255, 0), im3.getPixel(4, 6));
    EXPECT_EQ(Pixel(0, 0, 255), im3.getPixel(5, 2));
}

TEST(TestAssignmentOperator, STU_assignSmallerImageToBiggerImage)
{
    // STUDENT: please provide code for this test
    int height = 13;
    int width = 13;
    
    Image im1(width, height, 8, 2, 3);
    Pixel pix(8, 2, 3);
    // set a few distinctive pixels
    im1.setPixel(0, 0, 0, 0, 0); // (0,0) is black
    im1.setPixel(2, 8, 255, 255, 255); // (2,8) is white
    im1.setPixel(9, 10, 0, 0, 255); // (9,10) is blue

    // make a big image
    Image im2(20, 70);
    
    // execute the assignment
    im2 = im1;

    // ASSERT correct dimensions before continuing on...
    ASSERT_EQ(im2.getHeight(), height);
    ASSERT_EQ(im1.getHeight(), height);
    ASSERT_EQ(im2.getWidth(), width);
    ASSERT_EQ(im1.getWidth(), width);

    // check some pixels in im1
    EXPECT_EQ(Pixel(0, 0, 0), im2.getPixel(0, 0));
    EXPECT_EQ(pix, im2.getPixel(height-1, width-1));
    EXPECT_EQ(pix, im2.getPixel(0, width-1));
    EXPECT_EQ(pix, im2.getPixel(height-1, 0));
    EXPECT_EQ(pix, im2.getPixel(3, 2));
    EXPECT_EQ(Pixel(255, 255, 255), im2.getPixel(2, 8));
    EXPECT_EQ(Pixel(0, 0, 255), im2.getPixel(9, 10));
}

TEST(TestAssignmentOperator, STU_assignBiggerImageToSmallerImage)
{
    // STUDENT: please provide code for this test
    int height = 520;
    int width = 489;
    Image im1(width, height, 60, 99, 120);
    Pixel pix(60, 99, 120);
    // set a few distinctive pixels
    im1.setPixel(3, 5, 0, 0, 255); // (3,5) is blue
    im1.setPixel(1, 1, 255, 0, 0); // (1,1) is red
    im1.setPixel(5, 8, 0, 255, 0); // (5,8) is green

    Image im2(70, 90);
    // execute the assignment
    im2 = im1;

    // ASSERT correct dimensions before continuing on...
    ASSERT_EQ(im2.getHeight(), height);
    ASSERT_EQ(im1.getHeight(), height);
    ASSERT_EQ(im2.getWidth(), width);
    ASSERT_EQ(im1.getWidth(), width);

    // check some pixels in im2
    EXPECT_EQ(Pixel(0, 0, 255), im2.getPixel(3, 5));
    EXPECT_EQ(pix, im2.getPixel(height-1, width-1));
    EXPECT_EQ(pix, im2.getPixel(0, width-1));
    EXPECT_EQ(pix, im2.getPixel(height-1, 0));
    EXPECT_EQ(pix, im2.getPixel(3, 2));
    EXPECT_EQ(Pixel(255, 0, 0), im2.getPixel(1, 1));
    EXPECT_EQ(Pixel(0, 255, 0), im2.getPixel(5, 8));
}

}  // namespace

// main() for testing -- boilerplate
int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
