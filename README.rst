==================================
Package documentation for wav2mp3
==================================

A very simple module that converts standard WAV audio files to compressed MP3 files, given control over the quality of the resulting
file and the quality of the conversion process.  It is basically a lightweight wrapper around ``libmp3lame``, hiding much of the 
complexity (and the advanced functionality).  The purpose of the exercise is to be *quick* and *simple*.

Functions
---------

The module exposes one function:

wav2mp3. **transcode** (*infile*, *outfile*, *bitrate=64*, *quality=5*)

Arguments

  :infile:     
    The name of the input WAV file, *with* its file extension.  E.g. 'audio.wav'.
    
    It must be the name of a standard WAV file, containing mono or stereo 16 bit linear PCM encoded audio data.
    If it is not, then an exception is thrown.

  :outfile:    
    The name to assign to the output MP3 file, *with* its file extension.  E.g. 'audio.mp3'.
    It has the same number of channels as *infile*.
    

  :bitrate:    
    The target bitrate (in kilobits per second) of the MP3 transcoded data.
    It must be one of the standard MP3 bitrates (expressed in kilobits per second).

    Common values include 128, 96, 64, 48, 24 and 8.  For more detail on supported rates, 
    check out MP3 documentation, e.g. at https://en.wikipedia.org/wiki/MP3#Bit_rate.   

    Optional: defaults to 64 (i.e. 64 kpbs).

  :quality:    
    A factor measuring the quality of the transcoding; ranges from 1 (best possible quality; very slow), 
    to 7 (acceptable quality; very fast).  
    
    Optional: defaults to 5 (high quality; quite fast).

Return value
  The size of *outfile* in bytes

Exceptions
  Throws a wav2mp3.**MP3Error** (see below) whenever ``libmp3lame`` throws an exception, or else the data doesn't do what the module expects (see below).
    

Classes
-------

wav2mp3.**MP3Error**

A child of the standard **Exception** class, reporting on errors occurring during transcoding.  Opaque.

Example
-------

::

  import wav2mp3
  
  try:
      wav2mp3.transcode("input.wav","output.mp3",bitrate=8)
  except wav2mp3.MP3Error as e:
      print(str(e))


  
    
