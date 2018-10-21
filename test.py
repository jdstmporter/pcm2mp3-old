#!/usr/bin/python3
'''
Created on 3 May 2018

@author: julianporter
'''

import pcm2mp3
import traceback
import sys
import re




if __name__ == '__main__':
    TRANSCODE=re.match('^[tT]',sys.argv[1]) if len(sys.argv)>1 else 0

    try:
        
        if TRANSCODE:
            print('Using transcode')
            with open('test.wav','rb') as infile:
                print('Starting transcoding')
                out=pcm2mp3.transcode(infile.read(),quality=5,bitrate=16)
                print('Transcoding completed')
                with open('test.mp3','wb') as outfile:
                    outfile.write(out)
        else:
            print('Using MP3 Object')
            with open('test.wav','rb') as infile:
                pcm=pcm2mp3.PCM(infile.read())
                mp3=pcm2mp3.MP3(pcm,quality=5,rate=16)
                with open('test.mp3','wb') as outfile:
                    outfile.write(mp3.data())
        print('Success')
            
    except:
        ety,ev,etr= sys.exc_info()
        print(f"There was an error : {ety}, {ev}")
        stack=traceback.extract_tb(etr)
        for line in stack:
            print(f'{line.filename} : {line.lineno} - {line.name} : {line.line}')
            
    finally:
        print('Completion')
  
        

        
        