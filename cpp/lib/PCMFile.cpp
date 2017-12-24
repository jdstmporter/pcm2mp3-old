/*
 * PCMFile.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#include "PCMFile.hpp"

using namespace pylame::pcm;

PCMData::PCMData(const unsigned nChannels_,const unsigned nSamples_,Iterator32 &it) {
			nChannels=nChannels_;
			nSamples=nSamples_;
			if(nChannels<1 || nChannels>2) throw MP3Error("Invalid number of channels");

			short *lBuffer=new short[nSamples];
			short *rBuffer=new short[nSamples];

			unsigned index=0;
			if(nChannels==1) {
				while(!it.finished()) {
					try {
						auto d=it.next<pair_t>();
						lBuffer[index]=(short)d.first;
						rBuffer[index]=0;
						index++;
						rBuffer[index]=(short)d.second;
						rBuffer[index]=0;
						index++;
					}
					catch(...) {
						lBuffer[index]=0;
						rBuffer[index]=0;
						index++;
					}
				}
			} else { 	/// Stereo
				while(!it.finished()) {
					try {
						auto d=it.next<pair_t>();
						lBuffer[index]=(short)d.first;
						rBuffer[index]=(short)d.second;
						index++;
					}
					catch(...) {
						lBuffer[index]=0;
						rBuffer[index]=0;
						index++;
					}
				}
			}
			left=std::shared_ptr<short>(lBuffer);
			right=std::shared_ptr<short>(rBuffer);
		}

PCMFile::PCMFile(std::istream &stream) : format(DataFormat::PCM), file(), form() {
	stream.seekg (0, stream.end);
	auto length = stream.tellg();
	stream.seekg (0, stream.beg);

	file.assign(length,0);
	auto c=file.data();
	int pos=0;
	while(pos<length) {
	   stream.read(c+pos,1024);
	   pos+=stream.gcount();
	}
}




void PCMFile::parse(Iterator32 &it,const std::string &info,const std::string &sound) {
	form=Form(it);
	metadata=form.typeCheck();

	if(!metadata.verify(FormHeader(),FormTypes())) throw MP3Error("Not Valid file type");
	nBytesInFile=metadata.length;
	form.walk();

	if(!form.hasOne(info)) throw MP3Error("File has anomalous info chunk count");
	auto ic=form[info];
	infoChunk(ic);

	if(!form.hasOne(sound)) throw MP3Error("File has anomalous sound chunk count");
	auto sc=form[sound];
	soundChunk(sc);
};


std::ostream & operator<<(std::ostream &o,const pylame::pcm::PCMFile &w)  {
	o << "Size             " << w.size() << std::endl;
	o << "N Channels       " << w.nChans()  << std::endl;
	o << "Sample rate      " << w.samplesPerSecond() << std::endl;
	o << "Bits per sample  " << w.sampleSize() << std::endl;
	o << "Bytes per sample " << w.sampleSizeInBytes() << std::endl;
	o << "Data size        " << w.dSize() << std::endl;
	o << "N Samples        " << w.samplesPerChannel() << std::endl;
	return o;
}
