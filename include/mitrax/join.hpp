//-----------------------------------------------------------------------------
// Copyright (c) 2016-2018 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _mitrax__join__hpp_INCLUDED_
#define _mitrax__join__hpp_INCLUDED_

#include "make_matrix.hpp"


namespace mitrax{


	// TODO: Test for same value_type
	// TODO: Allow values
	// TODO: Add some kind of join_grid
	namespace detail{


		template < typename M, typename ... T >
		struct join_h_proxy{
			constexpr join_h_proxy(M const& m, T const& ... v):
				m(m), d(v ...){ rows(); }

			constexpr auto operator()(c_t c, r_t r)const{
				return c < m.cols() ? m(c, r) : d(c - c_t(m.cols()), r);
			}

			constexpr auto cols()const{
				return m.cols() + d.cols();
			}

			constexpr auto rows()const{
				return get(m.rows(), d.rows());
			}

			M const& m;
			join_h_proxy< T ... > const d;
		};

		template < typename M >
		struct join_h_proxy< M >{
			constexpr join_h_proxy(M const& m):
				m(m){}

			constexpr auto operator()(c_t c, r_t r)const{
				if(c < m.cols()) return m(c, r);
				throw std::out_of_range("join_h_proxy");
			}

			constexpr auto cols()const{
				return m.cols();
			}

			constexpr auto rows()const{
				return m.rows();
			}

			M const& m;
		};

		template < typename M, typename ... T >
		struct join_v_proxy{
			constexpr join_v_proxy(M const& m, T const& ... v):
				m(m), d(v ...){ cols(); }


			constexpr auto operator()(c_t c, r_t r)const{
				return r < m.rows() ? m(c, r) : d(c, r - r_t(m.rows()));
			}

			constexpr auto cols()const{
				return get(m.cols(), d.cols());
			}

			constexpr auto rows()const{
				return m.rows() + d.rows();
			}

			M const& m;
			join_v_proxy< T ... > const d;
		};

		template < typename M >
		struct join_v_proxy< M >{
			constexpr join_v_proxy(M const& m):
				m(m){}

			constexpr auto operator()(c_t c, r_t r)const{
				if(r < m.rows()) return m(c, r);
				throw std::out_of_range("join_v_proxy");
			}

			constexpr auto cols()const{
				return m.cols();
			}

			constexpr auto rows()const{
				return m.rows();
			}

			M const& m;
		};


	}


	template < typename ... T >
	constexpr auto join_h(T const& ... v){
		return detail::join_h_proxy< T ... >(v ...);
	}

	template < typename ... T >
	constexpr auto join_v(T const& ... v){
		return detail::join_v_proxy< T ... >(v ...);
	}

	template < typename T >
	constexpr auto join(T const& proxy){
		return make_matrix_fn(proxy.cols(), proxy.rows(), proxy);
	}


}


#endif
