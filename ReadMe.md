# CleanImage

**CleanImage** is a cross-platform image cleaning and random EXIF ​​data adding application built using C++, OpenCV, Exiv2 and libzip. The project supports compilation on Windows, Linux and macOS.

## Features
- Image uniqler
- Remove EXIF ​​metadata from images
- Generate random EXIF ​​data (including GPS coordinates, shooting time, camera settings, etc.)
- Archive results into a ZIP file

## Dependencies

To successfully build the project, you need:

- CMake 3.14 or later
- C++20 compiler
- OpenCV for image processing
- libzip for working with ZIP files
- Exiv2 for working with EXIF ​​data
- INIReader for reading configurations (inih library)

### Installing dependencies

#### Windows

1. Install `vcpkg` and add it to PATH.
2. Install the necessary packages:

```bash
vcpkg install opencv libzip exiv2
```
#### Linux
1. Install packages via a package manager (e.g. apt for Ubuntu):
```bash
sudo apt update
sudo apt install libopencv-dev libzip-dev libexiv2-dev cmake g++
```
#### MacBook
1. Install Homebrew for package management.
2. Install packages:
```bash
brew install opencv libzip exiv2 cmake
```
### Build the project
1. Clone the repository:
```bash
git clone https://github.com/your_profile/CleanImage.git
cd CleanImage
```
2. Set up the project with CMake:
```bash
mkdir build
cd build
cmake ..
```
3. Build the project:
```bash
cmake --build .
```
##### Note: On macOS, specify the path to OpenCV in CMakeLists.txt via set(OpenCV_DIR "path_to_vcpkg/installed/arm64-osx/share/opencv4").
4. Usage
   After a successful build, run CleanImage to perform actions:
```bash
./CleanImage
```
### Usage example
Run the program and select an action (remove metadata, add random EXIF ​​data, etc.).
Enter the path to the image you want to work with.
Select the number of images and confirm.
The results will be saved in a ZIP archive in the specified directory.

### Project structure
- src/ — source code:
- main.cpp — the main file for running the program.
- image_processing.cpp — functions for processing images.
- exif_operations.cpp — functions for working with EXIF ​​data.
- zip_operations.cpp — functions for creating a ZIP archive.
- include/ — header files.
- CMakeLists.txt — a file for configuring the build.
## Support
If you have any problems or suggestions, please open an issue in the repository or submit a pull request.