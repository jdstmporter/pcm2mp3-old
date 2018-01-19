/*
 * PCXMData.hpp
 *
 *  Created on: 16 Jan 2018
 *      Author: julianporter
 */

#ifndef PCMDATA_HPP_
#define PCMDATA_HPP_

#include "base.hpp"
#include "Iterator32.hpp"
#include <utility>
#include <type_traits>
#include <cfloat>

namespace pylame { namespace pcm {



template <typename T>
	struct Channels {
		int nChannels;
		int nSamples;
		bool boost;
		std::shared_ptr<T> left;
		std::shared_ptr<T> right;

		Channels() : nChannels(1), nSamples(0), boost(false), left(), right() {};
		Channels(std::shared_ptr<T> left_,std::shared_ptr<T> right_,int nChannels_,int nSamples_, bool boost_) : nChannels(nChannels_), nSamples(nSamples_), boost(boost_), left(left_), right(right_) {};
		Channels(T* left_, T* right_,int nChannels_,int nSamples_, bool boost_) : nChannels(nChannels_), nSamples(nSamples_), boost(boost_) {
			if(std::is_floating_point<T>::value) {
				normaliseArray((float *)left_);
				if(nChannels>1) normaliseArray((float *)right_);
				left=std::shared_ptr<T>(left_);
				right=std::shared_ptr<T>(right_);
			}
		};
		virtual ~Channels() = default;

		Channels<float> asFloat() {
			float *lBuffer=new float[nSamples];
			float *rBuffer=new float[nSamples];

			for(unsigned i=0;i<nSamples;i++) {
				lBuffer[i]=(float)left.get()[i];
				rBuffer[i]=(float)right.get()[i];
			}
			return Channels<float>(lBuffer,rBuffer,nChannels,nSamples,boost);
		};

		void normaliseArray(float * ptr) {
			float maxScale = boost ? FLT_MAX : 1.0;
			auto bounds=std::minmax_element(ptr,ptr+nSamples);
			auto min=*bounds.first;
			auto max=*bounds.second;
			float bound=std::max(fabs(min),fabs(max));
			if(bound==0.0) return;
			auto scale=std::min(maxScale,1.0f/bound);
			for(unsigned i=0;i<nSamples;i++) ptr[i]*=scale;
		}


	};

struct PCMData {
	pylame::SampleFormat format;
	unsigned nChannels;
	unsigned nSamples;
	Iterator32 it;

	PCMData() : format(pylame::SampleFormat::Unknown), nChannels(), nSamples(), it() {};
	PCMData(const pylame::SampleFormat &format_, const unsigned nChannels_,const unsigned nSamples_,Iterator32 &it_) : format(format_), nChannels(nChannels_), nSamples(nSamples_), it(it_) {};
	virtual ~PCMData() = default;



	template<typename T, class = typename std::enable_if<std::is_same<T,float>::value || std::is_same<T,int32_t>::value || std::is_same<T,int16_t>::value >::type>
	Channels<T> channels(bool boost=false) {

		if(nChannels<1 || nChannels>2) throw MP3Error("Invalid number of channels");

		T *lBuffer=new T[nSamples];
		T *rBuffer=new T[nSamples];

		unsigned index=0;

		if(nChannels==1) {
			while(!it.finished()) {
				try {
					std::pair<T,T> d=it.nextPairOf<T>();
					lBuffer[index]=d.first;
					rBuffer[index]=0;
					index++;
					rBuffer[index]=d.second;
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
					std::pair<T,T> d=it.nextPairOf<T>();
					lBuffer[index]=d.first;
					rBuffer[index]=d.second;
					index++;
				}
				catch(...) {
					lBuffer[index]=0;
					rBuffer[index]=0;
					index++;
				}
			}
		}
		return Channels<T>(lBuffer,rBuffer,nChannels,nSamples,boost);
	};

	Channels<float> asFloat(bool boost=false);



};
}}

#endif /* PCMDATA_HPP_ */
