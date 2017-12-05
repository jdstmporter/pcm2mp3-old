'''
Created on 2 Dec 2017

@author: julianporter
'''
from wav2mp3 import _wav2mp3
from wav2mp3 import rates
from wav2mp3 import quality

def transcode(*args,**kwargs):
    fn = None
    if len(args)==1 :
        fn=_wav2mp3.transcodeS
    else:
        fn=_wav2mp3.transcodeF
    return fn(*args,**kwargs)


