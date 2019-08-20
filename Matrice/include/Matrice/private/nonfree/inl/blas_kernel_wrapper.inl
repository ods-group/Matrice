/**************************************************************************
This file is part of Matrice, an effcient and elegant C++ library.
Copyright(C) 2018-2019, Zhilong(Dgelom) Su, all rights reserved.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for
more detail.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#pragma once
#include "../../math/kernel_wrapper.hpp"
#include "../../math/_config.h"
#include <util/_exception.h>

DGE_MATRICE_BEGIN
_INTERNAL_BEGIN
template<typename _Ptr>
decltype(auto) _blas_asum(const _Ptr x, size_t n, int inc) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
template<typename _Ty, typename _Ptr = add_pointer_t<_Ty>>
decltype(auto) _blas_axpy(const _Ty a, const _Ptr x, _Ptr y, size_t n, int incx, int incy) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
template<typename _Ptr>
decltype(auto) _blas_dot(size_t n, const _Ptr x, int incx, const _Ptr y, int incy) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
template<typename _Ptr>
decltype(auto) _blas_nrm2(size_t n, const _Ptr x, int inc) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
template<typename _Ptr, typename _Ty = typename std::pointer_traits<_Ptr>::element_type>
decltype(auto) _blas_rot(size_t n, _Ptr x, int incx, _Ptr y, int incy, _Ty c, _Ty s) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
template<typename _Ty, typename _Ptr = add_pointer_t<_Ty>>
decltype(auto) _blas_scal(size_t n, const _Ty a, _Ptr x, int inc) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
template<typename _Ty, typename _Ptr = add_pointer_t<_Ty>>
decltype(auto) _blas_gemv(size_t m, size_t n, _Ty a, const _Ptr A, const _Ptr x, int incx, _Ty b, _Ptr y, size_t incy) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
template<typename _Ty, typename _Ptr = add_pointer_t<_Ty>>
decltype(auto) _blas_gemtv(size_t m, size_t n, _Ty a, const _Ptr A, const _Ptr x, int incx, _Ty b, _Ptr y, size_t incy) {
	MATRICE_FAIL_TO_SPECIALIZATION
};
_INTERNAL_END

_DETAIL_BEGIN
struct _Blas_kernel_wrapper {
	template<class _Aty, typename _Ret = typename _Aty::value_type>
	static MATRICE_HOST_INL _Ret reduce(const _Aty & a, int inc = 1) noexcept {
		return internal::_blas_asum(a.data(), a.size(), inc);
	}

	template<class _Xty, class _Yty = _Xty,
		typename = enable_if_t<is_same_v<typename _Xty::value_type, typename _Yty::value_type>>>
	static MATRICE_HOST_INL _Yty& axpy(const _Xty& x, _Yty& y, typename _Xty::value_type a = 1) noexcept {
		internal::_blas_axpy(a, x.data(), y.data(), min(x.size(), y.size()), 1, 1);
		return (y);
	}
	
	template<class _Xty, class _Yty = _Xty,
		typename _Ret = common_type_t<typename _Xty::value_type, typename _Yty::value_type>>
	static MATRICE_HOST_INL _Ret dot(const _Xty & x, const _Yty & y, int incx = 1, int incy = 1) noexcept {
		const auto size = min(x.size() / incx, y.size() / incy);
		return internal::_blas_dot(size, x.data(), incx, y.data(), incy);
	}

	template<class _Aty, typename _Ret = typename _Aty::value_type>
	static MATRICE_HOST_INL _Ret norm2(const _Aty & a, int inc = 1) noexcept {
		return internal::_blas_nrm2(a.size(), a.data(), inc);
	}

	/**
	 *\brief in-plane rotation of points. 
	 For a = {x_0, y_0, x_1, y_1, ..., x_n, y_n}, performs a <- [c, s \\ -s, c]*a
	 */
	template<class _Aty, typename _Ty = typename _Aty::valye_type>
	static MATRICE_HOST_INL _Aty& rot(_Aty & a, _Ty c, _Ty s) noexcept {
		internal::_blas_rot(a.size(), a.data(), 2, a.data(), 2, c, s);
		return (a);
	}

	template<class _Aty, typename _Ty = typename _Aty::valye_type>
	static MATRICE_HOST_INL _Aty& scale(_Ty s, _Aty& a) noexcept {
		internal::_blas_scal(a.size(), s, a.data(), 1);
		return (a);
	}

	template<class _Aty, typename _Ity, typename _Oty = _Ity>
	static MATRICE_HOST_INL _Oty& gemv(const _Aty& a, const _Ity& x, _Oty& y) noexcept {
		constexpr auto _1 = typename _Aty::value_type(1);
		constexpr auto _0 = typename _Aty::value_type(0);
		if (a.cols() == x.size())
			internal::_blas_gemv(a.rows(), a.cols(), _1, a.data(), x.data(), 1, _0, y.data(), 1);
		else if (a.rows() == x.size())
			internal::_blas_gemtv(a.rows(), a.cols(), _1, a.data(), x.data(), 1, _0, y.data(), 1);
		else DGELOM_ERROR("The shape of a and x in gemv is not compatible.");
			
		return (y);
	}
};
_DETAIL_END

