// -----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------------------------
#define BOOST_TEST_MODULE mitrax make_matrix
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <mitrax/matrix/view.hpp>

#include <array>
#include <vector>
#include <complex>
#include <iostream>

#define BT BOOST_TEST


using namespace mitrax;
using namespace mitrax::literals;


template < typename T >
auto rt_id(T&& v){
	return boost::typeindex::type_id_runtime(static_cast< T&& >(v));
}

template < typename T >
auto const id = boost::typeindex::type_id< T >();


template < typename Matrix, size_t RefC, size_t RefR >
constexpr bool check_fn(
	Matrix& m,
	value_type_t< Matrix > const(&ref)[RefR][RefC]
){
	using value_type = value_type_t< Matrix >;

	bool res = true;
	res &= m.cols() == cols< col_t(RefC) >();
	res &= m.rows() == rows< row_t(RefR) >();

	for(size_t y = 0; y < RefR; ++y){
		for(size_t x = 0; x < RefC; ++x){
			res &= m(c_t(x), r_t(y)) == ref[y][x];
		}
	}

	if constexpr(Matrix::ct_cols == 1_C || Matrix::ct_rows == 1_R){
		auto vec_ref =
			reinterpret_cast< value_type const(&)[RefC * RefR] >(ref);

		for(size_t i = 0; i < RefC * RefR; ++i){
			res &= m[d_t(i)] == vec_ref[i];
		}
	}

	if constexpr(Matrix::ct_cols == 1_C && Matrix::ct_rows == 1_R){
		res &= static_cast< value_type >(m) == ref[0][0];
	}

	return res;
}

template < typename M, col_t C, row_t R, size_t RefC, size_t RefR >
constexpr bool check(
	matrix< M, C, R > const& m,
	value_type_t< M > const(&ref)[RefR][RefC]
){
	return check_fn(m, ref);
}

template < typename M, col_t C, row_t R, size_t RefC, size_t RefR >
constexpr bool check(
	matrix< M, C, R >& m,
	value_type_t< M > const(&ref)[RefR][RefC]
){
	auto const& m_const = m;
	return check_fn(m, ref) && check(m_const, ref);
}


template < typename T >
struct fn_xy_t{
	template < typename T1, typename T2 >
	constexpr T operator()(T1 x, T2 y)const noexcept{
		return size_t(x) + size_t(y) * 10;
	}
};

template < typename T >
struct fn_x_t{
	template < typename T1 >
	constexpr T operator()(T1 i)const noexcept{
		return size_t(i) * 10;
	}
};

template < typename T >
struct fn_y_t{
	template < typename T1 >
	constexpr T operator()(T1 i)const noexcept{
		return size_t(i);
	}
};

template < typename T >
struct fn_i_t{
	template < typename T1 >
	constexpr T operator()(T1 i)const noexcept{
		return size_t(i) + size_t(i) * 10;
	}
};


BOOST_AUTO_TEST_SUITE(suite_make_matrix)


using types = boost::mpl::list< int, double, std::complex< float > >;

using objects = boost::mpl::list< int[4], std::array< int, 4 >, std::vector< int > >;


BOOST_AUTO_TEST_CASE_TEMPLATE(test_view_matrix_iter, Object, objects){
	using type = detail::view_matrix_impl< Object, true, 3_C, 3_R >;
	static_assert(has_iterator_fn_v< type >);
	static_assert(has_data_v< int*, type >);
	static_assert(has_iterator_fn_v< type const >);
	static_assert(has_data_v< int const*, type const >);


	using const_type = detail::const_view_matrix_impl< Object, true, 3_C, 3_R >;
	static_assert(has_iterator_fn_v< const_type >);
	static_assert(has_data_v< int const*, const_type >);
	static_assert(has_iterator_fn_v< const_type const >);
	static_assert(has_data_v< int const*, const_type const >);


	using non_const_t_only_const_data = detail::view_matrix_impl<
		detail::const_view_matrix_impl< Object, true, 3_C, 3_R >,
		true, 3_C, 3_R >;
	static_assert(has_iterator_fn_v< non_const_t_only_const_data >);
	static_assert(has_data_v< int const*, non_const_t_only_const_data >);
	static_assert(has_iterator_fn_v< non_const_t_only_const_data const >);
	static_assert(has_data_v< int const*, non_const_t_only_const_data const >);
}

