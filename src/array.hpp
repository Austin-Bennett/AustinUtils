#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "Error.hpp"
#include "misc.hpp"

namespace AustinUtils {
    template<typename T>
    class array {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using size_type = usize;
        using difference_type = usize;
    private:
        T* arr;
        size_type msize = 0;
    public:


        array() {
            arr = nullptr;
            msize = 0;
        }

        explicit array(const size_type n) : msize(n) {
            arr = new T[n];
        }

        template<class inputIterator>
        array(inputIterator begin, inputIterator end) {
            inputIterator s = begin;
            while (s != end) {
                ++s;
                msize++;
            }
            arr = new T[msize];
            usize i = 0;
            while (begin != end) {
                arr[i] = *begin;
                ++begin;
                i++;
            }
        }

        array(std::initializer_list<T> l) {
            msize = l.size();
            auto b = l.begin();
            auto e = l.end();
            arr = new T[msize];
            usize i = 0;
            while (b != e) {
                arr[i] = *b;
                ++b;
                i++;
            }
        }


        array(const array& _arr) {
            msize = _arr.msize;
            arr = new T[_arr.msize];
            std::copy(_arr.arr, _arr.arr + _arr.msize, arr);
        }

        array(array&& _arr)  noexcept {
            msize = _arr.msize;
            arr = _arr.arr;
            _arr.msize = 0;
            _arr.arr = nullptr;
        }

        array& operator =(const array& _arr) {
            if (arr == _arr.arr) return *this;

            delete[] arr;

            msize = _arr.msize;
            arr = new T[_arr.msize];
            std::copy(_arr.arr, _arr.arr + _arr.msize, arr);
            return *this;
        }

        array& operator =(array&& _arr) noexcept {
            if (_arr.arr == arr) return *this;
            msize = _arr.msize;
            arr = _arr.arr;
            _arr.msize = 0;
            _arr.arr = nullptr;
            return *this;
        }

        NODISCARD T& at(size_type i) const {
            if (i >= msize) throw Exception("Cannot access element at index ", i);
            return arr[i];
        }

        NODISCARD T& operator[](size_type i) const {
            if (i >= msize) throw Exception("Cannot access element at index ", i);
            return arr[i];
        }

        T& front() {
            if (!arr) throw Exception("Cannot access the front of an array of size 0");
            return arr[0];
        }

        T& back() {
            if (!arr) throw Exception("Cannot access the front of an array of size 0");
            return arr[msize-1];
        }

        T* data() {
            return arr;
        }

        [[nodiscard]] bool empty() const {
            return arr != nullptr;
        }

        [[nodiscard]] size_type size() const {
            return msize;
        }

        [[nodiscard]] static size_type max_size() {
            return T_MAX(size_type);
        }

        template<typename... constructor>
        T& emplace(usize i, constructor... constructor_args) {
            //creates the variable in-place
            if (i >= msize)  throw Exception("Cannot access element at index ", i);

            //destroy object
            arr[i].~T();

            //create new one
            T* ret = new(arr+i) T(std::forward<constructor>(constructor_args)...);
            return *ret;
        }

        basic_iterator<T> begin() {
            return basic_iterator<T>(arr);
        }

        basic_iterator<T> end() {
            return basic_iterator<T>(arr+msize);
        }

        basic_reverse_iterator<T> rbegin() {
            return basic_reverse_iterator<T>(arr+msize-1);
        }

        basic_reverse_iterator<T> rend() {
            return basic_reverse_iterator<T>(arr-1);
        }


        friend std::ostream& operator <<(std::ostream& os, const array& A) {
            os << "[";
            for (size_type i = 0; i < A.size(); i++) {
                os << A[i];
                if (i != A.size()-1) os << ", ";
            }
            os << "]";
            return os;
        }


        friend std::wostream& operator <<(std::wostream& os, const array& A) {
            os << "[";
            for (size_type i = 0; i < A.size(); i++) {
                os << A[i];
                if (i != A.size()-1) os << ", ";
            }
            os << "]";
            return os;
        }

        ~array() {
            delete[] arr;
        }

    };
}

#endif