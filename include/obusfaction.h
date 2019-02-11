#ifndef OBUSFACTION_H
#define OBUSFACTION_H

#include <string>

namespace adin
{

template<std::size_t index>
struct encryptor {
    static constexpr int encrypt(char *dest, const char *str) {
        dest[index] = str[index] ^ (index+1);

        encryptor<index - 1>::encrypt(dest, str);
        return index;
    }
};

template<>
struct encryptor<0> {
    static constexpr int encrypt(char *dest, const char *str) {
        dest[0] = str[0] ^ 1;
        return 0;
    }
};


class cryptor {
public:
    template<std::size_t S>
    class string_encryptor {
    public:
        constexpr string_encryptor(const char str[S])
            : _buffer{'\0'}, _size(S-1) {
            adin::encryptor<S - 2>::encrypt(_buffer, str);
        }

        std::string decrypt() const {
            std::string ret(_size, '\0');

            for (size_t i = 0; i < _size; i++) {
                ret[i] = _buffer[i] ^ (i+1);
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
            ret[i] = str[i] ^ (i+1);
        }
        return ret;
    }

};

} //adin

#endif // OBUSFACTION_H
