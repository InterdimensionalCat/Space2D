# Space2D
Space2D is a lightweight, templated library for simple but high functionality representations of various 2-dimensional mathematical data structures
## Features
* Space2D has implementations of Points, Vectors, Normalized Vectors, 2 Dimensional sizes, Rectangles, Polygons, and 3x3 Matricies
* All of these primatives can be transformed using the wide veriety of supported linear transformations through Matricies. Additionally, custom transformations can be applied using direct access to these Matricies
* Strongly typed angular types (Radians, Degrees, Percent of unit circle) that can be implicitly converted between eachother, allowing for syntax such as this:
    
       auto cosval = s2d::cos(45_deg) //implicitly converts 45 degrees to pi/4 radians before performing the cos operation
* Strongly typed linear types (Pixels and Meters) that can be implicitly converted, just like the angular types. The conversion ratio of meters to pixels can be set by the end user, though it defaults to 1 Meter - 64 Pixels
* Intutive literal definitions defined for all angular and linear types (1_px, 1_mtr, 45_deg, 3_rad, 2_pirad [pi-radians], 50_pcent)
* Fully documented using [Doxygen](https://www.doxygen.nl/index.html)
* Comprehensive unit tests using [GoogleTest](https://github.com/google/googletest)
