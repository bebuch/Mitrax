//-----------------------------------------------------------------------------
// Copyright (c) 2015 Benjamin Buch
//
// https://github.com/bebuch/mitrax
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _mitrax__dim__hpp_INCLUDED_
#define _mitrax__dim__hpp_INCLUDED_

#include <cstddef>
#include <type_traits>
#include <functional>


namespace mitrax{


	using size_t = std::size_t;

	using ptrdiff_t = std::ptrdiff_t;


	template < typename T >
	using value_type_t = typename T::value_type;

	template < bool I >
	using bool_t = std::integral_constant< bool, I >;


	constexpr size_t dim(bool is_compile_time, size_t n)noexcept{
		return is_compile_time ? n : 0;
	}


	template < size_t V >
	struct size_ct{
		static constexpr size_t value = V;

		using value_type = size_t;

		using type = size_ct;

		constexpr operator size_t()const noexcept{
			return value;
		}

		constexpr size_t operator()()const noexcept{
			return value;
		}
	};


	struct size_rt: size_ct< 0 >{
	public:
		constexpr size_rt()noexcept: v_(0) {}

		constexpr size_rt(size_t v)noexcept: v_(v) {}


		constexpr operator size_t()const noexcept{
			return v_;
		}

		constexpr size_t operator()()const noexcept{
			return v_;
		}

	public:
		size_t v_;
	};


	template < size_t I >
	struct col_t;

	template < size_t I >
	struct row_t;

	template < size_t C, size_t R >
	class dim_t;


	template < bool CompileTimeTarget, size_t I >
	struct col_init_t;

	template < bool CompileTimeTarget, size_t I >
	struct row_init_t;

	template < bool CompileTimeTarget, size_t I >
	struct dim_init_t;


	template < size_t I >
	struct col_t: size_ct< I >{
		using size_ct< I >::size_ct;

		constexpr auto init()const noexcept{
			return col_init_t< true, I >();
		}

		constexpr auto as_row()const noexcept{
			return row_t< I >();
		}
	};

	template <>
	struct col_t< 0 >: size_rt{
		using size_rt::size_rt;

		constexpr auto init()const noexcept;
		constexpr auto as_row()const noexcept;
	};


	template < size_t I >
	struct row_t: size_ct< I >{
		using size_ct< I >::size_ct;

		constexpr auto init()const noexcept{
			return row_init_t< true, I >();
		}

		constexpr auto as_col()const noexcept{
			return col_t< I >();
		}
	};

	template <>
	struct row_t< 0 >: size_rt{
		using size_rt::size_rt;

		constexpr auto init()const noexcept;
		constexpr auto as_col()const noexcept;
	};


	template < size_t C, size_t R >
	class dim_t{
	public:
		constexpr dim_t(col_t< C > cols, row_t< R > rows)noexcept:
			cols_(cols), rows_(rows) {}

		constexpr dim_t(dim_t const&)noexcept = default;

		constexpr dim_t(dim_t&&)noexcept = default;


		constexpr dim_t& operator=(dim_t const&)noexcept = default;

		constexpr dim_t& operator=(dim_t&&)noexcept = default;


		constexpr col_t< C > cols()const noexcept{
			return cols_;
		}

		constexpr row_t< R > rows()const noexcept{
			return rows_;
		}


		constexpr void set_cols(col_t< C > c)noexcept{
			cols_ = c;
		}

		constexpr void set_rows(row_t< R > r)noexcept{
			rows_ = r;
		}

		constexpr void set(col_t< C > c, row_t< R > r)noexcept{
			cols_ = c;
			rows_ = r;
		}


	private:
		col_t< C > cols_;
		row_t< R > rows_;
	};


	constexpr auto dims(size_t c, size_t r)noexcept{
		return dim_t< 0, 0 >(col_t< 0 >(c), row_t< 0 >(r));
	}

	template < size_t C >
	constexpr auto dims(col_t< C > c, size_t r)noexcept{
		return dim_t< C, 0 >(c, row_t< 0 >(r));
	}

