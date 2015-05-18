# Object-Tracking

The program detects red circumferences in an input image and provides information related to this.  In addition it provides “tracking” services by taking a picture every 30ms and outputting the result. Once a particular image is stored in memory, its RGB space is transformed into an 8-bit color depth image. This is based on the red component of the input image. After this, a morphological operation (dilation and erosion) and a Gaussian blur are performed on the threshold image to reduce noise and eliminate small objects from the background (feature detector). Then, a hough transformation is performed on the threshold image to extract any found circles. This function uses a two-pass algorithm for detecting circles (acts a modebase). Finally, inner and outer circles are drawn on the input image. 

The program was developed using C++ and OpenCV 2.4.10.
