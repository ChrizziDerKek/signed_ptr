#pragma once

#ifndef _WIN64
#error only x64 is supported
#endif

//signed_ptr is a class that can be
//used instead of a regular pointer
//and ensures that a pointer was not
//modified in unexpected ways by
//storing a hash of the pointer in
//the unused 17 bits of a pointer
template <class T>
class signed_ptr
{
private:
	union {
		struct {
			size_t m_value : 47; //actual ptr value
			size_t m_signed : 1; //1 if we signed the ptr
			size_t m_hash : 16; //hash value of the ptr
		};
		size_t m_raw; //raw value which contains the 3 values above
	};

public:
	//default constructor
	inline signed_ptr() : m_raw(0) { sign(); }

	//ptr constructor
	inline signed_ptr(T* ptr) : m_raw(reinterpret_cast<size_t>(ptr)) { sign(); }

	//signed ptr constructor
	inline signed_ptr(const signed_ptr& ptr) : m_raw(ptr.m_raw) {}

	//true if the pointer is anything but null
	inline operator bool() const {
		return get() != nullptr;
	}

	//returns the ref only if the hash is still valid 
	inline T& operator*() const {
		return reinterpret_cast<T&>(*get());
	}

	//returns the ptr only if the hash is still valid
	inline T* operator->() const {
		return get();
	}

	//returns the ptr only if the hash is still valid
	inline T* get() const {
		if (m_signed == 0 || !is_valid())
			return nullptr;
		return reinterpret_cast<T*>(m_value);
	}

	//deletes a heap allocated ptr
	inline void destroy() {
		free(get());
		m_raw = 0;
		sign();
	}

private:
	//checks if the hash is valid
	inline bool is_valid() const {
		return hash(reinterpret_cast<void*>(m_value), sizeof(m_raw)) == m_hash;
	}

	//signs the ptr if it is not signed yet
	inline void sign() {
		if (m_signed != 0)
			return;
		m_hash = hash(reinterpret_cast<void*>(m_value), sizeof(m_raw));
		m_signed = 1;
	}

	//modified fnv1a hash to work with 16 bits
	inline static unsigned short hash(void* data, size_t size) {
		if (!data)
			return 0;
		unsigned hash = 0x811C9DC5;
		for (size_t i = 0; i < size; i++) {
			hash ^= reinterpret_cast<unsigned char*>(data)[i];
			hash *= 0x1000193;
		}
		return (hash >> 16) ^ (hash & 0xFFFF);
	}
};

//creates a signed_ptr
template <class T, class... TArgs>
inline signed_ptr<T> make_signed(const TArgs&... args) {
	return signed_ptr<T>(new T(args...));
}

//compare operators

template <class T1, class T2>
inline bool operator ==(const signed_ptr<T1>& t1, const signed_ptr<T2>& t2) {
	return t1.get() == t2.get();
}

template <class T1, class T2>
inline bool operator !=(const signed_ptr<T1>& t1, const signed_ptr<T2>& t2) {
	return t1.get() != t2.get();
}

template <class T1, class T2>
inline bool operator <=(const signed_ptr<T1>& t1, const signed_ptr<T2>& t2) {
	return t1.get() <= t2.get();
}

template <class T1, class T2>
inline bool operator >=(const signed_ptr<T1>& t1, const signed_ptr<T2>& t2) {
	return t1.get() >= t2.get();
}

template <class T1, class T2>
inline bool operator <(const signed_ptr<T1>& t1, const signed_ptr<T2>& t2) {
	return t1.get() < t2.get();
}

template <class T1, class T2>
inline bool operator >(const signed_ptr<T1>& t1, const signed_ptr<T2>& t2) {
	return t1.get() > t2.get();
}

template <class T1, class T2>
inline bool operator ==(const T1& t1, const signed_ptr<T2>& t2) {
	return t1 == t2.get();
}

template <class T1, class T2>
inline bool operator !=(const T1& t1, const signed_ptr<T2>& t2) {
	return t1 != t2.get();
}

template <class T1, class T2>
inline bool operator <=(const T1& t1, const signed_ptr<T2>& t2) {
	return t1 <= t2.get();
}

template <class T1, class T2>
inline bool operator >=(const T1& t1, const signed_ptr<T2>& t2) {
	return t1 >= t2.get();
}

template <class T1, class T2>
inline bool operator <(const T1& t1, const signed_ptr<T2>& t2) {
	return t1 < t2.get();
}

template <class T1, class T2>
inline bool operator >(const T1& t1, const signed_ptr<T2>& t2) {
	return t1 > t2.get();
}

template <class T1, class T2>
inline bool operator ==(const signed_ptr<T1>& t1, T2& t2) {
	return t1.get() == t2;
}

template <class T1, class T2>
inline bool operator !=(const signed_ptr<T1>& t1, const T2& t2) {
	return t1.get() != t2;
}

template <class T1, class T2>
inline bool operator <=(const signed_ptr<T1>& t1, const T2& t2) {
	return t1.get() <= t2;
}

template <class T1, class T2>
inline bool operator >=(const signed_ptr<T1>& t1, const T2& t2) {
	return t1.get() >= t2;
}

template <class T1, class T2>
inline bool operator <(const signed_ptr<T1>& t1, const T2& t2) {
	return t1.get() < t2;
}

template <class T1, class T2>
inline bool operator >(const signed_ptr<T1>& t1, const T2& t2) {
	return t1.get() > t2;
}