	template < bool Cct, size_t C >
	constexpr auto dims(col_init_t< Cct, C > c, size_t r)noexcept{
		return dim_t< dim(Cct, C), 0 >(c.get(), row_t< 0 >(r));
	}

	template < bool Cct, size_t C, size_t R >
	constexpr auto dims(col_init_t< Cct, C > c, row_t< R > r)noexcept{
		return dim_t< dim(Cct, C), R >(c.get(), r);
	}

	template < size_t R >
	constexpr auto dims(size_t c, row_t< R > r)noexcept{
		return dim_t< 0, R >(col_t< 0 >(c), r);
	}

	template < bool Rct, size_t R >
	constexpr auto dims(size_t c, row_init_t< Rct, R > r)noexcept{
		return dim_t< 0, dim(Rct, R) >(col_t< 0 >(c), r.get());
	}

	template < size_t C, bool Rct, size_t R >
	constexpr auto dims(col_t< C > c, row_init_t< Rct, R > r)noexcept{
		return dim_t< C, dim(Rct, R) >(c, r.get());
	}

	template < size_t C, size_t R >
	constexpr auto dims(col_t< C > c, row_t< R > r)noexcept{
		return dim_t< C, R >(c, r);
	}

	template < bool Cct, size_t C, bool Rct, size_t R >
	constexpr auto dims(
		col_init_t< Cct, C > c,
		row_init_t< Rct, R > r
	)noexcept{
		return dim_t< dim(Cct, C), dim(Rct, R) >(c.get(), r.get());
	}


	template < size_t I >
	struct col_init_t< true, I >: size_ct< I >{
		constexpr auto get()const noexcept{
			return col_t< I >();
		}

		constexpr auto as_row()const noexcept{
			return row_init_t< true, I >();
		}
	};

	template < size_t I >
	struct col_init_t< false, I >: size_ct< I >{
		constexpr auto get()const noexcept{
			return col_t< 0 >(I);
		}

		constexpr auto as_row()const noexcept{
			return row_init_t< false, I >();
		}
	};

	template <>
	struct col_init_t< false, 0 >: size_rt{
		using size_rt::size_rt;

		constexpr auto get()const noexcept;
		constexpr auto as_row()const noexcept;
	};


	template < size_t I >
	struct row_init_t< true, I >: size_ct< I >{
		constexpr auto get()const noexcept{
			return row_t< I >();
		}

		constexpr auto as_col()const noexcept{
			return col_init_t< true, I >();
		}
	};

	template < size_t I >
	struct row_init_t< false, I >: size_ct< I >{
		constexpr auto get()const noexcept{
			return row_t< 0 >(I);
		}

		constexpr auto as_col()const noexcept{
			return col_init_t< false, I >();
		}
	};

	template <>
	struct row_init_t< false, 0 >: size_rt{
		using size_rt::size_rt;

		constexpr auto get()const noexcept;
		constexpr auto as_col()const noexcept;
	};


	template < size_t I >
	struct dim_init_t< true, I >: size_ct< I >{
		constexpr auto get()const noexcept{
			return dims(col_t< I >(), row_t< I >());
		}

		constexpr auto as_col()const noexcept{
			return col_init_t< true, I >();
		}

		constexpr auto as_row()const noexcept{
			return row_init_t< true, I >();
		}
	};

	template < size_t I >
	struct dim_init_t< false, I >: size_ct< I >{
		constexpr auto get()const noexcept{
			return dims(I, I);
		}

		constexpr auto as_col()const noexcept{
			return col_init_t< false, I >();
		}

		constexpr auto as_row()const noexcept{
			return row_init_t< false, I >();
		}
	};

	template <>
	struct dim_init_t< false, 0 >: size_rt{
		using size_rt::size_rt;

		constexpr auto get()const noexcept;
		constexpr auto as_col()const noexcept;
		constexpr auto as_row()const noexcept;
	};


