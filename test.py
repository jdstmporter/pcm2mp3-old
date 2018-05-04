#!/usr/bin/python3
'''
Created on 3 May 2018

@author: julianporter
'''

import pcm2mp3
import traceback

if __name__ == '__main__':
    
    try:
    
        with open('test.wav','rb') as infile:
            data=infile.read()
            pcm=pcm2mp3.PCM(data)
            mp3=pcm2mp3.MP3(pcm,quality=5,rate=16)
        
            with open('test.mp3','wb') as outfile:
                outfile.write(mp3.data());
            
    except:
        traceback.print_exc()
        

        
        