#!/usr/bin/python3
'''
Created on 1 Dec 2017

@author: julianporter
'''
from setuptools import setup, Extension
from setuptools.config import read_configuration
import utils
from collections import namedtuple
from sys import exit


checker=utils.CheckCompiler('-std=c++14')
if not checker.run():
    print("Cannot build pcm2mp3 unless compiler supports -std=c++14")
    exit(1)

checker=utils.CheckLibrary('mp3lame')
if not checker.run():
    print("Cannot build pcm2mp3 unless libmp3lame is installed and on the compiler path")
    exit(1)

configuration=read_configuration('setup.cfg')
metadata=configuration['metadata']

wsrc=[]

libsrc=['MP3Encoder.cpp', 'MP3Data.cpp','MP3File.cpp', 'PCMData.cpp','PCMFile.cpp', 'WAVFile.cpp','AIFFFile.cpp', 'Iterator32.cpp',
        'DataChunk.cpp', 'Form.cpp', 'base.cpp', 'Conversions.cpp', 'transcoder.cpp']
tstsrc=['MP3.cpp','MP3File.cpp','MP3Frame.cpp','Test.cpp']
wsrc.extend([f'lib/{s}' for s in libsrc])
wsrc.extend([f'test/{s}' for s in tstsrc])
wsrc.extend(['MP3TestObject.cpp','PCMObject.cpp','MP3Object.cpp','Lame.cpp','PyHeader.cpp'])

Version = namedtuple('Version',['major','minor','maintenance'])
def processVersion():
    version=metadata['version']
    parts=version.split('.')
    if len(parts)<3: parts.extend([0,0,0])
    return Version(*(parts[:3]))

def makeExtension(module,src):
    #print("Making {} with {}".format(module,src))
    
    v=processVersion()
    mv=f'"{v.major}.{v.minor}.{v.maintenance}"'
    return Extension(module,
                    define_macros = [('MAJOR_VERSION', v.major),
                                     ('MINOR_VERSION', v.minor),
                                     ('MAINTENANCE_VERSION', v.maintenance),
                                     ('MODULE_VERSION', mv)],
                    sources = ['cpp/'+s for s in src],
                    language = 'c++',
                    include_dirs=['/usr/include'],
                    libraries = ['mp3lame'],
                    library_dirs = ['/usr/lib/x86_64-linux-gnu'])

coder = makeExtension('pcm2mp3',wsrc)

with open('README.rst') as readme:
    longDescription = readme.read()

setup (
    cmdclass = {'cleaner' : utils.Cleaner },
    ext_modules = [coder],
    long_description = longDescription 
)
