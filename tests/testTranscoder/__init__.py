from .test import Converter

INFILE = 'test'

def testConversion(): 
    for mode in ['transcode','classBased']:
        for bitrate in [16,32,64]:
            yield Converter(INFILE,mode,quality=5,bitrate=bitrate) 