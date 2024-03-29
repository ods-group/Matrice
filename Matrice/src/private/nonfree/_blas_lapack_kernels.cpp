#include <private/math/_config.h>
#include <private/nonfree/inl/blas_kernel_wrapper.inl>

DGE_MATRICE_BEGIN
_INTERNAL_BEGIN
using f32_t = float;
using f64_t = double;
using fptr = f32_t*;
using dptr = f64_t*;

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
void _blas_gemv(size_t m, size_t n, f32_t a, const fptr A, const fptr x, int incx, f32_t b, fptr y, size_t incy) {
	return cblas_sgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasNoTrans, m, n, a, A, (n > 1 ? n : 1), x, incx, b, y, incy);
}
template<>
void _blas_gemv(size_t m, size_t n, f64_t a, const dptr A, const dptr x, int incx, f64_t b, dptr y, size_t incy) {
	return cblas_dgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasNoTrans, m, n, a, A, (n > 1 ? n : 1), x, incx, b, y, incy);
}
template<>
void _blas_gemtv(size_t m, size_t n, f32_t a, const fptr A, const fptr x, int incx, f32_t b, fptr y, size_t incy) {
	return cblas_sgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasTrans, m, n, a, A, (m > 1 ? m : 1), x, incx, b, y, incy);
}
template<>
void _blas_gemtv(size_t m, size_t n, f64_t a, const dptr A, const dptr x, int incx, f64_t b, dptr y, size_t incy) {
	return cblas_dgemv(CBLAS_LAYOUT::CblasRowMajor, CBLAS_TRANSPOSE::CblasTrans, m, n, a, A, (m > 1 ? m : 1), x, incx, b, y, incy);
}
template<>
void _blas_gemm(int lyt, int trpa, int trpb, size_t m, size_t n, size_t k, f32_t a, const fptr A, size_t lda, const fptr B, size_t ldb, f32_t b, fptr C, size_t ldc) {
	return cblas_sgemm(CBLAS_LAYOUT(lyt), CBLAS_TRANSPOSE(trpa), CBLAS_TRANSPOSE(trpb), MKL_INT(m), MKL_INT(n), MKL_INT(k), a, A, lda, B, ldb, b, C, ldc);
}
template<>
void _blas_gemm(int lyt, int trpa, int trpb, size_t m, size_t n, size_t k, f64_t a, const dptr A, size_t lda, const dptr B, size_t ldb, f64_t b, dptr C, size_t ldc) {
	return cblas_dgemm(CBLAS_LAYOUT(lyt), CBLAS_TRANSPOSE(trpa), CBLAS_TRANSPOSE(trpb), MKL_INT(m), MKL_INT(n), MKL_INT(k), a, A, lda, B, ldb, b, C, ldc);
}
_INTERNAL_END
DGE_MATRICE_END