	constexpr auto col_init_t< false, 0 >::get()const noexcept{
		return col_t< 0 >(static_cast< size_t >(*this));
	}

	constexpr auto col_init_t< false, 0 >::as_row()const noexcept{
		return row_init_t< false, 0 >(static_cast< size_t >(*this));
	}


	constexpr auto row_init_t< false, 0 >::get()const noexcept{
		return row_t< 0 >(static_cast< size_t >(*this));
	}

	constexpr auto row_init_t< false, 0 >::as_col()const noexcept{
		return col_init_t< false, 0 >(static_cast< size_t >(*this));
	}


	constexpr auto dim_init_t< false, 0 >::get()const noexcept{
		auto v = static_cast< size_t >(*this);
		return dims(v, v);
	}

	constexpr auto dim_init_t< false, 0 >::as_col()const noexcept{
		return col_init_t< false, 0 >(static_cast< size_t >(*this));
	}

	constexpr auto dim_init_t< false, 0 >::as_row()const noexcept{
		return row_init_t< false, 0 >(static_cast< size_t >(*this));
	}


	constexpr auto col_t< 0 >::as_row()const noexcept{
		return row_t< 0 >(static_cast< size_t >(*this));
	}

	constexpr auto col_t< 0 >::init()const noexcept{
		return col_init_t< false, 0 >(static_cast< size_t >(*this));
	}


	constexpr auto row_t< 0 >::as_col()const noexcept{
		return col_t< 0 >(static_cast< size_t >(*this));
	}

	constexpr auto row_t< 0 >::init()const noexcept{
		return row_init_t< false, 0 >(static_cast< size_t >(*this));
	}


	constexpr auto cols(size_t c)noexcept{
		return col_init_t< false, 0 >(c);
	}

	constexpr auto rows(size_t r)noexcept{
		return row_init_t< false, 0 >(r);
	}

	constexpr auto dims(size_t d)noexcept{
		return dim_init_t< false, 0 >(d);
	}


	template < size_t I >
	constexpr auto cols()noexcept{
		return col_init_t< true, I >();
	}

	template < size_t I >
	constexpr auto rows()noexcept{
		return row_init_t< true, I >();
	}

	template < size_t I >
	constexpr auto dims()noexcept{
		return dim_init_t< true, I >();
	}


	template < size_t I >
	constexpr auto cols_rt()noexcept{
		return col_init_t< false, I >();
	}

	template < size_t I >
	constexpr auto rows_rt()noexcept{
		return row_init_t< false, I >();
	}

	template < size_t I >
	constexpr auto dims_rt()noexcept{
		return dim_init_t< false, I >();
	}


	namespace detail{


		template < template < size_t > class DimT, typename Op >
		struct col_t_op{
			template < size_t C1, size_t C2 >
			constexpr auto
			operator()(DimT< C1 >, DimT< C2 >)const noexcept{
				return DimT< Op()(C1, C2) >();
			}

			template < size_t C2 >
			constexpr auto
			operator()(DimT< 0 > c1, DimT< C2 > c2)const noexcept{
				return DimT< 0 >(
					Op()(static_cast< size_t >(c1), static_cast< size_t >(c2))
				);
			}

			template < size_t C1 >
			constexpr auto
			operator()(DimT< C1 > c1, DimT< 0 > c2)const noexcept{
				return DimT< 0 >(
					Op()(static_cast< size_t >(c1), static_cast< size_t >(c2))
				);
			}

			constexpr auto
			operator()(DimT< 0 > c1, DimT< 0 > c2)const noexcept{
				return DimT< 0 >(
					Op()(static_cast< size_t >(c1), static_cast< size_t >(c2))
				);
			}
		};


	}


	template < size_t C1, size_t C2 >
	constexpr auto operator+(col_t< C1 > c1, col_t< C2 > c2)noexcept{
		return detail::col_t_op< col_t, std::plus<> >()(c1, c2);
	}

	template < size_t C >
	constexpr auto operator+(size_t c1, col_t< C > c2)noexcept{
		return c1 + static_cast< size_t >(c2);
	}

