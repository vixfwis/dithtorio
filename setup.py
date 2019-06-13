from setuptools import setup, Extension
import distutils.sysconfig
from Cython.Build import cythonize

setup(
    name='dithtorio',
    version='1.0.1',
    packages=['dithtorio'],
    install_requires=[
        'Click',
    ],
    entry_points={
    	'console_scripts': [
    		'dithtorio=dithtorio.console_script:main'
    	]
    },
    ext_modules = cythonize([Extension(
            'libdithtorio',
            	[
	            	'libdithtorio.pyx', 
	            	'dithtorio.cpp',
	            	'blueprint.cpp'
            	],
            depends=[
	            	'dithtorio.h',
	            	'compression.h'
            ],
            #define_macros=[('DEBUG','0')],
            include_dirs=['/usr/include', '/usr/local/include', '/usr/local/include/opencv4'],
            library_dirs=['/usr/lib', '/usr/local/lib'],
            libraries=['opencv_core', 'opencv_imgcodecs', 'opencv_imgproc'],
            language='c++',
    )]),
)