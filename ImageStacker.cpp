// image_stacker.cpp
// CS 223, Winter 2019, Lab 3
// Name: Lauren Dickman
// Date: 2/14/2020

/*
Specifications for the image stacker program:
- Ask the user for the directory containing the images to stack.
- Ask the user for what to call the final good image file.
- Average all noisy images in the given directory to create a final good image.
- Write the final good image in PPM format to the filename given by the user.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include "Pixel.h"
#include "Image.h"

using namespace std;
using namespace imagelab;

// readDir -- PROVIDED CODE
// puts filenames from a directory into an output stream
// Explanation of the parameters:
//   out - output stream (may be a stringstream)
//   dir - string containing the path to the directory to read
//   n - number of filenames to return
// Preconditions:
//   dir is a valid directory
//   0 <= n <= the number of files in dir
// Postcondition:
//   output stream `out` has the filenames inserted (space-separated)
void readDir(ostream& out, string dir, int n)
{
   string fname, filepath;
   DIR *dp;
   struct dirent *dirp;

   cout << "dir to get files of: " << dir << endl;
   dp = opendir( dir.c_str() );
   if (dp == NULL)
   {
      cout << "ERROR_OPENING:" << dir << endl;
   }

   int i = 0;
   while ((dirp = readdir( dp )) && i < n)
   {
      fname = dirp->d_name;

      if ((fname.length() >= 3) &&
      (fname.substr(fname.length()-3, 3) == "ppm"))
      {
         filepath = dir + "/" + fname;
         out << filepath << " ";
         i++;
      }
   }
   closedir( dp );
}

// HELPER FUNCTIONS
// nameDir- Ask the user for the directory containing the images to stack.
string nameDir()
{
   string dir_name;
   cout << "Please enter the name of the directory containing the images: ";
   cin >> dir_name;
   return dir_name;
}

// numberDir- Ask the user the number of files in the directory containing the images to stack.
int numberDir()
{
   int dir_number;
   cout << "Please enter the number of how many images to stack (up to 10): ";
   cin >> dir_number;
   return dir_number;
}

// nameFile - Ask the user for what to call the final good image file.
string nameFile()
{
   string new_dir_name;
   cout << "Please enter the new name of the file containing the images: ";
   cin >> new_dir_name;
   return new_dir_name;
}

//Makes an arrray of all the images in the files
Image* get_image_arry(int num_images, string input_file)
{
   stringstream files;
   string file_name;
   Image* image_arr = new Image[num_images]; 
   readDir(files, input_file, num_images);
   for (int i = 0; i < num_images; i++)
   {
      files >> file_name;
      ifstream imgfile(file_name.c_str());
      Image image(imgfile);
      image_arr[i] = image;
   }
   return image_arr;
}


// Average_Noisy_images - Average all noisy images in the given directory to create a final good image.
Image Average_Noisy_images_to_Final_Image(int num_images, string input_file)
{
   int av_r, av_b, av_g;   // Average colors (red, green, blue) start off with 0
   Image* All_Images = get_image_arry(num_images, input_file);
   Image One_Image(All_Images[0].getWidth(), All_Images[0].getHeight());
   Pixel pix;
   for (int row = 0; row < All_Images[0].getHeight(); row++) //im1[0].getHeight()
   {
      for (int col = 0; col < All_Images[0].getWidth(); col++)
      {
         av_r = 0;
         av_b = 0;
         av_g = 0;
         for (int i = 0; i < num_images; i++)
         {
            pix = All_Images[i].getPixel(row, col);
            av_r = av_r + pix.getRed();
            av_g = av_g + pix.getGreen();
            av_b = av_b + pix.getBlue();
         }
         av_g = av_g / num_images;
         av_r = av_r / num_images;
         av_b = av_b / num_images;
         One_Image.setPixel(row, col, av_r, av_g, av_b);
      }
   }
   delete [] All_Images;
   All_Images = NULL; 
   return One_Image;
}


int main()
{
   int num_images = numberDir();
   string input_file = nameDir();
   string new_file_name = nameFile();
   ofstream finalfile;
   finalfile.open(new_file_name);
   finalfile << Average_Noisy_images_to_Final_Image(num_images, input_file);
   finalfile.close();
}
