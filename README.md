## Requirements
OpenCV 4.0.0 (might work with other versions, not tested though)  
`sudo apt-get install libopencv-core-dev libopencv-imgproc-dev libopencv-imgcodecs-dev` or build it from source

ZLib  
`sudo apt-get install zlib1g-dev`

RapidJSON  
`sudo apt-get install rapidjson-dev`

## Build
- cmake: `mkdir build && cd build && cmake .. && make`
- python/cython lib: `python setup.py install`

## CLI usage
Works with C++ executable or python entrypoint script from setuptools

`dithtorio image.png`  
gives `converted_image.png` and `blueprint_image.txt`
