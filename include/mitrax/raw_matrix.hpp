//-----------------------------------------------------------------------------
// Copyright (c) 2015 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _mitrax__raw_matrix__hpp_INCLUDED_
#define _mitrax__raw_matrix__hpp_INCLUDED_

#include "matrix_interface.hpp"


namespace mitrax{


	template < typename T, size_t Cols, size_t Rows >
	class raw_matrix_impl{
	public:
		/// \brief Type of the data that administrates the matrix
		using value_type = T;

		/// \brief Type of a iterator for data
		using iterator = typename
			std::array< value_type, Cols * Rows >::iterator;

		/// \brief Type of a iterator for const data
		using const_iterator = typename
			std::array< value_type, Cols * Rows >::const_iterator;

		/// \brief Type of a reverse iterator for data
		using reverse_iterator = typename
			std::array< value_type, Cols * Rows >::reverse_iterator;

		/// \brief Type of a reverse iterator for const data
		using const_reverse_iterator = typename
			std::array< value_type, Cols * Rows >::const_reverse_iterator;


		constexpr raw_matrix_impl(raw_matrix_impl&&) = default;

		constexpr raw_matrix_impl(raw_matrix_impl const&) = default;

		constexpr raw_matrix_impl(
			col_t< true, Cols >, row_t< true, Rows >,
			std::array< value_type, Cols * Rows >&& values
		):
			values_(std::move(values))
			{}


		constexpr raw_matrix_impl& operator=(raw_matrix_impl&&) = default;

		constexpr raw_matrix_impl& operator=(raw_matrix_impl const&) = default;


		constexpr value_type& operator()(size_t x, size_t y){
			return values_[y * cols() + x];
		}

		constexpr value_type const& operator()(size_t x, size_t y)const{
			return values_[y * cols() + x];
		}


		constexpr col_t< Cols != 0, Cols > cols()const noexcept{
			return col_t< Cols != 0, Cols >();
		}

		constexpr row_t< Rows != 0, Rows > rows()const noexcept{
			return row_t< Rows != 0, Rows >();
		}


		constexpr iterator begin(){
			return values_.begin();
		}

		constexpr const_iterator begin()const{
			return values_.begin();
		}

		constexpr iterator end(){
			return values_.end();
		}

		constexpr const_iterator end()const{
			return values_.end();
		}

		constexpr reverse_iterator rbegin(){
			return values_.rbegin();
		}

		constexpr const_reverse_iterator rbegin()const{
			return values_.rbegin();
		}

		constexpr reverse_iterator rend(){
			return values_.rend();
		}

		constexpr const_reverse_iterator rend()const{
			return values_.rend();
		}


		constexpr std::array< value_type, Cols * Rows >& data(){
			return values_;
		}

		constexpr std::array< value_type, Cols * Rows > const& data()const{
			return values_;
		}


	private:
		std::array< value_type, Cols * Rows > values_;
	};


	template < typename T, size_t Cols, size_t Rows >
	class raw_matrix_impl_base{
	public:
		/// \brief Type of the data that administrates the matrix
		using value_type = T;

		/// \brief Type of a iterator for data
		using iterator = typename
			boost::container::vector< value_type >::iterator;

		/// \brief Type of a iterator for const data
		using const_iterator = typename
			boost::container::vector< value_type >::const_iterator;

		/// \brief Type of a reverse iterator for data
		using reverse_iterator = typename
			boost::container::vector< value_type >::reverse_iterator;

		/// \brief Type of a reverse iterator for const data
		using const_reverse_iterator = typename
			boost::container::vector< value_type >::const_reverse_iterator;


		raw_matrix_impl_base(raw_matrix_impl_base&&) = default;

		raw_matrix_impl_base(raw_matrix_impl_base const&) = default;

		raw_matrix_impl_base(
			col_t< true, Cols > c, row_t< true, Rows > r,
			boost::container::vector< value_type >&& values
		):
			values_(std::move(values)),
			cols_(c),
			rows_(r)
			{}

		raw_matrix_impl_base(
			col_t< true, Cols > c, row_t< false, Rows > r,
			boost::container::vector< value_type >&& values
		):
			values_(std::move(values)),
			cols_(c),
			rows_(r)
			{}

		raw_matrix_impl_base(
			col_t< false, Cols > c, row_t< true, Rows > r,
			boost::container::vector< value_type >&& values
		):
			values_(std::move(values)),
			cols_(c),
			rows_(r)
			{}

		raw_matrix_impl_base(
			col_t< false, Cols > c, row_t< false, Rows > r,
			boost::container::vector< value_type >&& values
		):
			values_(std::move(values)),
			cols_(c),
			rows_(r)
			{}


		raw_matrix_impl_base& operator=(raw_matrix_impl_base&&) = default;

		raw_matrix_impl_base& operator=(raw_matrix_impl_base const&) = default;


		value_type& operator()(size_t x, size_t y){
			return values_[y * cols() + x];
		}

		value_type const& operator()(size_t x, size_t y)const{
			return values_[y * cols() + x];
		}


		constexpr col_t< Cols != 0, Cols > cols()const noexcept{
			return cols_;
		}

		constexpr row_t< Rows != 0, Rows > rows()const noexcept{
			return rows_;
		}


		iterator begin(){
			return values_.begin();
		}

		const_iterator begin()const{
			return values_.begin();
		}

		iterator end(){
			return values_.end();
		}

		const_iterator end()const{
			return values_.end();
		}

		reverse_iterator rbegin(){
			return values_.rbegin();
		}

		const_reverse_iterator rbegin()const{
			return values_.rbegin();
		}

		reverse_iterator rend(){
			return values_.rend();
		}

		const_reverse_iterator rend()const{
			return values_.rend();
		}


		boost::container::vector< value_type >& data(){
			return values_;
		}

		boost::container::vector< value_type > const& data()const{
			return values_;
		}


	protected:
		boost::container::vector< value_type > values_;
		col_t< Cols != 0, Cols > cols_;
		row_t< Rows != 0, Rows > rows_;
	};


	template < typename T, size_t Rows >
	class raw_matrix_impl< T, 0, Rows >:
		public raw_matrix_impl_base< T, 0, Rows >{
	public:
		using raw_matrix_impl_base< T, 0, Rows >::raw_matrix_impl_base;
	};


	template < typename T, size_t Cols >
	class raw_matrix_impl< T, Cols, 0 >:
		public raw_matrix_impl_base< T, Cols, 0 >{
	public:
		using raw_matrix_impl_base< T, Cols, 0 >::raw_matrix_impl_base;
	};


	template < typename T >
	class raw_matrix_impl< T, 0, 0 >:
		public raw_matrix_impl_base< T, 0, 0 >{
	public:
		using raw_matrix_impl_base< T, 0, 0 >::raw_matrix_impl_base;
	};


}


#endif
