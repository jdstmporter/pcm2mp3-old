/*
 * Enumerated.hpp
 *
 *  Created on: 8 Dec 2017
 *      Author: julianporter
 */

#ifndef ENUMERATED_HPP_
#define ENUMERATED_HPP_

#include <string>
#include <iostream>
#include <type_traits>

namespace pylame {
namespace util {


struct NamedValue {
private:
	 const char *name;
	 const unsigned value;

	 static unsigned counter;
public:
	constexpr NamedValue() : name(""), value(0) {};
	constexpr NamedValue(const char *name_,const unsigned value_) : name(name_), value(value_) {};
	constexpr NamedValue(const NamedValue &o) : name(o.name), value(o.value) {};
	NamedValue & operator=(NamedValue &o) = delete;
	//NamedValue & operator=(NamedValue &&o) {
	//	std::swap(*this,o);
	//	return *this;
	//};
	
	std::string str() const { return std::string(name); };
	constexpr operator unsigned() const { return value; };
	//T val() const { return value; };
	static void reset() { counter=0; }
	static const unsigned count() {
		return counter++;
	};
};

struct Counter {
	unsigned n;

	Counter() : n(0) {};
	operator unsigned() { return ++n; };
};


} /* namespace util */
} /* namespace pylame */


bool operator==(const pylame::util::NamedValue &l,const pylame::util::NamedValue &r) {
	return (unsigned)l==(unsigned)r;
};
bool operator!=(const pylame::util::NamedValue &l,const pylame::util::NamedValue &r){
	return unsigned(l)!=unsigned(r);
};
bool operator<(const pylame::util::NamedValue &l,const pylame::util::NamedValue &r) {
	return unsigned(l)<unsigned(r);
};
using item_t = pylame::util::NamedValue;


#define ITEM(name,value) static constexpr item_t name = item_t(#name,value);
#define _CLASS_FUNCS \
		using value_t=item_t;\
		using const_iterator=std::vector<item_t>::const_iterator; \
		static const std::vector<item_t> all; \
		item_t static named(const std::string &name,bool caseIndependent); \
		static const_iterator cbegin() { return all.cbegin(); }; \
		static const_iterator cend() { return all.cend(); };
#define _CLASS_FUNCS_DEF(cls, ...) \
	const std::vector<item_t> cls::all=std::vector<item_t>{ __VA_ARGS__ }; \
	item_t cls::named(const std::string &name,bool caseIndependent) { \
		auto it=std::find_if(all.begin(),all.end(),[&name](auto &v) { return v.str()==name; }); \
		return (it==all.end()) ? Other : *it; \
	}





#define ENUM_MAKE_STEP(count) ENUM_M ## count

#define ENUM_INVOKE(macro, ...) ENUM_EXPAND(macro(__VA_ARGS__))
#define ENUM_EXPAND(x) x

#define ENUM_M1(d,...) ITEM(d,0)
#define ENUM_M2(d,...) ITEM(d,1) \
		ENUM_EXPAND(ENUM_M1(__VA_ARGS__))
#define ENUM_M3(d,...) ITEM(d,2) \
		ENUM_EXPAND(ENUM_M2(__VA_ARGS__))
#define ENUM_M4(d,...) ITEM(d,3) \
		ENUM_EXPAND(ENUM_M3(__VA_ARGS__))
#define ENUM_M5(d,...) ITEM(d,4) \
		ENUM_EXPAND(ENUM_M4(__VA_ARGS__))
#define ENUM_M6(d,...) ITEM(d,5) \
		ENUM_EXPAND(ENUM_M5(__VA_ARGS__))
#define ENUM_M7(d,...) ITEM(d,6) \
		ENUM_EXPAND(ENUM_M6(__VA_ARGS__))
#define ENUM_M8(d,...) ITEM(d,7) \
		ENUM_EXPAND(ENUM_M7(__VA_ARGS__))
#define ENUM_M9(d,...) ITEM(d,8) \
		ENUM_EXPAND(ENUM_M8(__VA_ARGS__))
#define ENUM_M10(d,...) ITEM(d,9) \
		ENUM_EXPAND(ENUM_M9(__VA_ARGS__))
#define ENUM_COUNT_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, count, ...) count

#define ENUM_COUNT(...) \
		ENUM_EXPAND(ENUM_COUNT_I('x',__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

		#define ENUM_ITERATE(X, f, l) X(f, l, 0) X(f, l, 1) X(f, l, 2)        \
		    X(f, l, 3) X(f, l, 4) X(f, l, 5) X(f, l, 6) X(f, l, 7) X(f, l, 8)          \
		    X(f, l, 9) X(f, l, 10) X(f, l, 11) X(f, l, 12) X(f, l, 13) X(f, l, 14)     \
		    X(f, l, 15) X(f, l, 16) X(f, l, 17) X(f, l, 18) X(f, l, 19) X(f, l, 20)    \
		    X(f, l, 21) X(f, l, 22) X(f, l, 23)


#define __loop(macro, ...) \
		ENUM_EXPAND( \
    		ENUM_INVOKE( \
    			ENUM_MAKE_STEP, \
            ENUM_COUNT(__VA_ARGS__)) \
        (macro, __VA_ARGS__))

#define ENUM_CLASS(name,...) \
		struct name { \
			__loop(ITEM,__VA_ARGS__) \
			_CLASS_FUNCS \
	};



#endif /* ENUMERATED_HPP_ */