	template < size_t C >
	constexpr auto operator+(col_t< C > c1, size_t c2)noexcept{
		return static_cast< size_t >(c1) + c2;
	}


	template < size_t C1, size_t C2 >
	constexpr auto operator-(col_t< C1 > c1, col_t< C2 > c2)noexcept{
		return detail::col_t_op< col_t, std::minus<> >()(c1, c2);
	}

	template < size_t C >
	constexpr auto operator-(size_t c1, col_t< C > c2)noexcept{
		return c1 - static_cast< size_t >(c2);
	}

	template < size_t C >
	constexpr auto operator-(col_t< C > c1, size_t c2)noexcept{
		return static_cast< size_t >(c1) - c2;
	}


	template < size_t C1, size_t C2 >
	constexpr auto operator*(col_t< C1 > c1, col_t< C2 > c2)noexcept{
		return detail::col_t_op< col_t, std::multiplies<> >()(c1, c2);
	}

	template < size_t C >
	constexpr auto operator*(size_t c1, col_t< C > c2)noexcept{
		return c1 * static_cast< size_t >(c2);
	}

	template < size_t C >
	constexpr auto operator*(col_t< C > c1, size_t c2)noexcept{
		return static_cast< size_t >(c1) * c2;
	}


	template < size_t C1, size_t C2 >
	constexpr auto operator/(col_t< C1 > c1, col_t< C2 > c2)noexcept{
		return detail::col_t_op< col_t, std::divides<> >()(c1, c2);
	}

	template < size_t C >
	constexpr auto operator/(size_t c1, col_t< C > c2)noexcept{
		return c1 / static_cast< size_t >(c2);
	}

	template < size_t C >
	constexpr auto operator/(col_t< C > c1, size_t c2)noexcept{
		return static_cast< size_t >(c1) / c2;
	}


	template < size_t C1, size_t C2 >
	constexpr auto operator%(col_t< C1 > c1, col_t< C2 > c2)noexcept{
		return detail::col_t_op< col_t, std::modulus<> >()(c1, c2);
	}

	template < size_t C >
	constexpr auto operator%(size_t c1, col_t< C > c2)noexcept{
		return c1 % static_cast< size_t >(c2);
	}

	template < size_t C >
	constexpr auto operator%(col_t< C > c1, size_t c2)noexcept{
		return static_cast< size_t >(c1) % c2;
	}


	template < size_t R1, size_t R2 >
	constexpr auto operator+(row_t< R1 > r1, row_t< R2 > r2)noexcept{
		return detail::col_t_op< row_t, std::plus<> >()(r1, r2);
	}

	template < size_t R >
	constexpr auto operator+(size_t r1, row_t< R > r2)noexcept{
		return r1 + static_cast< size_t >(r2);
	}

	template < size_t R >
	constexpr auto operator+(row_t< R > r1, size_t r2)noexcept{
		return static_cast< size_t >(r1) + r2;
	}


	template < size_t R1, size_t R2 >
	constexpr auto operator-(row_t< R1 > r1, row_t< R2 > r2)noexcept{
		return detail::col_t_op< row_t, std::minus<> >()(r1, r2);
	}

	template < size_t R >
	constexpr auto operator-(size_t r1, row_t< R > r2)noexcept{
		return r1 - static_cast< size_t >(r2);
	}

	template < size_t R >
	constexpr auto operator-(row_t< R > r1, size_t r2)noexcept{
		return static_cast< size_t >(r1) - r2;
	}


	template < size_t R1, size_t R2 >
	constexpr auto operator*(row_t< R1 > r1, row_t< R2 > r2)noexcept{
		return detail::col_t_op< row_t, std::multiplies<> >()(r1, r2);
	}

	template < size_t R >
	constexpr auto operator*(size_t r1, row_t< R > r2)noexcept{
		return r1 * static_cast< size_t >(r2);
	}

	template < size_t R >
	constexpr auto operator*(row_t< R > r1, size_t r2)noexcept{
		return static_cast< size_t >(r1) * r2;
	}


