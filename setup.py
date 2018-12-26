from setuptools import setup, Extension
import distutils.sysconfig
from Cython.Build import cythonize

setup(
    name='dithtorio',
    version='1.0.0',
    packages=['dithtorio'],
    entry_points={
    	'console_scripts': [
    		'dithtorio=dithtorio.console_script:main'
    	]
    },
    ext_modules = cythonize([Extension(
            'libdithtorio',
            	[
	            	'libdithtorio.pyx', 
	            	'dithering.cpp',
	            	'blueprint.cpp'
            	],
            depends=[
	            	'dithering.h',
	            	'blueprint.h',
	            	'compression.h',
	            	'base64.h'
            ],
            #define_macros=[('DEBUG','0')],
            include_dirs=['/usr/include', '/usr/local/include', '/usr/local/include/opencv4'],
            library_dirs=['/usr/lib', '/usr/local/lib'],
            libraries=['opencv_core', 'opencv_imgcodecs', 'opencv_imgproc'],
            language='c++',
    )]),
)