/**************************************************************************
This file is part of Matrice, an effcient and elegant C++ library.
Copyright(C) 2018-2019, Zhilong(Dgelom) Su, all rights reserved.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#pragma once
#include <array>
#include <private/_plain_base.hpp>
#ifdef MATRICE_ENABLE_CUDA
#include <private/_dev_matrix_base.h>
#endif

MATRICE_NAMESPACE_BEGIN_TYPES

#define MATRICE_MAKE_METHOD_CREATE(M, N) \
MATRICE_GLOBAL void __create_impl(size_t M, size_t N)

/*******************************************************************
	Generic Matrix Class with Aligned Static Memory Allocation
	       Copyright (c) : Zhilong Su, since 14/Feb/2018
*******************************************************************/
template<typename _Ty, int M, int N = M>
class Matrix_ : public Base_<Matrix_<_Ty, M, N>>
{
	using _Myt = Matrix_;
	using _Mybase = Base_<Matrix_<_Ty, M, N>>;
public:
	using typename _Mybase::value_t;
	using typename _Mybase::pointer;
	using typename _Mybase::const_initlist;
	using _Mybase::Size;
	using _Mybase::rows_at_compiletime;
	using _Mybase::cols_at_compiletime;

	MATRICE_GLOBAL_FINL constexpr Matrix_() noexcept
		: _Mybase() {};
	MATRICE_GLOBAL_FINL constexpr Matrix_(int, int) noexcept
		: Matrix_() {};
	MATRICE_GLOBAL_FINL constexpr Matrix_(pointer data) noexcept
		: _Mybase(rows_at_compiletime, cols_at_compiletime, data) {};
	MATRICE_HOST_FINL constexpr Matrix_(const_initlist _list) noexcept
		: _Mybase(_list) {}
	MATRICE_GLOBAL_FINL constexpr Matrix_(_Myt&& _other) noexcept
		: _Mybase((_other)) {}
	MATRICE_GLOBAL_FINL constexpr Matrix_(const _Myt& _other) noexcept
		: _Mybase(_other) {};

	template<typename _Uy, MATRICE_ENABLE_IF(is_scalar_v<_Uy>)>
	MATRICE_GLOBAL_FINL constexpr Matrix_(_Uy _val) noexcept
		: _Mybase(rows_at_compiletime, cols_at_compiletime, _val) {};
	template<typename _Uy>
	MATRICE_HOST_FINL constexpr Matrix_(const nested_initlist<_Uy> _list) noexcept
		: _Mybase(_list) {}
	MATRICE_HOST_FINL constexpr Matrix_(const std::array<value_t, Size>& _array) noexcept
		: Matrix_(pointer(_array.data())) {}
	template<typename... _Args> 
	MATRICE_GLOBAL_FINL constexpr Matrix_(_Args&&... args)noexcept
		: _Mybase(forward<_Args>(args)...) {};

	MATRICE_HOST_FINL _Myt& operator=(const_initlist _list)noexcept {
		return _Mybase::operator=(_list); 
	}
	MATRICE_GLOBAL_FINL _Myt& operator=(const _Myt& _other)noexcept {
		return _Mybase::operator=(_other);
	}
	MATRICE_GLOBAL_FINL _Myt& operator=(_Myt&& _other)noexcept {
		return _Mybase::operator=(move(_other)); 
	}
	template<typename _Uy>
	MATRICE_HOST_FINL _Myt& operator=(nested_initlist<_Uy> _list)noexcept {
		return _Mybase::operator=(_list);
	}
	template<typename _Argt>
	MATRICE_HOST_INL _Myt& operator=(const _Argt& _arg)noexcept {
		return _Mybase::operator=(_arg);
	}
	
	MATRICE_GLOBAL_FINL constexpr auto(size)() const noexcept { 
		return (Size);
	}
	MATRICE_GLOBAL_FINL constexpr auto(rows)() const noexcept { 
		return (rows_at_compiletime);
	}
	MATRICE_GLOBAL_FINL constexpr auto(cols)() const noexcept { 
		return (cols_at_compiletime);
	}

	MATRICE_GLOBAL_FINL operator std::array<value_t, Size>() const noexcept {
		return internal::_Fill_array<value_t, Size>(_Mybase::begin());
	}
	MATRICE_GLOBAL_FINL operator std::array<value_t, Size>() noexcept {
		return internal::_Fill_array<value_t, Size>(_Mybase::begin());
	}
	MATRICE_GLOBAL_FINL operator Matrix_<value_t, ::dynamic>() const noexcept {
		return Matrix_<value_t, ::dynamic>(rows(), cols(), _Mybase::m_data); 
	}
	MATRICE_GLOBAL_FINL operator Matrix_<value_t, ::dynamic>() noexcept {
		return Matrix_<value_t, ::dynamic>(rows(), cols(), _Mybase::m_data); 
	}
};

