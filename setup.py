#!/usr/bin/python3
'''
Created on 1 Dec 2017

@author: julianporter
'''
from setuptools import setup, Extension
from setuptools.config import read_configuration

configuration=read_configuration('setup.cfg')
print(configuration)
metadata=configuration['metadata']

package=metadata['name']

libsrc=['MP3Encoder.cpp','PCMFile.cpp', 'WAVFile.cpp', 'enums.cpp', 'Iterator32.cpp']
wsrc=['lib/'+s for s in libsrc]
wsrc.append('Lame.cpp')
qsrc=['Member.cpp','Quality.cpp']
rsrc=['Member.cpp','Rates.cpp']
version=metadata['version']

def makeExtension(module,src):
    majorV,minorV = version.split('.')
    return Extension(package+'.'+module,
                    define_macros = [('MAJOR_VERSION', majorV),
                                     ('MINOR_VERSION', minorV)],
                    sources = ['cpp/'+s for s in src],
                    include_dirs=['/usr/include'],
                    libraries = ['mp3lame'],
                    library_dirs = ['/usr/lib/x86_64-linux-gnu'])

coder = makeExtension('_wav2mp3',wsrc)
rates = makeExtension('rates',rsrc)
quality = makeExtension('quality',qsrc)

setup (
    entry_points = {
        'distutils.commands' : [
           'cleaner = clean:Cleaner' 
           ]
        },
    ext_modules = [coder,rates,quality],
    
    )
