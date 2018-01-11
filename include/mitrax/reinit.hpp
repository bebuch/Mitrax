//-----------------------------------------------------------------------------
// Copyright (c) 2016-2018 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _mitrax__reinit__hpp_INCLUDED_
#define _mitrax__reinit__hpp_INCLUDED_

#include "matrix_interface.hpp"

#include "iterator/convert.hpp"


namespace mitrax{


	namespace detail{


		template< typename M, typename T, typename = void >
		struct has_reinit_v: std::false_type{};

		template< typename M, typename T >
		struct has_reinit_v< M, T, decltype((void)
			std::declval< M >().reinit_v(std::declval< T >())
		) >: std::true_type{};

		template< typename M, typename T >
		constexpr auto has_reinit_v_v = has_reinit_v< M, T >::value;


		template< typename M, typename T, typename = void >
		struct has_reinit_iter: std::false_type{};

		template< typename M, typename T >
		struct has_reinit_iter< M, T, decltype((void)
			std::declval< M >().reinit_iter(std::declval< T >())
		) >: std::true_type{};

		template< typename M, typename T >
		constexpr auto has_reinit_iter_v = has_reinit_iter< M, T >::value;


	}


	template < typename M, col_t C, row_t R, typename Iter >
	constexpr void reinit_iter(matrix< M, C, R >& m, Iter iter){
		static_assert(detail::has_reinit_iter_v< M, Iter >,
			"Matrix implementation M does not support reinit_iter");

		m.impl().reinit_iter(iter);
	}

	template < typename T, typename M, col_t C, row_t R >
	constexpr void reinit_v(matrix< M, C, R >& m, T const& v = T()){
		reinit_iter(m, make_value_iterator(v));
	}

	template < typename M, col_t C, row_t R, typename F >
	constexpr void reinit_fn(matrix< M, C, R >& m, F&& f){
		reinit_iter(m, make_function_iterator(make_function_xy_adapter(
				static_cast< F&& >(f), m.cols())));
	}

	template < typename T, typename M, col_t C, row_t R,
		enable_if_t< C != 0_C && R != 0_R > = 0 >
	constexpr void reinit(matrix< M, C, R >& m, T(&v)[size_t(R)][size_t(C)]){
		reinit_iter(m, make_convert_iterator< value_type_t< M > >(
			flat_iterator< T, size_t(R), size_t(C) >(&v)));
	}

	template < typename T, typename M, col_t C, row_t R,
		enable_if_t< C != 0_C && R != 0_R > = 0 >
	constexpr void reinit(matrix< M, C, R >& m, T(&&v)[size_t(R)][size_t(C)]){
		reinit_iter(m, mitrax::make_move_iterator(
			make_convert_iterator< value_type_t< M > >(
			flat_iterator< T, size_t(R), size_t(C) >(&v))));
	}

	template < typename M, col_t C, row_t R, typename F >
	constexpr void reinit_vector_fn(matrix< M, C, R >& m, F&& f){
		static_assert((C == 1_C && R != 0_R) || (C != 0_C && R == 1_R),
			"reinit_vector_fn is only allowed for compile time dim vectors");

		reinit_iter(m, make_function_iterator(static_cast< F&& >(f)));
	}

	template < typename T, typename M, col_t C, row_t R,
		enable_if_t< (C == 1_C && R != 0_R) || (C != 0_C && R == 1_R) > = 0 >
	constexpr void reinit_vector(
		matrix< M, C, R >& m, T(&v)[size_t(R) * size_t(C)]
	){
		reinit_iter(m, make_convert_iterator< value_type_t< M > >(v));
	}

	template < typename T, typename M, col_t C, row_t R,
		enable_if_t< (C == 1_C && R != 0_R) || (C != 0_C && R == 1_R) > = 0 >
	constexpr void reinit_vector(
		matrix< M, C, R >& m, T(&&v)[size_t(R) * size_t(C)]
	){
		reinit_iter(m, mitrax::make_move_iterator(
			make_convert_iterator< value_type_t< M > >(v)));
	}


}


#endif
