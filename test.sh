#!/bin/bash

#rm -fr build
#rm -fr dist
#rm -fr pcm2mp3.egg-info

./setup.py build
./setup.py bdist
sudo rm -fr /usr/local/lib/python3.6/dist-packages/pcm2mp3*
pushd /
sudo tar xzf /opt/projects/singlr/pcm2mp3/dist/pcm2mp3-1.8.linux-x86_64.tar.gz
popd


mkdir -p $HOME/ptest
cp test.wav $HOME/ptest

pushd $HOME/ptest
cat <<! > t.py
import pcm2mp3
val=pcm2mp3.transcodeF('test.wav','out.mp3')
print('output was {}'.format(val))
!

python3 t.py
popd
rm -fr $HOME/ptest