template<typename _Ty, size_t Rows>
class Matrix_<_Ty, Rows, ::dynamic> 
	:public Base_<Matrix_<_Ty, Rows, ::dynamic>> {
	using _Myt = Matrix_<_Ty, Rows, ::dynamic>;
	using _Mybase = Base_<_Myt>;
public:
	using typename _Mybase::value_t;
	using typename _Mybase::pointer;
	using typename _Mybase::const_initlist;
	using _Mybase::operator=;

	MATRICE_HOST_INL Matrix_(diff_t cols)
		: _Mybase(_Mybase::rows_at_compiletime, cols) {};
	MATRICE_HOST_INL Matrix_(diff_t, diff_t cols)
		: Matrix_(cols) {};
	MATRICE_HOST_INL Matrix_(const _Myt& other)
		: _Mybase(other) {};
	MATRICE_HOST_INL Matrix_(_Myt&& other) noexcept
		: _Mybase(move(other)) {};
	template<typename _Argt>
	MATRICE_HOST_INL Matrix_(_Argt&& arg) noexcept
		: _Mybase(forward<_Argt>(arg)) {};

	MATRICE_HOST_INL _Myt& operator= (const _Myt& other) {
		return _Mybase::operator=(other);
	}
	MATRICE_HOST_INL _Myt& operator= (_Myt&& other) noexcept {
		return _Mybase::operator=(move(other));
	}
	template<typename _Argt>
	MATRICE_HOST_INL _Myt& operator=(const _Argt& _arg) {
		return _Mybase::operator=(_arg);
	}

	MATRICE_GLOBAL_FINL constexpr auto(rows)() const noexcept {
		return (_Mybase::rows_at_compiletime);
	}

	MATRICE_MAKE_METHOD_CREATE(cols, ) {
		_Mybase::_Myalloc.alloc(rows(), cols);
		_Mybase::_Xfields({ size_t(rows()), cols });
	}
};

template<typename _Ty, size_t Cols>
class Matrix_<_Ty, ::dynamic, Cols> 
	:public Base_<Matrix_<_Ty, ::dynamic, Cols>> {
	using _Myt = Matrix_<_Ty, ::dynamic, Cols>;
	using _Mybase = Base_<_Myt>;
public:
	using typename _Mybase::value_t;
	using typename _Mybase::pointer;
	using typename _Mybase::const_initlist;
	using _Mybase::operator=;

	MATRICE_HOST_INL Matrix_(diff_t rows)
		: _Mybase(rows, _Mybase::cols_at_compiletime) {};
	MATRICE_HOST_INL Matrix_(diff_t rows, diff_t)
		: Matrix_(rows) {};
	MATRICE_HOST_INL Matrix_(const _Myt& other)
		: _Mybase(other) {};
	MATRICE_HOST_INL Matrix_(_Myt&& other) noexcept
		: _Mybase(move(other)) {};
	template<typename _Argt>
	MATRICE_HOST_INL Matrix_(_Argt&& arg) noexcept
		: _Mybase(forward<_Argt>(arg)) {};

	MATRICE_HOST_INL _Myt& operator= (const _Myt& other) {
		return _Mybase::operator=(other);
	}
	MATRICE_HOST_INL _Myt& operator= (_Myt&& other) noexcept {
		return _Mybase::operator=(move(other));
	}
	template<typename _Argt>
	MATRICE_HOST_INL _Myt& operator=(const _Argt& _arg) {
		return _Mybase::operator=(_arg);
	}

	MATRICE_GLOBAL_FINL constexpr auto(cols)() const noexcept { 
		return (_Mybase::cols_at_compiletime);
	}

	MATRICE_MAKE_METHOD_CREATE(rows, ) {
		_Mybase::_Myalloc.alloc(rows, cols());
		_Mybase::_Xfields({ rows, size_t(cols()) });
	}
};

/*******************************************************************
    Generic Matrix Class with Aligned Dynamic Memory Allocation
	         Copyright (c) : Zhilong Su since 14/Feb/2018
 ******************************************************************/
template<typename _Ty>
class Matrix_<_Ty, ::dynamic> : public Base_<Matrix_<_Ty, ::dynamic>>
{
	using _Myt = Matrix_<_Ty, ::dynamic>;
	using _Mybase = Base_<_Myt>;
public:
	using typename _Mybase::value_t;
	using typename _Mybase::pointer;
	using typename _Mybase::const_initlist;
	//enum { Size = ::dynamic, rows_at_compiletime = ::dynamic, cols_at_compiletime = ::dynamic, };
	
