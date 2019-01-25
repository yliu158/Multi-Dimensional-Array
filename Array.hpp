#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <stdexcept>
#include <assert.h>
#include <cstdarg>

namespace cs540 {
	typedef std::out_of_range OutOfRange;

	template <typename T, size_t ... Dims>
	class Array {};

	template <typename T, size_t D, size_t ... Dims>
	class Array <T, D, Dims...> {
		static_assert(D > 0, "Dimension cannot be 0.");
	private:
		void init(size_t d, ...) {
			dims[0] = D;
			va_list args;
			va_start(args,d);
			for (size_t i = 1; i <= d; i++) dims[i] = va_arg(args, size_t);
			va_end(args);
		}
	public:
		struct FirstDimensionMajorIterator {

			size_t d = sizeof...(Dims)+1;
			size_t indexs[sizeof...(Dims)+1];
			Array<T,D,Dims...> *data;

			FirstDimensionMajorIterator() {init_it();}
			FirstDimensionMajorIterator(Array<T,D,Dims...> *dt) {data = dt;init_it();}
			FirstDimensionMajorIterator(size_t ids[sizeof...(Dims)+1], Array<T,D,Dims...> *dt) {
				for (size_t i = 0; i < d; i++) indexs[i] = ids[i];
				data = dt;
			}
			FirstDimensionMajorIterator(const FirstDimensionMajorIterator & it) {clone(it);}
			FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator & it) {clone(it);return *this;}
			bool operator==(const FirstDimensionMajorIterator & it) {return equal(it);}
			bool operator!=(const FirstDimensionMajorIterator & it) {return !equal(it);};
			FirstDimensionMajorIterator &operator++() {inc(); return *this;}
			FirstDimensionMajorIterator operator++(int) {FirstDimensionMajorIterator rtv(*this); inc(); return rtv;}
			T &operator*() const {return data->get(d, indexs);}
		private:
			void clone(const FirstDimensionMajorIterator & it) {
				for (size_t i = 0; i < d; i++) indexs[i] = it.indexs[i];
				data = it.data;
			}
			bool equal(const FirstDimensionMajorIterator & it) {
				if (data != it.data) return false;
				for (size_t i = 0; i < d; i++) {if (indexs[i] != it.indexs[i]) return false;}
				return true;
			}
			void inc() {
				int cur = d-1;
				while (cur >= 0) {
					if (indexs[cur]+1 >= data->dims[cur]) {indexs[cur--] = 0;}
					else {++indexs[cur]; break;}
				}
				if (cur == -1) {indexs[0] = data->size;}
			}
			void init_it() {for (size_t i = 0; i < d; i++) {indexs[i] = 0;}}
		};
		struct LastDimensionMajorIterator {

			size_t d = sizeof...(Dims)+1;
			size_t indexs[sizeof...(Dims)+1];
			Array<T,D,Dims...>* data;

