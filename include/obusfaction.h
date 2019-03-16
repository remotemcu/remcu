#ifndef OBUSFACTION_H
#define OBUSFACTION_H

#include <cstring>
#include <string>

#if 0

namespace remcu
{

template<std::size_t index>
struct struct_expr {
    static constexpr int func(char *dest, const char *str) {
        if (index == 0) { 
          dest[index] = str[index] ^ 1;
          return 0;
        } else {
          dest[index] = str[index] ^ ((index+1) & 0xFF);
          struct_expr<index - 1>::func(dest, str);
          return index;
        }
    }
};
/*
template<>
struct struct_expr<0> {
    static constexpr int func(char *dest, const char *str) {
        dest[0] = 1;
        return 0;
    }
};
*/
template<std::size_t index>
struct encryptor {
    static constexpr int encrypt(char *dest, const char *str) {
        if (index == 0) { 
          dest[index] = str[index] ^ 1;
          return 0;
        } else {
          dest[index] = str[index] ^ ((index+1) & 0xFF);
          struct_expr<index - 1>::func(dest, str);
          return index;
        }
    }
};
/*
template<>
struct encryptor<0> {
    static constexpr int encrypt(char *dest, const char *str) {
        dest[0] = str[0] ^ 1;
        return 0;
    }
};
*/

class test_class {
public:
    template<std::size_t S>
    class string_class {
    private:
        char _buffer[S];
        const size_t _size;
    public:
        constexpr string_class(const char str[S])
            : _buffer{'\0'}, _size(S-1) {
            struct_expr<S-1>::func(_buffer, str);
        } // error
    };
};


class cryptor {
public:
    template<std::size_t S>
    class string_encryptor {
    public:
        constexpr string_encryptor(const char str[S])
            : _buffer{'\0'}, _size(S-1) {
            encryptor<S - 2>::encrypt(_buffer, str);
        }

        std::string decrypt() const {
            std::string ret(_size, '\0');

            for (size_t i = 0; i < _size; i++) {
                ret[i] = _buffer[i] ^ ((i+1) & 0xFF);
            }

            return ret;
        }

        std::string get() const {
            return std::string(_buffer, _size);
        }

    private:
        char _buffer[S];
        const size_t _size;
    };

    template<std::size_t S>
    static constexpr string_encryptor<S> create(const char(&str)[S]) {
        return string_encryptor<S>{ str};
    }

    static std::string proccess(const char* str){
        size_t size = strlen(str);
        std::string ret(size, '\0');
        size--;
        for (size_t i = 0; i < size; i++) {
            ret[i] = str[i] ^ ((i+1) & 0xFF);
        }
        return ret;
    }

};

#define __SHIFT 2

template<std::size_t index>
struct encryptor_debug {
    static constexpr int encrypt(char *dest, const char *str) {
        if(str[index] == '.'){
            dest[index] = '\0';
        } else {
            dest[index] = str[index] + __SHIFT;
        }
        encryptor_debug<index - 1>::encrypt(dest, str);
        return index;
    }
};

template<>
struct encryptor_debug<0> {
    static constexpr int encrypt(char *dest, const char *str) {
        dest[0] = str[0] + __SHIFT;
        return 0;
    }
};


class cryptor_debug {
public:
    template<std::size_t S>
    class string_encryptor_debug {
    public:
        constexpr string_encryptor_debug(const char str[S])
            : _buffer{'\0'}, _size(S-1) {
            encryptor_debug<S - 2>::encrypt(_buffer, str);
        }

        std::string get() const {
            return std::string(_buffer, _size);
        }

    private:
        char _buffer[S];
        const size_t _size;
    };

    template<std::size_t S>
    static constexpr string_encryptor_debug<S> create1(const char(&str)[S]) {
        return string_encryptor_debug<S>{ str};
    }

};

} //namespace

#define _S_(str) cryptor::create(str).decrypt()

#define _D_(str) cryptor_debug::create1(str).get().c_str()
#else

#define _S_(str) std::string(str)

#define _D_(str) (str)

#endif

#endif // OBUSFACTION_H
