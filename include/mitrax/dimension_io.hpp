//-----------------------------------------------------------------------------
// Copyright (c) 2009-2015 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _mitrax__dimension_io__hpp_INCLUDED_
#define _mitrax__dimension_io__hpp_INCLUDED_

#include "io.hpp"
#include "dimension.hpp"

#include <iostream>


namespace mitrax{


	template < typename charT, typename traits, typename TX, typename TY >
	std::basic_ostream< charT, traits >& operator<<(
		std::basic_ostream< charT, traits >& os,
		dimension< TX, TY > const& data
	){
		return os << data.cols() << "x" << data.rows();
	}


	template < typename charT, typename traits, typename TX, typename TY >
	std::basic_istream< charT, traits >& operator>>(
		std::basic_istream< charT, traits >& is,
		dimension< TX, TY >& data
	){
		dimension< TX, TY > tmp;
		is >> tmp.cols();
		if(!io::equal(is, 'x')) return is;
		is >> tmp.rows();

		data = std::move(tmp);

		return is;
	}


}


#endif