BOOST_AUTO_TEST_CASE(test_view_matrix_iter2){
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_view_matrix_types, Object, objects){
	using detail::view_matrix_impl;
	using detail::const_view_matrix_impl;

	BT((
		id< view_matrix< Object, true, 1_C, 4_R > > ==
		id< matrix< view_matrix_impl< Object, true, 1_C, 4_R >, 1_C, 4_R > >
	));

	BT((
		id< view_bitmap< Object, true > > ==
		id< bitmap< view_matrix_impl< Object, true, 0_C, 0_R > > >
	));

	BT((
		id< view_col_vector< Object, true, 4_R > > ==
		id< col_vector< view_matrix_impl< Object, true, 1_C, 4_R >, 4_R > >
	));

	BT((
		id< view_col_vector< Object, true, 4_R > > ==
		id< matrix< view_matrix_impl< Object, true, 1_C, 4_R >, 1_C, 4_R > >
	));

	BT((
		id< view_row_vector< Object, true, 4_C > > ==
		id< row_vector< view_matrix_impl< Object, true, 4_C, 1_R >, 4_C > >
	));

	BT((
		id< view_row_vector< Object, true, 4_C > > ==
		id< matrix< view_matrix_impl< Object, true, 4_C, 1_R >, 4_C, 1_R > >
	));

	BT((
		id< view_square_matrix< Object, true, 2_D > > ==
		id< square_matrix< view_matrix_impl< Object, true, 2_C, 2_R >, 2_D > >
	));

	BT((
		id< view_square_matrix< Object, true, 2_D > > ==
		id< matrix< view_matrix_impl< Object, true, 2_C, 2_R >, 2_C, 2_R > >
	));


	BT((
		id< const_view_matrix< Object, true, 1_C, 4_R > > ==
		id< matrix< const_view_matrix_impl< Object, true, 1_C, 4_R >,
			1_C, 4_R > >
	));

	BT((
		id< const_view_bitmap< Object, true > > ==
		id< bitmap< const_view_matrix_impl< Object, true, 0_C, 0_R > > >
	));

	BT((
		id< const_view_col_vector< Object, true, 4_R > > ==
		id< col_vector< const_view_matrix_impl< Object, true, 1_C, 4_R >,
			4_R > >
	));

	BT((
		id< const_view_col_vector< Object, true, 4_R > > ==
		id< matrix< const_view_matrix_impl< Object, true, 1_C, 4_R >,
			1_C, 4_R > >
	));

	BT((
		id< const_view_row_vector< Object, true, 4_C > > ==
		id< row_vector< const_view_matrix_impl< Object, true, 4_C, 1_R >,
			4_C > >
	));

	BT((
		id< const_view_row_vector< Object, true, 4_C > > ==
		id< matrix< const_view_matrix_impl< Object, true, 4_C, 1_R >,
			4_C, 1_R > >
	));

	BT((
		id< const_view_square_matrix< Object, true, 2_D > > ==
		id< square_matrix< const_view_matrix_impl< Object, true, 2_C, 2_R >,
			2_D > >
	));

	BT((
		id< const_view_square_matrix< Object, true, 2_D > > ==
		id< matrix< const_view_matrix_impl< Object, true, 2_C, 2_R >,
			2_C, 2_R > >
	));
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_view_matrix_3x3, T, types){
	T ref_i[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
	T init_ca[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	std::array< T, 9 > init_a{{0, 1, 2, 3, 4, 5, 6, 7, 8}};
	std::vector< T > init_v{0, 1, 2, 3, 4, 5, 6, 7, 8};

	auto m01 = make_view_matrix(3_CS, 3_RS, init_ca);
	auto m02 = make_view_matrix(3_CD, 3_RS, init_ca);
	auto m03 = make_view_matrix(3_CS, 3_RD, init_ca);
	auto m04 = make_view_matrix(3_CD, 3_RD, init_ca);
	auto m05 = make_view_matrix(dim_pair(3_CS, 3_RS), init_ca);
	auto m06 = make_view_matrix(dim_pair(3_CD, 3_RS), init_ca);
	auto m07 = make_view_matrix(dim_pair(3_CS, 3_RD), init_ca);
	auto m08 = make_view_matrix(dim_pair(3_CD, 3_RD), init_ca);
	auto m09 = make_view_matrix(3_DS, init_ca);
	auto m10 = make_view_matrix(3_DD, init_ca);
	auto m11 = make_view_matrix(dim_pair(3_DS), init_ca);
	auto m12 = make_view_matrix(dim_pair(3_DD), init_ca);

	auto m13 = make_view_matrix(3_CS, 3_RS, init_a);
	auto m14 = make_view_matrix(3_CD, 3_RS, init_a);
	auto m15 = make_view_matrix(3_CS, 3_RD, init_a);
	auto m16 = make_view_matrix(3_CD, 3_RD, init_a);
	auto m17 = make_view_matrix(dim_pair(3_CS, 3_RS), init_a);
	auto m18 = make_view_matrix(dim_pair(3_CD, 3_RS), init_a);
	auto m19 = make_view_matrix(dim_pair(3_CS, 3_RD), init_a);
	auto m20 = make_view_matrix(dim_pair(3_CD, 3_RD), init_a);
	auto m21 = make_view_matrix(3_DS, init_a);
	auto m22 = make_view_matrix(3_DD, init_a);
	auto m23 = make_view_matrix(dim_pair(3_DS), init_a);
	auto m24 = make_view_matrix(dim_pair(3_DD), init_a);

	auto m25 = make_view_matrix(3_CS, 3_RS, init_v);
	auto m26 = make_view_matrix(3_CD, 3_RS, init_v);
	auto m27 = make_view_matrix(3_CS, 3_RD, init_v);
	auto m28 = make_view_matrix(3_CD, 3_RD, init_v);
	auto m29 = make_view_matrix(dim_pair(3_CS, 3_RS), init_v);
	auto m30 = make_view_matrix(dim_pair(3_CD, 3_RS), init_v);
	auto m31 = make_view_matrix(dim_pair(3_CS, 3_RD), init_v);
	auto m32 = make_view_matrix(dim_pair(3_CD, 3_RD), init_v);
	auto m33 = make_view_matrix(3_DS, init_v);
	auto m34 = make_view_matrix(3_DD, init_v);
	auto m35 = make_view_matrix(dim_pair(3_DS), init_v);
	auto m36 = make_view_matrix(dim_pair(3_DD), init_v);

	BT((rt_id(m01) == id< view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m02) == id< view_matrix< T[9], true, 0_C, 3_R > >));
	BT((rt_id(m03) == id< view_matrix< T[9], true, 3_C, 0_R > >));
	BT((rt_id(m04) == id< view_matrix< T[9], true, 0_C, 0_R > >));
	BT((rt_id(m05) == id< view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m06) == id< view_matrix< T[9], true, 0_C, 3_R > >));
	BT((rt_id(m07) == id< view_matrix< T[9], true, 3_C, 0_R > >));
	BT((rt_id(m08) == id< view_matrix< T[9], true, 0_C, 0_R > >));
	BT((rt_id(m09) == id< view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m10) == id< view_matrix< T[9], true, 0_C, 0_R > >));
	BT((rt_id(m11) == id< view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m12) == id< view_matrix< T[9], true, 0_C, 0_R > >));

	using array = std::array< T, 9 >;
	BT((rt_id(m13) == id< view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m14) == id< view_matrix< array, true, 0_C, 3_R > >));
	BT((rt_id(m15) == id< view_matrix< array, true, 3_C, 0_R > >));
	BT((rt_id(m16) == id< view_matrix< array, true, 0_C, 0_R > >));
	BT((rt_id(m17) == id< view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m18) == id< view_matrix< array, true, 0_C, 3_R > >));
	BT((rt_id(m19) == id< view_matrix< array, true, 3_C, 0_R > >));
	BT((rt_id(m20) == id< view_matrix< array, true, 0_C, 0_R > >));
	BT((rt_id(m21) == id< view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m22) == id< view_matrix< array, true, 0_C, 0_R > >));
	BT((rt_id(m23) == id< view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m24) == id< view_matrix< array, true, 0_C, 0_R > >));

	using vector = std::vector< T >;
	BT((rt_id(m25) == id< view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m26) == id< view_matrix< vector, true, 0_C, 3_R > >));
	BT((rt_id(m27) == id< view_matrix< vector, true, 3_C, 0_R > >));
	BT((rt_id(m28) == id< view_matrix< vector, true, 0_C, 0_R > >));
	BT((rt_id(m29) == id< view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m30) == id< view_matrix< vector, true, 0_C, 3_R > >));
	BT((rt_id(m31) == id< view_matrix< vector, true, 3_C, 0_R > >));
	BT((rt_id(m32) == id< view_matrix< vector, true, 0_C, 0_R > >));
	BT((rt_id(m33) == id< view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m34) == id< view_matrix< vector, true, 0_C, 0_R > >));
	BT((rt_id(m35) == id< view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m36) == id< view_matrix< vector, true, 0_C, 0_R > >));


	BT((check(m01, ref_i)));
	BT((check(m02, ref_i)));
	BT((check(m03, ref_i)));
	BT((check(m04, ref_i)));
	BT((check(m05, ref_i)));
	BT((check(m06, ref_i)));
	BT((check(m07, ref_i)));
	BT((check(m08, ref_i)));
	BT((check(m09, ref_i)));
	BT((check(m10, ref_i)));
	BT((check(m11, ref_i)));
	BT((check(m12, ref_i)));

	BT((check(m13, ref_i)));
	BT((check(m14, ref_i)));
	BT((check(m15, ref_i)));
	BT((check(m16, ref_i)));
	BT((check(m17, ref_i)));
	BT((check(m18, ref_i)));
	BT((check(m19, ref_i)));
	BT((check(m20, ref_i)));
	BT((check(m21, ref_i)));
	BT((check(m22, ref_i)));
	BT((check(m23, ref_i)));
	BT((check(m24, ref_i)));

	BT((check(m25, ref_i)));
	BT((check(m26, ref_i)));
	BT((check(m27, ref_i)));
	BT((check(m28, ref_i)));
	BT((check(m29, ref_i)));
	BT((check(m30, ref_i)));
	BT((check(m31, ref_i)));
	BT((check(m32, ref_i)));
	BT((check(m33, ref_i)));
	BT((check(m34, ref_i)));
	BT((check(m35, ref_i)));
	BT((check(m36, ref_i)));
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_view_matrix_3x3_const, T, types){
	static constexpr T ref_i[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
	static T init_ca[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	static std::array< T, 9 > init_a{{0, 1, 2, 3, 4, 5, 6, 7, 8}};
	static std::vector< T > init_v{0, 1, 2, 3, 4, 5, 6, 7, 8};

	constexpr auto m01 = make_const_view_matrix(3_CS, 3_RS, init_ca);
	auto m02 = make_const_view_matrix(3_CD, 3_RS, init_ca);
	auto m03 = make_const_view_matrix(3_CS, 3_RD, init_ca);
	auto m04 = make_const_view_matrix(3_CD, 3_RD, init_ca);
	constexpr auto m05 = make_const_view_matrix(dim_pair(3_CS, 3_RS), init_ca);
	auto m06 = make_const_view_matrix(dim_pair(3_CD, 3_RS), init_ca);
	auto m07 = make_const_view_matrix(dim_pair(3_CS, 3_RD), init_ca);
	auto m08 = make_const_view_matrix(dim_pair(3_CD, 3_RD), init_ca);
	constexpr auto m09 = make_const_view_matrix(3_DS, init_ca);
	auto m10 = make_const_view_matrix(3_DD, init_ca);
	constexpr auto m11 = make_const_view_matrix(dim_pair(3_DS), init_ca);
	auto m12 = make_const_view_matrix(dim_pair(3_DD), init_ca);

	auto m13 = make_const_view_matrix(3_CS, 3_RS, init_a);
	auto m14 = make_const_view_matrix(3_CD, 3_RS, init_a);
	auto m15 = make_const_view_matrix(3_CS, 3_RD, init_a);
	auto m16 = make_const_view_matrix(3_CD, 3_RD, init_a);
	auto m17 = make_const_view_matrix(dim_pair(3_CS, 3_RS), init_a);
	auto m18 = make_const_view_matrix(dim_pair(3_CD, 3_RS), init_a);
	auto m19 = make_const_view_matrix(dim_pair(3_CS, 3_RD), init_a);
	auto m20 = make_const_view_matrix(dim_pair(3_CD, 3_RD), init_a);
	auto m21 = make_const_view_matrix(3_DS, init_a);
	auto m22 = make_const_view_matrix(3_DD, init_a);
	auto m23 = make_const_view_matrix(dim_pair(3_DS), init_a);
	auto m24 = make_const_view_matrix(dim_pair(3_DD), init_a);

	auto m25 = make_const_view_matrix(3_CS, 3_RS, init_v);
	auto m26 = make_const_view_matrix(3_CD, 3_RS, init_v);
	auto m27 = make_const_view_matrix(3_CS, 3_RD, init_v);
	auto m28 = make_const_view_matrix(3_CD, 3_RD, init_v);
	auto m29 = make_const_view_matrix(dim_pair(3_CS, 3_RS), init_v);
	auto m30 = make_const_view_matrix(dim_pair(3_CD, 3_RS), init_v);
	auto m31 = make_const_view_matrix(dim_pair(3_CS, 3_RD), init_v);
	auto m32 = make_const_view_matrix(dim_pair(3_CD, 3_RD), init_v);
	auto m33 = make_const_view_matrix(3_DS, init_v);
	auto m34 = make_const_view_matrix(3_DD, init_v);
	auto m35 = make_const_view_matrix(dim_pair(3_DS), init_v);
	auto m36 = make_const_view_matrix(dim_pair(3_DD), init_v);

	BT((rt_id(m01) == id< const_view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m02) == id< const_view_matrix< T[9], true, 0_C, 3_R > >));
	BT((rt_id(m03) == id< const_view_matrix< T[9], true, 3_C, 0_R > >));
	BT((rt_id(m04) == id< const_view_matrix< T[9], true, 0_C, 0_R > >));
	BT((rt_id(m05) == id< const_view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m06) == id< const_view_matrix< T[9], true, 0_C, 3_R > >));
	BT((rt_id(m07) == id< const_view_matrix< T[9], true, 3_C, 0_R > >));
	BT((rt_id(m08) == id< const_view_matrix< T[9], true, 0_C, 0_R > >));
	BT((rt_id(m09) == id< const_view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m10) == id< const_view_matrix< T[9], true, 0_C, 0_R > >));
	BT((rt_id(m11) == id< const_view_matrix< T[9], true, 3_C, 3_R > >));
	BT((rt_id(m12) == id< const_view_matrix< T[9], true, 0_C, 0_R > >));

	using array = std::array< T, 9 >;
	BT((rt_id(m13) == id< const_view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m14) == id< const_view_matrix< array, true, 0_C, 3_R > >));
	BT((rt_id(m15) == id< const_view_matrix< array, true, 3_C, 0_R > >));
	BT((rt_id(m16) == id< const_view_matrix< array, true, 0_C, 0_R > >));
	BT((rt_id(m17) == id< const_view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m18) == id< const_view_matrix< array, true, 0_C, 3_R > >));
	BT((rt_id(m19) == id< const_view_matrix< array, true, 3_C, 0_R > >));
	BT((rt_id(m20) == id< const_view_matrix< array, true, 0_C, 0_R > >));
	BT((rt_id(m21) == id< const_view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m22) == id< const_view_matrix< array, true, 0_C, 0_R > >));
	BT((rt_id(m23) == id< const_view_matrix< array, true, 3_C, 3_R > >));
	BT((rt_id(m24) == id< const_view_matrix< array, true, 0_C, 0_R > >));

	using vector = std::vector< T >;
	BT((rt_id(m25) == id< const_view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m26) == id< const_view_matrix< vector, true, 0_C, 3_R > >));
	BT((rt_id(m27) == id< const_view_matrix< vector, true, 3_C, 0_R > >));
	BT((rt_id(m28) == id< const_view_matrix< vector, true, 0_C, 0_R > >));
	BT((rt_id(m29) == id< const_view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m30) == id< const_view_matrix< vector, true, 0_C, 3_R > >));
	BT((rt_id(m31) == id< const_view_matrix< vector, true, 3_C, 0_R > >));
	BT((rt_id(m32) == id< const_view_matrix< vector, true, 0_C, 0_R > >));
	BT((rt_id(m33) == id< const_view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m34) == id< const_view_matrix< vector, true, 0_C, 0_R > >));
	BT((rt_id(m35) == id< const_view_matrix< vector, true, 3_C, 3_R > >));
	BT((rt_id(m36) == id< const_view_matrix< vector, true, 0_C, 0_R > >));


	BT((check(m01, ref_i)));
	BT((check(m02, ref_i)));
	BT((check(m03, ref_i)));
	BT((check(m04, ref_i)));
	BT((check(m05, ref_i)));
	BT((check(m06, ref_i)));
	BT((check(m07, ref_i)));
	BT((check(m08, ref_i)));
	BT((check(m09, ref_i)));
	BT((check(m10, ref_i)));
	BT((check(m11, ref_i)));
	BT((check(m12, ref_i)));

	BT((check(m13, ref_i)));
	BT((check(m14, ref_i)));
	BT((check(m15, ref_i)));
	BT((check(m16, ref_i)));
	BT((check(m17, ref_i)));
	BT((check(m18, ref_i)));
	BT((check(m19, ref_i)));
	BT((check(m20, ref_i)));
	BT((check(m21, ref_i)));
	BT((check(m22, ref_i)));
	BT((check(m23, ref_i)));
	BT((check(m24, ref_i)));

	BT((check(m25, ref_i)));
	BT((check(m26, ref_i)));
	BT((check(m27, ref_i)));
	BT((check(m28, ref_i)));
	BT((check(m29, ref_i)));
	BT((check(m30, ref_i)));
	BT((check(m31, ref_i)));
	BT((check(m32, ref_i)));
	BT((check(m33, ref_i)));
	BT((check(m34, ref_i)));
	BT((check(m35, ref_i)));
	BT((check(m36, ref_i)));
}


BOOST_AUTO_TEST_SUITE_END()
