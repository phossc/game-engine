# Third party libraries

## bullet
- URL: https://github.com/bulletphysics/bullet3
- Version: 8bc1c8e01b1b2b9284df08385da0e03241f4e6aa (commit)
- License: zlib

All files from the src subdirectory excluding CMakeLists.txt and premake4.lua
files. Added a custom CMakeLists.txt file that builds the Bullet2 part of the
library.

## Catch2
- URL: https://github.com/catchorg/Catch2
- Version: v2.5.0 (tag)
- License: BSL-1.0

## glad
- URL: https://github.com/Dav1dde/glad
- Version: v0.1.28 (tag)
- License: MIT

Used through the webservice to generate glad files for OpenGL 3.3 Core.

## glm
- URL: https://github.com/g-truc/glm
- Version: v0.9.9.3 (tag)
- License: Happy Bunny License (Modified MIT) or MIT

Only included glm subdirectory and deleted CMakeLists.txt within it. Also added
a file containing the license.

```glm/detail/setup.hpp``` was modified to make glm never use language
extensions.

## GLFW
- URL: https://github.com/glfw/glfw
- Version: f9923e90958e726aaabc86d83fb3681216d76067 (commit)
- License: zlib/libpng

Removed dotfiles in the root directory.

## spdlog
- URL: https://github.com/gabime/spdlog
- Version: v1.2.1 (tag)
- License: MIT

Included files are the include directory and the LICENSE file. Uses a custom
CMakeLists.txt file.

## strong_typedef
- URL: https://github.com/anthonywilliams/strong_typedef
- Version: 7bdfadda96b88525e2b61a6b24c4db6a77083fb1 (commit)
- License: Boost Software License

Included files are the `README.md` and `strong_typedef.hpp`. Uses a custom
CMakeLists.txt file.