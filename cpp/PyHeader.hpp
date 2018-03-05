/*
 * PyHeader.hpp
 *
 *  Created on: 2 Dec 2017
 *      Author: julianporter
 */

#ifndef MODULE_CPP_PYHEADER_HPP_
#define MODULE_CPP_PYHEADER_HPP_

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "PException.hpp"

#if PY_MAJOR_VERSION==3
	#define IS_PYTHON3
#else
	#define IS_PYTHON2
#endif



#endif /* MODULE_CPP_PYHEADER_HPP_ */
