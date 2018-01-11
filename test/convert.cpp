//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE mitrax convert
#include <boost/test/unit_test.hpp>

#include <mitrax/convert.hpp>


using boost::typeindex::type_id;
using boost::typeindex::type_id_runtime;
using namespace mitrax;
using namespace mitrax::literals;


constexpr int data[2][2] = {{0, 1}, {2, 3}};

template < typename M >
constexpr bool check(M const& m){
	return
		m.cols() == 2_CS &&
		m.rows() == 2_RS &&
		m(0_c, 0_r) == 0 &&
		m(1_c, 0_r) == 1 &&
		m(0_c, 1_r) == 2 &&
		m(1_c, 1_r) == 3;
}


using int_ref = std::reference_wrapper< int >;

using cint_ref = std::reference_wrapper< int const >;


template < typename T >
auto rt_id(T&& v){
	return type_id_runtime(static_cast< T&& >(v));
}

template < typename T >
auto const id = type_id< T >();


BOOST_AUTO_TEST_SUITE(suite_convert)


BOOST_AUTO_TEST_CASE(test_convert_as_std_matrix){
	constexpr auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);
	auto m5 = m1;

	constexpr auto n1 = as_std_matrix(m1);
	auto const n2 = as_std_matrix(m2);
	auto const n3 = as_std_matrix(m3);
	auto const n4 = as_std_matrix(m4);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int, 0_C, 0_R > >));

	auto const o1 = as_std_matrix(m5);
	auto const o2 = as_std_matrix(m2);
	auto const o3 = as_std_matrix(m3);
	auto const o4 = as_std_matrix(m4);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< int, 0_C, 0_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_type){
	constexpr auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);
	auto m5 = m1;

	constexpr auto n1 = convert< double >(m1);
	auto const n2 = convert< double >(m2);
	auto const n3 = convert< double >(m3);
	auto const n4 = convert< double >(m4);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< double, 0_C, 0_R > >));

	auto const o1 = convert< double >(m5);
	auto const o2 = convert< double >(m2);
	auto const o3 = convert< double >(m3);
	auto const o4 = convert< double >(m4);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< double, 0_C, 0_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2x2){
	constexpr auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);
	auto m5 = m1;

	constexpr auto n1 = convert(m1, 2_CS, 2_RS);
	auto const n2 = convert(m2, 2_CS, 2_RS);
	auto const n3 = convert(m3, 2_CS, 2_RS);
	auto const n4 = convert(m4, 2_CS, 2_RS);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int, 2_C, 2_R > >));

	auto const o1 = convert< int >(m5, 2_CS, 2_RS);
	auto const o2 = convert< int >(m2, 2_CS, 2_RS);
	auto const o3 = convert< int >(m3, 2_CS, 2_RS);
	auto const o4 = convert< int >(m4, 2_CS, 2_RS);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< int, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< int, 2_C, 2_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2rtx2){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert(m1, 2_CD, 2_RS);
	auto const n2 = convert(m2, 2_CD, 2_RS);
	auto const n3 = convert(m3, 2_CD, 2_RS);
	auto const n4 = convert(m4, 2_CD, 2_RS);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int, 0_C, 2_R > >));

	auto const o1 = convert< int >(m1, 2_CD, 2_RS);
	auto const o2 = convert< int >(m2, 2_CD, 2_RS);
	auto const o3 = convert< int >(m3, 2_CD, 2_RS);
	auto const o4 = convert< int >(m4, 2_CD, 2_RS);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< int, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< int, 0_C, 2_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2x2rt){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert(m1, 2_CS, 2_RD);
	auto const n2 = convert(m2, 2_CS, 2_RD);
	auto const n3 = convert(m3, 2_CS, 2_RD);
	auto const n4 = convert(m4, 2_CS, 2_RD);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int, 2_C, 0_R > >));

	auto const o1 = convert< int >(m1, 2_CS, 2_RD);
	auto const o2 = convert< int >(m2, 2_CS, 2_RD);
	auto const o3 = convert< int >(m3, 2_CS, 2_RD);
	auto const o4 = convert< int >(m4, 2_CS, 2_RD);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< int, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< int, 2_C, 0_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2rtx2rt){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert(m1, 2_CD, 2_RD);
	auto const n2 = convert(m2, 2_CD, 2_RD);
	auto const n3 = convert(m3, 2_CD, 2_RD);
	auto const n4 = convert(m4, 2_CD, 2_RD);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int, 0_C, 0_R > >));

	auto const o1 = convert< int >(m1, 2_CD, 2_RD);
	auto const o2 = convert< int >(m2, 2_CD, 2_RD);
	auto const o3 = convert< int >(m3, 2_CD, 2_RD);
	auto const o4 = convert< int >(m4, 2_CD, 2_RD);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< int, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< int, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< int, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< int, 0_C, 0_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2x2_and_type){
	constexpr auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);
	auto m5 = m1;

	constexpr auto n1 = convert< double >(m1, 2_CS, 2_RS);
	auto const n2 = convert< double >(m2, 2_CS, 2_RS);
	auto const n3 = convert< double >(m3, 2_CS, 2_RS);
	auto const n4 = convert< double >(m4, 2_CS, 2_RS);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< double, 2_C, 2_R > >));

	auto const o1 = convert< double >(m5, 2_CS, 2_RS);
	auto const o2 = convert< double >(m2, 2_CS, 2_RS);
	auto const o3 = convert< double >(m3, 2_CS, 2_RS);
	auto const o4 = convert< double >(m4, 2_CS, 2_RS);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< double, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< double, 2_C, 2_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2rtx2_and_type){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert< double >(m1, 2_CD, 2_RS);
	auto const n2 = convert< double >(m2, 2_CD, 2_RS);
	auto const n3 = convert< double >(m3, 2_CD, 2_RS);
	auto const n4 = convert< double >(m4, 2_CD, 2_RS);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< double, 0_C, 2_R > >));

	auto const o1 = convert< double >(m1, 2_CD, 2_RS);
	auto const o2 = convert< double >(m2, 2_CD, 2_RS);
	auto const o3 = convert< double >(m3, 2_CD, 2_RS);
	auto const o4 = convert< double >(m4, 2_CD, 2_RS);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< double, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< double, 0_C, 2_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2x2rt_and_type){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert< double >(m1, 2_CS, 2_RD);
	auto const n2 = convert< double >(m2, 2_CS, 2_RD);
	auto const n3 = convert< double >(m3, 2_CS, 2_RD);
	auto const n4 = convert< double >(m4, 2_CS, 2_RD);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< double, 2_C, 0_R > >));

	auto const o1 = convert< double >(m1, 2_CS, 2_RD);
	auto const o2 = convert< double >(m2, 2_CS, 2_RD);
	auto const o3 = convert< double >(m3, 2_CS, 2_RD);
	auto const o4 = convert< double >(m4, 2_CS, 2_RD);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< double, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< double, 2_C, 0_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2rtx2rt_and_type){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert< double >(m1, 2_CD, 2_RD);
	auto const n2 = convert< double >(m2, 2_CD, 2_RD);
	auto const n3 = convert< double >(m3, 2_CD, 2_RD);
	auto const n4 = convert< double >(m4, 2_CD, 2_RD);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< double, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< double, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< double, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< double, 0_C, 0_R > >));

	auto const o1 = convert< double >(m1, 2_CD, 2_RD);
	auto const o2 = convert< double >(m2, 2_CD, 2_RD);
	auto const o3 = convert< double >(m3, 2_CD, 2_RD);
	auto const o4 = convert< double >(m4, 2_CD, 2_RD);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< double, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< double, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< double, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< double, 0_C, 0_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2x2_and_type_to_ref){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert< int_ref >(m1, 2_CS, 2_RS);
	auto const n2 = convert< int_ref >(m2, 2_CS, 2_RS);
	auto const n3 = convert< int_ref >(m3, 2_CS, 2_RS);
	auto const n4 = convert< int_ref >(m4, 2_CS, 2_RS);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int_ref, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int_ref, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int_ref, 2_C, 2_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int_ref, 2_C, 2_R > >));

	auto const o1 = convert< cint_ref >(m1, 2_CS, 2_RS);
	auto const o2 = convert< cint_ref >(m2, 2_CS, 2_RS);
	auto const o3 = convert< cint_ref >(m3, 2_CS, 2_RS);
	auto const o4 = convert< cint_ref >(m4, 2_CS, 2_RS);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< cint_ref, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< cint_ref, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< cint_ref, 2_C, 2_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< cint_ref, 2_C, 2_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2rtx2_and_type_to_ref){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert< int_ref >(m1, 2_CD, 2_RS);
	auto const n2 = convert< int_ref >(m2, 2_CD, 2_RS);
	auto const n3 = convert< int_ref >(m3, 2_CD, 2_RS);
	auto const n4 = convert< int_ref >(m4, 2_CD, 2_RS);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int_ref, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int_ref, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int_ref, 0_C, 2_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int_ref, 0_C, 2_R > >));

	auto const o1 = convert< cint_ref >(m1, 2_CD, 2_RS);
	auto const o2 = convert< cint_ref >(m2, 2_CD, 2_RS);
	auto const o3 = convert< cint_ref >(m3, 2_CD, 2_RS);
	auto const o4 = convert< cint_ref >(m4, 2_CD, 2_RS);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< cint_ref, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< cint_ref, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< cint_ref, 0_C, 2_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< cint_ref, 0_C, 2_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2x2rt_and_type_to_ref){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert< int_ref >(m1, 2_CS, 2_RD);
	auto const n2 = convert< int_ref >(m2, 2_CS, 2_RD);
	auto const n3 = convert< int_ref >(m3, 2_CS, 2_RD);
	auto const n4 = convert< int_ref >(m4, 2_CS, 2_RD);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int_ref, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int_ref, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int_ref, 2_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int_ref, 2_C, 0_R > >));

	auto const o1 = convert< cint_ref >(m1, 2_CS, 2_RD);
	auto const o2 = convert< cint_ref >(m2, 2_CS, 2_RD);
	auto const o3 = convert< cint_ref >(m3, 2_CS, 2_RD);
	auto const o4 = convert< cint_ref >(m4, 2_CS, 2_RD);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< cint_ref, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< cint_ref, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< cint_ref, 2_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< cint_ref, 2_C, 0_R > >));
}


