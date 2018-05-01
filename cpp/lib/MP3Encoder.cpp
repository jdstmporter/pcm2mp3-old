/*
 * MP3Encoder.cpp
 *
 *  Created on: 30 Nov 2017
 *      Author: julianporter
 */

#include "MP3Encoder.hpp"
#include "PCMData.hpp"
#include <algorithm>

namespace pylame { namespace mp3 {

const std::map<std::string,ID3Tag> MP3Parameters::id3names = {
		{ "title" , ID3Tag::Title },
		{ "artist" , ID3Tag::Artist },
		{ "album" , ID3Tag::Album },
		{ "year" , ID3Tag::Year },
		{ "comment" , ID3Tag::Comment },
		{ "track" , ID3Tag::Track },
		{ "genre" , ID3Tag::Genre },
};

void MP3Parameters::write(lame_global_flags *gf) const {
	lame_set_copyright(gf,copyright);
	lame_set_original(gf,original);

	id3tag_init(gf);
	switch(versions) {
	case ID3Versions::OneOnly:
		id3tag_v1_only(gf);
		break;
	case ID3Versions::TwoOnly:
			id3tag_v2_only(gf);
			id3tag_pad_v2(gf);
			break;
	case ID3Versions::OneAndTwo:
			id3tag_add_v2(gf);
			id3tag_pad_v2(gf);
			break;
	}
	for(auto it=id3.begin();it!=id3.end();it++) {
		auto tag=it->first;
		auto value=it->second;
		switch(tag) {
		case ID3Tag::Title:
			id3tag_set_title(gf,value.c_str());
			break;
		case ID3Tag::Artist:
					id3tag_set_artist(gf,value.c_str());
					break;
		case ID3Tag::Album:
					id3tag_set_album(gf,value.c_str());
					break;
		case ID3Tag::Year:
					id3tag_set_year(gf,value.c_str());
					break;
		case ID3Tag::Comment:
					id3tag_set_comment(gf,value.c_str());
					break;
		case ID3Tag::Track:
					id3tag_set_track(gf,value.c_str());
					break;
		case ID3Tag::Genre:
					id3tag_set_genre(gf,value.c_str());
					break;
		}
	}
}

unsigned MP3Encoder::mp3SizeCalc(unsigned n) {
	return unsigned((double)n*1.25+7200.0);
}


MP3Encoder::MP3Encoder(const pcm::file_t &data_,const unsigned quality,const unsigned rate) : MP3Encoder(data_,MP3Parameters(quality,rate)) {};


MP3Encoder::MP3Encoder(const pcm::file_t &data_,const MP3Parameters &parameters):
	data(data_), nSamples(data->samplesPerChannel()), mp3Size(MP3Encoder::mp3SizeCalc(nSamples)), output(mp3Size,0) {
		
	gf = lame_init();
	if(gf==nullptr) throw MP3Error("Cannot initialise LAME transcoder");
	lame_set_num_channels(gf,data->nChans());
	lame_set_in_samplerate(gf,data->samplesPerSecond());
	lame_set_brate(gf,parameters.Rate());
	lame_set_mode(gf,data->mp3Mode());
	lame_set_quality(gf,parameters.Quality());
	
	parameters.write(gf);
	auto response=lame_init_params(gf);
	if(response<0) throw MP3Error("Cannot initialise LAME transcoder options");

	//mp3Out = new unsigned char[mp3Size];

}
	
MP3Encoder::~MP3Encoder() {
	lame_close(gf);
	//delete[] mp3Out;
}	
	
void MP3Encoder::transcode() {

	try {

		auto d=data->bytes();
		mp3Out=output.data();
		int status=0;
		switch(d.format) {
		case pylame::SampleFormat::Int16: {
			pylame::pcm::Channels<int16_t> channels=d.channels<int16_t>();
			status=lame_encode_buffer(gf,channels.left.get(),channels.right.get(),nSamples,mp3Out,mp3Size);
			break; }
		case pylame::SampleFormat::Int32: {
			pylame::pcm::Channels<int32_t> channels=d.channels<int32_t>();
			status=lame_encode_buffer_int(gf,channels.left.get(),channels.right.get(),nSamples,mp3Out,mp3Size);
			break; }
		case pylame::SampleFormat::Float32: {
			pylame::pcm::Channels<float> channels=d.channels<float>();
			status=lame_encode_buffer_ieee_float(gf,channels.left.get(),channels.right.get(),nSamples,mp3Out,mp3Size);
			break; }
		default:
			throw MP3Error("Unacceptable sample type");
		}



		if(status<0) {
			switch(status) {
			case -1:
				throw MP3Error("Transcoding failed: insufficient memory allocated for transcoding");
				break;
			case -2:
				throw MP3Error("Transcoding failed: memory fault");
				break;
			case -3:
				throw MP3Error("Transcoding failed: liblamemp3 subsystem not initialised");
				break;
			case -4:
				throw MP3Error("Transcoding failed: psycho-acoustic problem");
				break;
			default:
				throw MP3Error("Transcoding failed");
				break;
			}
		}
		auto extra=lame_encode_flush(gf,mp3Out,status);

		mp3Size=status+extra;
		output.resize(status+extra);
	}
	catch(MP3Error &e) { throw e; }
	catch(std::exception &e) {
		throw MP3Error("Transcoding failed");
	}
}

}}

std::ostream & operator<<(std::ostream &o,const pylame::mp3::MP3Encoder &e) {
	const unsigned char *u=e.ptr();
	const char *d=reinterpret_cast<const char *>(u);
	o.write(d,e.size());
	return o;
}
     
