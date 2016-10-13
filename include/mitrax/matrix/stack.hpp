//-----------------------------------------------------------------------------
// Copyright (c) 2015-2016 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _mitrax__matrix__stack__hpp_INCLUDED_
#define _mitrax__matrix__stack__hpp_INCLUDED_

#include "stack_fwd.hpp"

#include "../iterator/value.hpp"


namespace mitrax::detail{


	template < typename T, size_t N >
	struct array_s{
		T elems[N]; // exposition only

		// element access:
		constexpr T& operator[](std::size_t n){ return elems[n]; }

		constexpr T const& operator[](std::size_t n)const{
			return elems[n];
		}

		constexpr T& at(std::size_t n){
			if(n >= N) throw std::out_of_range("array_s");
			return elems[n];
		}

		constexpr T const& at(std::size_t n)const{
			if(n >= N) throw std::out_of_range("array_s");
			return elems[n];
		}

		constexpr T& front(){ return elems[0]; }

		constexpr T const& front()const{ return elems[0]; }

		constexpr T& back(){ return elems[N - 1]; }

		constexpr T const& back()const{ return elems[N - 1]; }

		constexpr T* data()noexcept{ return elems; }

		constexpr T const* data()const noexcept{ return elems; }
	};


	template < typename Iter, size_t ... I >
	constexpr array_s< iter_type_t< Iter >, sizeof...(I) >
	to_array(Iter iter, std::index_sequence< I ... >){
		return {{ iter[I] ... }};
	}

	template < size_t N, typename Iter >
	constexpr auto to_array(Iter iter){
		return to_array(iter, std::make_index_sequence< N >());
	}


	template < typename T, size_t C, size_t R >
	class stack_matrix_impl: public dims_t< C, R >{
	public:
		static_assert(!std::is_const_v< T >);
		static_assert(!std::is_reference_v< T >);
		static_assert(
			C > 0 && R > 0,
			"stack_matrix_impl< T, C, R > is for static dimensions only, "
			"use heap_matrix_impl< T, C, R > instead"
		);


		/// \brief Type of the data that administrates the matrix
		using value_type = T;

		/// \brief Type with the make functions
		using maker_type = maker::stack_t;

		/// \brief Type of a iterator for data
		using iterator = value_type*;

		/// \brief Type of a iterator for const data
		using const_iterator = value_type const*;

		/// \brief Type of a reverse iterator for data
		using reverse_iterator = std::reverse_iterator< iterator >;

		/// \brief Type of a reverse iterator for const data
		using const_reverse_iterator = std::reverse_iterator< const_iterator >;


		constexpr stack_matrix_impl():
			values_(to_array< C * R >(
				mitrax::make_value_iterator< value_type >())) {}

		constexpr stack_matrix_impl(stack_matrix_impl&&) = default;

		constexpr stack_matrix_impl(stack_matrix_impl const&) = default;

		constexpr stack_matrix_impl(
			col_t< true, C >, row_t< true, R >,
			array_s< value_type, C * R >&& values
		):
			values_(std::move(values))
			{}


		constexpr stack_matrix_impl& operator=(stack_matrix_impl&&) = default;

		constexpr stack_matrix_impl& operator=(stack_matrix_impl const&) = default;


		constexpr value_type& operator()(size_t x, size_t y){
			return values_[y * this->cols() + x];
		}

		constexpr value_type const& operator()(size_t x, size_t y)const{
			return values_[y * this->cols() + x];
		}


		constexpr iterator begin(){
			return values_.data();
		}

		constexpr const_iterator begin()const{
			return values_.data();
		}

		constexpr iterator end(){
			return values_.data() + C * R;
		}

		constexpr const_iterator end()const{
			return values_.data() + C * R;
		}

		constexpr reverse_iterator rbegin(){
			return std::make_reverse_iterator(end());
		}

		constexpr const_reverse_iterator rbegin()const{
			return std::make_reverse_iterator(end());
		}

		constexpr reverse_iterator rend(){
			return std::make_reverse_iterator(begin());
		}

		constexpr const_reverse_iterator rend()const{
			return std::make_reverse_iterator(begin());
		}


		constexpr value_type* data(){
			return values_.data();
		}

		constexpr value_type const* data()const{
			return values_.data();
		}


		template < typename Iter >
		constexpr void reinit_iter(Iter iter){
			*this = maker_type().by_sequence
				(this->cols(), this->rows(), iter).impl();
		}


	private:
		array_s< value_type, C * R > values_;
	};


}


namespace mitrax::maker{


	template < typename Iter, bool Cct, size_t C, bool Rct, size_t R >
	constexpr stack_matrix< iter_type_t< Iter >, Cct ? C : 0, Rct ? R : 0 >
	stack_t::by_sequence(col_t< Cct, C > c, row_t< Rct, R > r, Iter iter)const{
		return {init, c, r, detail::to_array< C * R >(iter)};
	}

	constexpr auto stack = stack_t();


}


#endif
