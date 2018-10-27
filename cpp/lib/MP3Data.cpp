/*
 * MP3Data.cpp
 *
 *  Created on: 2 May 2018
 *      Author: julianporter
 */

#include "MP3Data.hpp"
#include <chrono>
#include <ctime>

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

bool MP3Parameters::has(const ID3Tag tag) const {
	return id3.find(tag)!=id3.end();
}

MP3Parameters::MP3Parameters(const unsigned q,const unsigned r) :
		id3(), versions(ID3Versions::OneAndTwo),
		copyright(false), original(false), quality(q), rate(r) {

	try {
		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto tStruct=localtime(&now);
		std::stringstream s;
		s << tStruct->tm_year;
		id3[ID3Tag::Year]=s.str();
	}
	catch(...) {};
	id3[ID3Tag::Comment]="Converted by pcm2mp3 (c) JP Embedded Solutions 2018)";
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

}}


