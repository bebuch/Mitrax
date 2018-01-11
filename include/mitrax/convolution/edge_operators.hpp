//-----------------------------------------------------------------------------
// Copyright (c) 2016-2018 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _mitrax__convolution_edge_operators__hpp_INCLUDED_
#define _mitrax__convolution_edge_operators__hpp_INCLUDED_

#include "../convolution.hpp"

#include <cmath>


namespace mitrax{


	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto roberts_cross_x(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_matrix< value_type >(2_DS, {{1, 0}, {0, -1}}),
			std::plus< T >(), std::multiplies< T >()
		);
	}

	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto roberts_cross_y(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_matrix< value_type >(2_DS, {{0, 1}, {-1, 0}}),
			std::plus< T >(), std::multiplies< T >()
		);
	}


	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto prewitt_x(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_vector< value_type >(3_RS, {1, 1, 1}),
			make_vector< value_type >(3_CS, {-1, 0, 1}),
			std::plus< T >(), std::multiplies< T >()
		);
	}

	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto prewitt_y(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_vector< value_type >(3_RS, {-1, 0, 1}),
			make_vector< value_type >(3_CS, {1, 1, 1}),
			std::plus< T >(), std::multiplies< T >()
		);
	}


	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto sobel_x(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_vector< value_type >(3_RS, {1, 2, 1}),
			make_vector< value_type >(3_CS, {-1, 0, 1}),
			std::plus< T >(), std::multiplies< T >()
		);
	}

	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto sobel_y(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_vector< value_type >(3_RS, {-1, 0, 1}),
			make_vector< value_type >(3_CS, {1, 2, 1}),
			std::plus< T >(), std::multiplies< T >()
		);
	}


	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto scharr_x(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_vector< value_type >(3_RS, {3, 10, 3}),
			make_vector< value_type >(3_CS, {-1, 0, 1}),
			std::plus< T >(), std::multiplies< T >()
		);
	}

	template < typename T = void, typename M, col_t C, row_t R >
	constexpr auto scharr_y(matrix< M, C, R > const& m){
		using value_type = auto_t< T, value_type_t< M > >;
		return convolution(
			m,
			make_vector< value_type >(3_RS, {-1, 0, 1}),
			make_vector< value_type >(3_CS, {3, 10, 3}),
			std::plus< T >(), std::multiplies< T >()
		);
	}


	template <
		typename T = void,
		typename M1, col_t C1, row_t R1,
		typename M2, col_t C2, row_t R2
	> constexpr auto gradient_magnitude(
		matrix< M1, C1, R1 > const& mx,
		matrix< M2, C2, R2 > const& my
	){
		return make_matrix_fn(get_dims(mx, my), [&mx, &my](auto j, auto k){
			decltype(auto) x = auto_ref< T >(mx(j, k));
			decltype(auto) y = auto_ref< T >(my(j, k));
			return auto_ref< T >(std::sqrt(x * x + y * y));
		});
	}

	template <
		typename T = void,
		typename M1, col_t C1, row_t R1,
		typename M2, col_t C2, row_t R2
	> constexpr auto gradient_magnitude_approximately(
		matrix< M1, C1, R1 > const& mx,
		matrix< M2, C2, R2 > const& my
	){
		return make_matrix_fn(get_dims(mx, my), [&mx, &my](auto j, auto k){
			return auto_ref< T >(
				std::abs(auto_ref< T >(my(j, k))) +
				std::abs(auto_ref< T >(mx(j, k)))
			);
		});
	}

	template <
		typename T = void,
		typename M1, col_t C1, row_t R1,
		typename M2, col_t C2, row_t R2
	> constexpr auto gradient_direction(
		matrix< M1, C1, R1 > const& mx,
		matrix< M2, C2, R2 > const& my
	){
		return make_matrix_fn(get_dims(mx, my), [&mx, &my](auto j, auto k){
			return auto_ref< T >(
				std::atan2(auto_ref< T >(my(j, k)), auto_ref< T >(mx(j, k)))
			);
		});
	}



}

#endif