	MATRICE_HOST_INL Matrix_(diff_t rows) noexcept
		: _Mybase(rows, 1) {};
	MATRICE_HOST_INL Matrix_(const _Myt& other) noexcept
		: _Mybase(other) {};
	template<typename _Arg>
	MATRICE_HOST_INL Matrix_(_Arg&& other) noexcept
		: _Mybase(move(other)) {};
	template<typename... _Args> 
	MATRICE_HOST_INL Matrix_(_Args&&... args) noexcept
		: _Mybase(forward<_Args>(args)...) {};

	MATRICE_HOST_INL _Myt& operator= (const _Myt& other) {
		return _Mybase::operator=(other); 
	}
	MATRICE_HOST_INL _Myt& operator= (_Myt&& other) noexcept {
		return _Mybase::operator=(move(other)); 
	}
	MATRICE_HOST_INL _Myt& operator= (const_initlist list) {
		return _Mybase::operator=(list); 
	}
	
	using _Mybase::operator=;

	MATRICE_MAKE_METHOD_CREATE(rows, cols = 1);
};


#ifdef MATRICE_ENABLE_CUDA
/*******************************************************************
    Generic Matrix Class with Unifief Memory Allocation (CUDA)
	         Copyright (c) : Zhilong Su 24/May/2018
 ******************************************************************/
template<typename _Ty>
class Matrix_<_Ty, ::global> : public Base_<Matrix_<_Ty, ::global>>
{
	using Myt = Matrix_<_Ty, ::global>;
	using Myt_reference = std::add_lvalue_reference_t<Myt>;
	using Myt_const_reference = add_const_reference_t<Myt>;
	using Myt_move_reference = std::add_rvalue_reference_t<Myt>;
	using _Mybase = Base_<Matrix_<_Ty, ::global>>;
public:
	using typename _Mybase::value_t;
	using typename _Mybase::pointer;
	using typename _Mybase::const_initlist;
	enum { Size = ::dynamic, rows_at_compiletime = 0, cols_at_compiletime = 0, };
	MATRICE_GLOBAL_INL Matrix_(int _rows) noexcept 
		: _Mybase(_rows, 1) {};
	MATRICE_GLOBAL_INL Matrix_(Myt_move_reference _other) noexcept 
		: _Mybase(move(_other)) {};
	template<typename... _Args> 
	MATRICE_GLOBAL_INL Matrix_(_Args&&... args) noexcept 
		: _Mybase(forward<_Args>(args)...) {};

	template<typename _Arg> 
	MATRICE_GLOBAL_INL Myt_reference operator= (add_const_reference<_Arg> _arg) { 
		return _Mybase::operator=(_arg); 
	}
	MATRICE_GLOBAL_INL Myt_reference operator= (Myt_move_reference _other) {
		return _Mybase::operator=(move(_other)); 
	}
	MATRICE_HOST_INL Myt_reference operator= (const_initlist _list) {
		return _Mybase::operator=(_list); 
	}

	MATRICE_MAKE_METHOD_CREATE(rows, cols = 1);
};


/*******************************************************************
    Generic Matrix Class with Device Memory Allocation (CUDA)
	         Copyright (c) : Zhilong Su 25/May/2018
 ******************************************************************/
template<typename _Ty>
class Matrix_<_Ty, ::device> : public Base_<Matrix_<_Ty, ::device>>, public device::Base_<_Ty>
{
	using _Myt = Matrix_;
	using Myt_reference = std::add_lvalue_reference_t<_Myt>;
	using Myt_const_reference = add_const_reference_t<_Myt>;
	using Myt_move_reference = std::add_rvalue_reference_t<_Myt>;
	using _Mydevbase = device::Base_<_Ty>;
	using _Mybase = Base_<Matrix_<_Ty, ::device>>;
	using _Mybase::m_data;
	using _Mybase::m_rows;
	using _Mybase::m_cols;
	size_t m_pitch = _Mybase::m_storage.pitch();
public:
	enum { Size = 0, rows_at_compiletime = ::device, cols_at_compiletime = ::device, };
	using typename _Mybase::value_t;
	using typename _Mybase::value_type;
	using typename _Mybase::pointer;
	using typename _Mybase::const_initlist;
	using host_t = Matrix_<value_t, ::dynamic>;
	
