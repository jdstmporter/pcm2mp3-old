/*
 * DataChunk.hpp
 *
 *  Created on: 20 Dec 2017
 *      Author: julianporter
 */

#ifndef DATACHUNK_HPP_
#define DATACHUNK_HPP_

#include "base.hpp"
#include "Iterator32.hpp"
#include <map>
#include <algorithm>
#include "PCMFile.hpp"

namespace pylame {
namespace pcm {



	template<Endianness E>
	class DataChunk {
private:
	std::string ID;
	data_t data;
	
		
public:
	DataChunk() : ID(), data() {};
	DataChunk(const std::string &name,const data_t &data_) : ID(name), data(data_){};
	DataChunk(const DataChunk &) = default;
	virtual ~DataChunk() = default;
	DataChunk & operator=(const DataChunk &) = default;
	
	unsigned size() const { return data.size(); };
	Iterator32<E> iterator() const { return Iterator32<E>(data); };
	std::string kind() const { return ID; };
};

template<Endianness E>
	class DataForm {
	private:

		Iterator32<E> it;

		std::multimap<std::string,DataChunk<E>> chunks;
		unsigned len;

		bool nextChunk() {
			try {
				auto idx=it.nextString();
				auto n=it.nextInt();
				data_t d(n,0);
				it.getN(n,d.data());
				DataChunk<E> c(idx,d);
				chunks.insert(std::make_pair(idx,c));
				return true;
			}
			catch(...) {
				 return false;
			}
		};

	public:
		FileType fileType;
		DataForm() : it(), fileType(FileType::Other), chunks(), len() {};
		DataForm(const data_t &data) : it(data,E), fileType(FileType::Other), chunks(), len() {};
		DataForm(Iterator32<E> &ptr) : it(ptr), fileType(FileType::Other), chunks(), len() {};
		virtual ~DataForm() = default;

		void walk() {
			auto header=it.nextString();
			len=it.nextInt();
			auto type=it.nextString();
			std::cout << "Form header = '" << header << "'" << std::endl;
			std::cout << "Length = " <<  std::hex << len << std::dec << std::endl;
			std::cout << "Type string = '" << type << "'" << std::endl;
			if(header=="RIFF") {
				fileType=FileType::RIFF;
				if (type!="WAVE") throw MP3Error("WAVE tag not present in RIFF header");
			}
			else if(header=="FORM") {
				if(type=="AIFF") fileType=FileType::AIFF;
				else if(type=="AIFC") fileType=FileType::AIFC;
				else throw MP3Error("Unrecognised file type in FORM header");
			}
			else throw MP3Error("File is of unknown format");
			while(nextChunk()) {};
			std::for_each(chunks.begin(),chunks.end(),[](auto c) { std::cout << c.second << std::endl; });
		}

		bool isWAV() const { return fileType==FileType::RIFF; };
		bool isAIFC() const { return fileType==FileType::AIFC; };
		bool isAIFF() const { return fileType==FileType::AIFF; };
		unsigned size() const { return chunks.size(); };
		unsigned bytesInFile() const { return len; };
		bool has(const std::string &key) const { return chunks.count(key)>0; };
		std::vector<DataChunk<E>> operator[](const std::string &);

	};



	template<Endianness E>
	class ChunkProcessor {
	private:
		PCMFile::DataFormat format;
		unsigned nChannels;
		unsigned sampleRate;
		unsigned bytesPerSample;
		unsigned nSamples;	// per channel
		unsigned nBytesInFile;
		unsigned dataSize;
		unsigned bitsPerSample;
		unsigned offset;
		unsigned blocksize;
		DataForm<E> form;

		PCMFile::DataFormat convertFormat(const uint16_t value) {
			switch(value) {
				case 1:
				return PCMFile::DataFormat::PCM;
				case 3:
				return PCMFile::DataFormat::IEEEFloat;
				case 6:
				return PCMFile::DataFormat::ALaw;
				case 7:
				return PCMFile::DataFormat::ULaw;
				default:
				throw MP3Error("Unknown WAV data format");
			}
		};

		void junkChunk() {};

		void fmtChunk(const DataChunk<E> &chunk) {
			if(chunk.len!=16) throw MP3Error("Format block length wrong");
			auto itc=chunk.iterator();

			auto fmtCh=itc.nextPair();
			format=convertFormat(fmtCh.first);
			nChannels=fmtCh.second;

			sampleRate=itc.nextInt();
			auto byteRate=itc.nextInt();
			auto alignBits=itc.nextPair();
			bitsPerSample=alignBits.second;
			if((bitsPerSample&7) != 0) throw MP3Error("Bad bits per sample");
			bytesPerSample=bitsPerSample/8;
			if(alignBits.first!= bytesPerSample*nChannels) throw MP3Error("Block align check failed");
			if(byteRate != sampleRate*bytesPerSample*nChannels) throw MP3Error("Byte rate check failed");
	};

		void commChunk(const DataChunk<E> &chunk) {


		    auto itc=chunk.iterator();

		    auto p1=itc.nextPair();
		    auto p2=itc.nextPair();
		    nChannels=(unsigned)p1.first;
		    nSamples=swap(p1.second,p2.first);
		    bitsPerSample=(unsigned)p2.second;
		    sampleRate=(unsigned)itc.nextLongDouble();
		    if(form.isAIFC()) {
		    	auto f=itc.convertNext();
		    	char bytes[4];
		    	std::locale l;
		    	for(auto i=0;i<4;i++) bytes[i]=std::tolower(f.bytes[i],l);
		    	std::string fmt(bytes);
		    	if(fmt=="none" || fmt=="sowt") format=PCMFile::DataFormat::PCM;
		    	else if(fmt=="fl32" || fmt=="fl64") format=PCMFile::DataFormat::IEEEFloat;
		    	else if(fmt=="ulaw") format=PCMFile::DataFormat::ULaw;
		    	else if(fmt=="alaw") format=PCMFile::DataFormat::ALaw;
		    	else MP3Error("Unknown AIFC data format");
		    }
		    else format=PCMFile::DataFormat::PCM;
		}

		void dataChunk(const DataChunk<E> &chunk) {
				};

		void soundChunk(const DataChunk<E> &chunk) {
			 dataSize=chunk.size()-8;
			 auto itc=chunk.iterator();
			 auto offset=itc.nextInt();
			 auto block=itc.nextInt();
			 if(itc.size()!=dataSize) throw MP3Error("Data size error");
		};



		PCMData bytes() {
			if(format!=PCMFile::DataFormat::PCM) throw MP3Error("Can only enumerate PCM files");
			if(offset!=0 || blocksize!=0) throw MP3Error("Cannot enumerate blocked data sets");
			auto qr=std::div(bitsPerSample,8);
			if(qr.rem!=0) throw MP3Error("Cannot enumerate non-integral byte samples");

			DataChunk<E> ssnd=(form.isWAV()) ? form["DATA"] : form["SSND"];
			auto it=ssnd.iterator();
			it.skip(2);
			return PCMData(nChannels,nSamples,it);
		}
	};




} /* namespace pcm */
} /* namespace pylame */

template<pylame::Endianness E>
std::ostream & operator<<(std::ostream &o,const pylame::pcm::DataChunk<E> &c) {
	o << "Chunk of type '" << c.kind() << "' and length " << c.size();
	return o;
}

#endif /* DATACHUNK_HPP_ */