BOOST_AUTO_TEST_CASE(test_convert_convert_to_2rtx2rt_and_type_to_ref){
	auto m1 = make_matrix(2_CS, 2_RS, data);
	auto m2 = make_matrix(2_CD, 2_RS, data);
	auto m3 = make_matrix(2_CS, 2_RD, data);
	auto m4 = make_matrix(2_CD, 2_RD, data);

	auto const n1 = convert< int_ref >(m1, 2_CD, 2_RD);
	auto const n2 = convert< int_ref >(m2, 2_CD, 2_RD);
	auto const n3 = convert< int_ref >(m3, 2_CD, 2_RD);
	auto const n4 = convert< int_ref >(m4, 2_CD, 2_RD);

	BOOST_TEST(check(n1));
	BOOST_TEST(check(n2));
	BOOST_TEST(check(n3));
	BOOST_TEST(check(n4));

	BOOST_TEST(rt_id(n1) == (id< std_matrix< int_ref, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n2) == (id< std_matrix< int_ref, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n3) == (id< std_matrix< int_ref, 0_C, 0_R > >));
	BOOST_TEST(rt_id(n4) == (id< std_matrix< int_ref, 0_C, 0_R > >));

	auto const o1 = convert< cint_ref >(m1, 2_CD, 2_RD);
	auto const o2 = convert< cint_ref >(m2, 2_CD, 2_RD);
	auto const o3 = convert< cint_ref >(m3, 2_CD, 2_RD);
	auto const o4 = convert< cint_ref >(m4, 2_CD, 2_RD);

	BOOST_TEST(check(o1));
	BOOST_TEST(check(o2));
	BOOST_TEST(check(o3));
	BOOST_TEST(check(o4));

	BOOST_TEST(rt_id(o1) == (id< std_matrix< cint_ref, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o2) == (id< std_matrix< cint_ref, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o3) == (id< std_matrix< cint_ref, 0_C, 0_R > >));
	BOOST_TEST(rt_id(o4) == (id< std_matrix< cint_ref, 0_C, 0_R > >));
}


BOOST_AUTO_TEST_SUITE_END()