	template < size_t R1, size_t R2 >
	constexpr auto operator/(row_t< R1 > r1, row_t< R2 > r2)noexcept{
		return detail::col_t_op< row_t, std::divides<> >()(r1, r2);
	}

	template < size_t R >
	constexpr auto operator/(size_t r1, row_t< R > r2)noexcept{
		return r1 / static_cast< size_t >(r2);
	}

	template < size_t R >
	constexpr auto operator/(row_t< R > r1, size_t r2)noexcept{
		return static_cast< size_t >(r1) / r2;
	}


	template < size_t R1, size_t R2 >
	constexpr auto operator%(row_t< R1 > r1, row_t< R2 > r2)noexcept{
		return detail::col_t_op< row_t, std::modulus<> >()(r1, r2);
	}

	template < size_t R >
	constexpr auto operator%(size_t r1, row_t< R > r2)noexcept{
		return r1 % static_cast< size_t >(r2);
	}

	template < size_t R >
	constexpr auto operator%(row_t< R > r1, size_t r2)noexcept{
		return static_cast< size_t >(r1) % r2;
	}


	template < size_t C1, size_t R1, size_t C2, size_t R2 >
	constexpr bool operator==(dim_t< C1, R1 > a, dim_t< C2, R2 > b)noexcept{
		static_assert(
			(C1 == 0 || C2 == 0 || C1 == C2) &&
			(R1 == 0 || R2 == 0 || R1 == R2),
			"Matrix dimensions are not compatible"
		);

		return a.cols() == b.cols() || a.rows() == b.rows();
	}

	template < size_t C1, size_t R1, size_t C2, size_t R2 >
	constexpr bool operator!=(dim_t< C1, R1 > a, dim_t< C2, R2 > b)noexcept{
		return !(a == b);
	}


	namespace detail{


		// Adopted from Boost Hana
		template < size_t N >
		constexpr size_t parse_int(char const(&arr)[N])noexcept{
			size_t number = 0;
			size_t base = 1;
			for(size_t i = 0; i < N; ++i){
				number += (static_cast< int >(arr[N - 1 - i]) - 48) * base;
				base *= 10;
			}
			return number;
		}


	}


	namespace literals{


		// Adopted from Boost Hana
		template < char ... C >
		constexpr auto operator"" _C()noexcept{
			static_assert(
				detail::parse_int< sizeof...(C) >({C ...}),
				"Compile time cols can not be 0, use '_C_rt' prefix instead"
			);
			return cols< detail::parse_int< sizeof...(C) >({C ...}) >();
		}

		// Adopted from Boost Hana
		template < char ... C >
		constexpr auto operator"" _C_rt()noexcept{
			return cols_rt< detail::parse_int< sizeof...(C) >({C ...}) >();
		}

		// Adopted from Boost Hana
		template < char ... C >
		constexpr auto operator"" _R()noexcept{
			static_assert(
				detail::parse_int< sizeof...(C) >({C ...}),
				"Compile time rows can not be 0, use '_R_rt' prefix instead"
			);
			return rows< detail::parse_int< sizeof...(C) >({C ...}) >();
		}

		// Adopted from Boost Hana
		template < char ... C >
		constexpr auto operator"" _R_rt()noexcept{
			return rows_rt< detail::parse_int< sizeof...(C) >({C ...}) >();
		}

		// Adopted from Boost Hana
		template < char ... C >
		constexpr auto operator"" _D()noexcept{
			static_assert(
				detail::parse_int< sizeof...(C) >({C ...}),
				"Compile time dims can not be 0, use '_D_rt' prefix instead"
			);
			return dims< detail::parse_int< sizeof...(C) >({C ...}) >();
		}

		// Adopted from Boost Hana
		template < char ... C >
		constexpr auto operator"" _D_rt()noexcept{
			return dims_rt< detail::parse_int< sizeof...(C) >({C ...}) >();
		}


	}


}


#endif