	MATRICE_GLOBAL_INL Matrix_(int _rows) noexcept 
		:_Mybase(_rows, 1), _Mydevbase(m_data, &m_pitch, &m_cols, &m_rows) {};
	MATRICE_GLOBAL_INL Matrix_(Myt_const_reference _other) noexcept 
		:_Mybase(_other), _Mydevbase(m_data, &m_pitch, &m_cols, &m_rows) {};
	MATRICE_GLOBAL_INL Matrix_(Myt_move_reference _other) noexcept 
		:_Mybase(move(_other)), _Mydevbase(m_data, &m_pitch, &m_cols, &m_rows) {};
	MATRICE_GLOBAL_INL Matrix_(const host_t& _other) noexcept
		:_Mybase(_other), _Mydevbase(m_data, &m_pitch, &m_cols, &m_rows) {}
	template<int _M = 0, int _N = _M>
	MATRICE_HOST_INL Matrix_(const Matrix_<value_t, _M, _N>&_other)noexcept
		:_Mybase(_other), _Mydevbase(m_data, &m_pitch, &m_cols, &m_rows) {};
	template<typename... _Args>
	MATRICE_GLOBAL_INL Matrix_(_Args&&... args) noexcept 
		:_Mybase(forward<_Args>(args)...), _Mydevbase(m_data, &m_pitch, &m_cols, &m_rows) {};

	MATRICE_MAKE_METHOD_CREATE(rows, cols=1);

	/**
	 *\sync all device threads, it should be called before downloading data to host memory
	 */
	MATRICE_HOST_INL _Myt& sync(){ this->_Sync_impl(); return(*this); }

	template<typename _Arg> 
	MATRICE_GLOBAL_INL Myt_reference operator= (const _Arg& _arg) { 
		return _Mydevbase::operator=(_arg); 
	}
	MATRICE_DEVICE_INL Myt_reference operator= (Myt_const_reference _other) { 
		return _Mydevbase::operator=(_other); 
	}
	MATRICE_DEVICE_INL Myt_reference operator= (Myt_move_reference _other) { 
		return _Mydevbase::operator=(move(_other)); 
	}
	MATRICE_GLOBAL_INL Myt_reference operator= (const_initlist _list) { 
		_Mydevbase::operator=(const_cast<pointer>(_list.begin()));
		return (*this);
	}

	/**
	 * \in-place element-wise addition
	 */
	MATRICE_GLOBAL_INL Myt_reference operator+(Myt_const_reference _other) {
		m_data = (*this)._Mydevbase::operator+(_other); return (*this);
	}
	/**
	 * \in-place element-wise subtraction
	 */
	MATRICE_GLOBAL_INL Myt_reference operator-(Myt_const_reference _other) {
		m_data = (*this)._Mydevbase::operator-(_other); return (*this);
	}
	/**
	 * \in-place element-wise multiplication
	 */
	MATRICE_GLOBAL_INL Myt_reference operator*(Myt_const_reference _other) {
		m_data = (*this)._Mydevbase::operator*(_other); return (*this);
	}
	/**
	 * \in-place element-wise division
	 */
	MATRICE_GLOBAL_INL Myt_reference operator/(Myt_const_reference _other) {
		m_data = (*this)._Mydevbase::operator/(_other); return (*this);
	}

};

#endif
#undef MATRICE_MAKE_METHOD_CREATE
MATRICE_NAMESPACE_END_TYPES

DGE_MATRICE_BEGIN
//\matrix type with host managed memory allocator
template<typename T, int _M, int _N=_M, 
	size_t _Options = rmaj|gene, 
	MATRICE_ENABLE_IF(is_arithmetic_v<T>)>
using Matrix_ = types::Matrix_<T, _M, _N>;

//\matrix type with host dynamic memory allocator
template<typename T, size_t _Options = rmaj | gene> using Matrix = Matrix_<T,
	compile_time_size<>::RunTimeDeducedOnHost,
	compile_time_size<>::RunTimeDeducedOnHost,
	_Options>;

//\matrix type with unified memory allocator
template<typename T, size_t _Options = rmaj | gene> using Umatrix = Matrix_<T,
	compile_time_size<>::RunTimeDeducedOnDevice,
	compile_time_size<>::RunTimeDeducedOnHost,
	_Options>;

//\matrix type with device memory allocator
template<typename T, size_t _Options = rmaj | gene> using Dmatrix = Matrix_<T,
	compile_time_size<>::RunTimeDeducedOnDevice,
	compile_time_size<>::RunTimeDeducedOnDevice,
	_Options>;

//\dynamic matrix type with single(32)/double(64) floating point type
using matrix_f32 = Matrix<float>;
using matrix_f64 = Matrix<double>;

//\N-dimensional array with host managed memory
template<typename T, int _N> using array_n = Matrix_<T, _N, 1>;

DGE_MATRICE_END