_INTERNAL_BEGIN
using f32_t = float;
using f64_t = double;
using fptr = std::add_pointer_t<f32_t>;
using dptr = std::add_pointer_t<double>;
template<>
decltype(auto) _blas_asum(const fptr x, size_t n, int inc) {
	return cblas_sasum(MKL_INT(n), x, MKL_INT(inc));
}
template<>
decltype(auto) _blas_asum(const dptr x, size_t n, int inc) {
	return cblas_dasum(MKL_INT(n), x, MKL_INT(inc));
}

template<>
decltype(auto) _blas_axpy(const f32_t a, const fptr x, fptr y, size_t n, int incx, int incy) {
	return cblas_saxpy(n, a, x, incx, y, incy);
}
template<>
decltype(auto) _blas_axpy(const f64_t a, const dptr x, dptr y, size_t n, int incx, int incy) {
	return cblas_daxpy(n, a, x, incx, y, incy);
}

template<>
decltype(auto) _blas_dot(size_t n, const fptr x, int incx, const fptr y, int incy) {
	return cblas_sdot(n, x, incx, y, incy);
}
template<>
decltype(auto) _blas_dot(size_t n, const dptr x, int incx, const dptr y, int incy) {
	return cblas_ddot(n, x, incx, y, incy);
}

template<>
decltype(auto) _blas_nrm2(size_t n, const fptr x, int inc) {
	return cblas_snrm2(n, x, inc);
}
template<>
decltype(auto) _blas_nrm2(size_t n, const dptr x, int inc) {
	return cblas_dnrm2(n, x, inc);
}

template<>
decltype(auto) _blas_scal(size_t n, const f32_t a, fptr x, int inc) {
	return cblas_sscal(n, a, x, inc);
}
template<>
decltype(auto) _blas_scal(size_t n, const f64_t a, dptr x, int inc) {
	return cblas_dscal(n, a, x, inc);
}

template<>
decltype(auto) _blas_gemv(size_t m, size_t n, f32_t a, const fptr A, const fptr x, int incx, f32_t b, fptr y, size_t incy) {
	return cblas_sgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasNoTrans, m, n, a, A, (n > 1 ? n : 1), x, incx, b, y, incy);
}
template<>
decltype(auto) _blas_gemv(size_t m, size_t n, f64_t a, const dptr A, const dptr x, int incx, f64_t b, dptr y, size_t incy) {
	return cblas_dgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasNoTrans, m, n, a, A, (n > 1 ? n : 1), x, incx, b, y, incy);
}
template<>
decltype(auto) _blas_gemtv(size_t m, size_t n, f32_t a, const fptr A, const fptr x, int incx, f32_t b, fptr y, size_t incy) {
	return cblas_sgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasTrans, m, n, a, A, (m > 1 ? m : 1), x, incx, b, y, incy);
}
template<>
decltype(auto) _blas_gemtv(size_t m, size_t n, f64_t a, const dptr A, const dptr x, int incx, f64_t b, dptr y, size_t incy) {
	return cblas_dgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasTrans, m, n, a, A, (m > 1 ? m : 1), x, incx, b, y, incy);
}
_INTERNAL_END
DGE_MATRICE_END
