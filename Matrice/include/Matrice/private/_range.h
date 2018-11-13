#pragma once
#include "../core/vector.h"

DGE_MATRICE_BEGIN namespace detail {

// \class range base : [begin, end)
template<typename _Ty, 
	typename  = std::enable_if<std::is_arithmetic_v<_Ty>>>
class _Range_base {
public:
	using value_t = _Ty;
	using const_value_t = std::add_const_t<value_t>;

	class _My_iterator {
	public:
		MATRICE_GLOBAL_INL _My_iterator(const_value_t& _Pos, const_value_t&_Inc = 1) : _My_pos(_Pos), _My_inc(_Inc) {}

		MATRICE_GLOBAL_INL value_t& operator++() { 
			return _My_pos += _My_inc; 
		}
		MATRICE_GLOBAL_INL const value_t& operator++() const {
			return _My_pos += _My_inc; 
		}

		MATRICE_GLOBAL_INL value_t& operator*() { return _My_pos; }
		MATRICE_GLOBAL_INL const value_t& operator*() const { return _My_pos; }

		MATRICE_GLOBAL_INL auto operator != (const _My_iterator& _Other)const {
			return _My_pos < (_Other._My_pos);
		}

		MATRICE_GLOBAL_INL operator value_t() const { return _My_pos; }

	private:
		value_t _My_pos, _My_inc;
	};

	_Range_base(const_value_t& _Begin, const_value_t& _End, const_value_t& _Stride = 1) noexcept
		: m_begin(_Begin), m_end(_End), m_stride(_Stride) {}
	_Range_base(const _Range_base& _other) noexcept
		:m_begin(_other.m_begin), m_end(_other.m_end), m_stride(_other.m_stride) {}

	MATRICE_GLOBAL_INL _Range_base& operator= (const _Range_base& _other) {
		m_begin = _other.m_begin, m_end = _other.m_end;
		m_stride = _other.m_stride;
		return (*this);
	}
	MATRICE_GLOBAL_INL _Range_base& operator= (const std::initializer_list<value_t> _L) {
		m_begin = *_L.begin(), m_end = *(_L.begin()+1);
		if (_L.size() == 2) m_stride = value_t(1);
		if (_L.size() == 3) m_stride = *(_L.begin() + 2);
		return (*this);
	}

	MATRICE_GLOBAL_INL value_t operator[](index_t i) { 
		return (m_pos = m_begin + i * m_stride); 
	}
	MATRICE_GLOBAL_INL const_value_t operator[](index_t i) const { 
		return (m_pos = m_begin + i * m_stride); 
	}
	MATRICE_GLOBAL_INL value_t operator()(index_t i) { 
		return operator[](i); 
	}
	MATRICE_GLOBAL_INL const_value_t operator()(index_t i) const { 
		return operator[](i); 
	}

	MATRICE_GLOBAL_INL value_t& value() { return m_pos; }
	MATRICE_GLOBAL_INL const_value_t& value() const { return m_pos; }
	MATRICE_GLOBAL_INL std::size_t size() const { 
		return (m_end - m_begin) / m_stride; 
	}

	MATRICE_GLOBAL_INL value_t& front() { return m_begin; }
	MATRICE_GLOBAL_INL const value_t& front() const { return m_begin; }

	MATRICE_GLOBAL_INL _My_iterator begin() { 
		return _My_iterator(m_pos, m_stride); 
	}
	MATRICE_GLOBAL_INL _My_iterator end() { 
		return _My_iterator(m_end, m_stride); 
	}
	MATRICE_GLOBAL_INL const _My_iterator begin() const {
		return _My_iterator(m_pos, m_stride);
	}
	MATRICE_GLOBAL_INL const _My_iterator end() const {
		return _My_iterator(m_end, m_stride);
	}

	MATRICE_GLOBAL_INL operator bool() const { 
		return (m_pos + m_stride < m_end); 
	}

private:
	value_t m_begin, m_end, m_stride = value_t(1);
	mutable value_t m_pos = m_begin;
}; }

// \TEMPLATE CLASS range : [begin, end[, stride])
template<typename _Ty>
class range MATRICE_NONHERITABLE : public detail::_Range_base<_Ty> {
	using _Mybase = detail::_Range_base<_Ty>;
public:
	MATRICE_GLOBAL_INL range(const _Ty& _First, const _Ty& _Last)
		: _Mybase(_First, _Last) {}
	MATRICE_GLOBAL_INL range(const _Ty& _First, const _Ty& _Last, const _Ty& _Inc)
		: _Mybase(_First, _Last, _Inc) {}
};

DGE_MATRICE_END