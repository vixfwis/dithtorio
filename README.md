## Requirements
OpenCV 4  
[Build instructions](https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html)

ZLib  
`sudo apt-get install zlib1g-dev`

RapidJSON  
`sudo apt-get install rapidjson-dev`

Cython (for Python module build)  
`pip install cython`

## Build
- Python module: `python setup.py install`

## CLI usage
`dithtorio image.png`  
gives `converted_image.png` and `blueprint_image.txt`. See `dithtorio --help` for more details.