			LastDimensionMajorIterator() {init_it();}
			LastDimensionMajorIterator(Array<T,D,Dims...>* dt) {init_it();data = dt;}
			LastDimensionMajorIterator(size_t ids[sizeof...(Dims)], Array<T,D,Dims...>* dt) {
				for (size_t i = 0; i < d; i++) {indexs[i] = ids[i];}
				data = dt;
			}
			LastDimensionMajorIterator(const LastDimensionMajorIterator& it) {clone(it);}
			LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator& it) {clone(it); return *this;}
			bool operator== (const LastDimensionMajorIterator& it) {return equal(it);}
			bool operator!= (const LastDimensionMajorIterator& it) {return !equal(it);}
			LastDimensionMajorIterator &operator++() {inc();return *this;}
			LastDimensionMajorIterator operator++(int){LastDimensionMajorIterator rtv(*this); inc(); return rtv;}
			T &operator*() const {return data->get(d, indexs);}
		private:
			void init_it() {for (size_t i = 0; i < d; i++) {indexs[i]=0;}}
			void clone(const LastDimensionMajorIterator& it) {
				for (size_t i = 0; i < d; ++i) {indexs[i] = it.indexs[i];}
				data = it.data;
			}
			bool equal(const LastDimensionMajorIterator& it) {
				if (data != it.data) return false;
				for (size_t i = 0; i < d; i++) {if (indexs[i] != it.indexs[i]) return false;}
				return true;
			}
			void inc() {
				size_t cur = 0;
				while (cur < d) {
					if (indexs[cur]+1 >= data->dims[cur]) {indexs[cur++] = 0;}
					else {++indexs[cur]; break;}
				}
				if (cur == d) indexs[d-1] = data->dims[d-1];
			}
		};
		/****************Array<T,D,Dims> fileds*****************/
		size_t size = D;
		Array<T, Dims...> data[D];
		size_t dims[sizeof...(Dims)+1];
		/****************Array<T,D,Dims> fileds****************/

		static T ValueType;
		Array <T, D, Dims...> (){
			size_t d = sizeof...(Dims);
			init(d, Dims...);
		}
		Array <T, D, Dims...> (const Array<T,D,Dims...>& arr) {
			for (size_t i = 0; i < size; ++i) {data[i]=arr.data[i];}
			size = arr.size;
		}
		template <typename U>
		Array<T, D, Dims...> (const Array<U,D,Dims...>& arr) {
			size = arr.size;
			for (size_t i = 0; i < size; i++) {data[i]=arr.data[i];}
		}
		Array<T, Dims...>& operator[](size_t id) {
			if (id >= D) throw OutOfRange("OutOfRange");
			else return *(&data[id]);
		}
		const Array<T, Dims...>& operator[](size_t id) const {
			if (id >= D) throw OutOfRange("OutOfRange");
			else return *(&data[id]);
		}
		Array<T, D, Dims...>& operator=(const Array<T,D,Dims...>& arr) {
			assert(size == arr.size);
			for (size_t i = 0; i < size; ++i) {data[i] = arr.data[i];}
			return *this;
		}
		template <typename U>
		Array<T, D, Dims...>& operator=(const Array<U,D,Dims...>& arr) {
			assert(size == arr.size);
			for (size_t i = 0; i < size; ++i) {data[i] = arr.data[i];}
			return *this;
		}
		FirstDimensionMajorIterator fmbegin() {return FirstDimensionMajorIterator(this);}
		FirstDimensionMajorIterator fmend() {
			FirstDimensionMajorIterator e(this);e.indexs[0] = D;
			return e;
		}
		LastDimensionMajorIterator lmbegin() {return LastDimensionMajorIterator(this);}
		LastDimensionMajorIterator lmend() {
			LastDimensionMajorIterator e(this); e.indexs[sizeof...(Dims)] = dims[sizeof...(Dims)];
			return e;
		}
		T &get(const size_t d, const size_t * indexs) {
			assert(d == sizeof...(Dims)+1);
			if (indexs[0] >= D) throw OutOfRange("OutOfRange");
			return data[indexs[0]].get(d-1, indexs+1);
		}
	};

	template <typename T, size_t D>
	class Array<T, D> {
		static_assert(D > 0, "Dimension cannot be 0.");
	private:
		void init() {for (size_t i = 0; i < D; i++) {data[i] = T();}}
	public:
		struct FirstDimensionMajorIterator {
			size_t index = 0; Array<T,D>* data; size_t len;
			FirstDimensionMajorIterator() {}
			FirstDimensionMajorIterator(Array<T,D>* d, size_t l) {data = d; len = l;}
			FirstDimensionMajorIterator(const FirstDimensionMajorIterator & it) {clone(it);}
			FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator & it) {clone(it); return *this;}
			bool operator==(const FirstDimensionMajorIterator &it) {return equal(it);}
			bool operator!=(const FirstDimensionMajorIterator &it) {return !equal(it);}
			FirstDimensionMajorIterator &operator++() {++index; return *this;}
			FirstDimensionMajorIterator operator++(int) {FirstDimensionMajorIterator rtv(*this); ++index; return rtv;}
			T &operator*() const {
				if (index >= len) throw OutOfRange("OutOfRange");
				return *(&data->data[index]);
			}
		private:
			void clone(const FirstDimensionMajorIterator & it) {index = it.index; data = it.data; len = it.len;}
			bool equal(const FirstDimensionMajorIterator & it) {return index == it.index && data == it.data;}
		};
		struct LastDimensionMajorIterator {
			size_t index = 0; Array<T,D>* data; size_t len;
			LastDimensionMajorIterator() {}
			LastDimensionMajorIterator(Array<T,D>* d, size_t l) {data = d; len = l;}
			LastDimensionMajorIterator(const LastDimensionMajorIterator& it) {clone(it);}
			LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator& it) {clone(it); return *this;}
			bool operator==(const LastDimensionMajorIterator &it) {return equal(it);}
			bool operator!=(const LastDimensionMajorIterator &it) {return !equal(it);}
			LastDimensionMajorIterator &operator++() {++index; return *this;}
			LastDimensionMajorIterator operator++(int){LastDimensionMajorIterator rtv(*this);++index; return rtv;}
			T &operator*() const {
				if (index >= len) throw OutOfRange("OutOfRange");
				return *(&data->data[index]);
			}
		private:
			void clone(const LastDimensionMajorIterator& it) {index = it.index; data = it.data; len = it.len;}
			bool equal(const LastDimensionMajorIterator& it) {return index == it.index && len == it.len;}
		};

		static T ValueType;
		/* */
		T data[D];
		size_t size = D;
		/* */

		Array<T, D> () {init();}
		Array<T, D> (const Array <T,D>& arr) {
			for (size_t i = 0; i < arr.size; ++i) data[i]=arr.data[i];
			size = arr.size;
		}
		template <typename U>
		Array<T, D> (const Array <U,D>& arr) {
			size = arr.size;
			for (size_t i = 0; i < size; i++) {data[i]=arr.data[i];}
		}
		T& operator[](size_t id) {
			if (id >= D) throw OutOfRange("OutOfRange");
			else return *(&data[id]);
		}
		const T& operator[](size_t id) const {
			if (id >= D) throw OutOfRange("OutOfRange");
			else return *(&data[id]);
		}
		Array<T, D>& operator=(const Array<T, D>& arr) {
			assert(size == arr.size);
			for (size_t i = 0; i < size; ++i) {data[i] = arr.data[i];}
			return *this;
		}
		template <typename U>
		Array<T, D>& operator=(const Array<U, D>& arr) {
			assert(size == arr.size);
			for (size_t i = 0; i < size; ++i) {data[i] = arr.data[i];}
			return *this;
		}
		FirstDimensionMajorIterator fmbegin(){return FirstDimensionMajorIterator(this, size);}
		FirstDimensionMajorIterator fmend() {
			FirstDimensionMajorIterator it = FirstDimensionMajorIterator(this, size);
			it.index = size;
			return it;
		}
		LastDimensionMajorIterator lmbegin() {return LastDimensionMajorIterator(this, size);}
		LastDimensionMajorIterator lmend() {
			LastDimensionMajorIterator it = LastDimensionMajorIterator(this, size);
			it.index = size;
			return it;
		}
		T &get(const size_t d, const size_t* indexs) {
			assert(d == 1);
			if (indexs[0] >= size) throw OutOfRange("OutOfRange");
			return *&data[indexs[0]];
		}
	};
}

#endif
