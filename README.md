## Requirements
OpenCV 4.0.0 (might work with other versions, not tested though)  
`sudo apt-get install libopencv-core-dev libopencv-imgproc-dev libopencv-imgcodecs-dev` or build it from source

ZLib  
`sudo apt-get install zlib1g-dev`

RapidJSON  
`sudo apt-get install rapidjson-dev`

Cython (for Python module build)  
`pip install cython`

## Build
- cmake: `mkdir build && cd build && cmake .. && make`  
Note: cmake build has a very basic CLI controls and mostly used for development.
- Python module: `python setup.py install`

## CLI usage
Assuming Python module build:  
`dithtorio image.png`  
gives `converted_image.png` and `blueprint_image.txt`. See `dithtorio --help` for more details.
