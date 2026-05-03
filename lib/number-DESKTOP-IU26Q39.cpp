#include "number.h"

static const int bytes = 254; // колво байт по условию
static const uint8_t mask_top = 0x01; //Старший байт

static void zero(uint8_t* dst) { // обнуление
    for (int i = 0; i < bytes; ++i) dst[i] = 0;
}

static void normalize(uint8_t* d) {
    d[bytes - 1] &= mask_top;
}

static void copy_bytes(uint8_t* dst, const uint8_t* src) { //ctrl c массива
    for (int i = 0; i < bytes; ++i) dst[i] = src[i];
}

static void add_bytes(const uint8_t* a, const uint8_t* b, uint8_t* out) { //сложение
    unsigned carry = 0;
    for (int i = 0; i < bytes; ++i) {
        unsigned sum = a[i] + b[i] + carry;
        out[i] = (uint8_t)(sum & 0xFF);
        carry = sum >> 8;
    }
    normalize(out);
}

static void negate(const uint8_t* in, uint8_t* out) { //случай отрицательного
    for (int i = 0; i < bytes; ++i) out[i] = (uint8_t)(~in[i]);
    unsigned carry = 1;
    for (int i = 0; i < bytes && carry; ++i) {
        unsigned sum = out[i] + carry;
        out[i] = (uint8_t)(sum & 0xFF);
        carry = sum >> 8;
    }
    normalize(out);
}

static void mul_bytes(const uint8_t* a, const uint8_t* b, uint8_t* out) { // умножение без учета знака
    static uint16_t tmp[bytes * 2];
    for (int i = 0; i < bytes * 2; ++i) tmp[i] = 0;

    for (int i = 0; i < bytes; ++i) {
        unsigned carry = 0;
        for (int j = 0; j < bytes; ++j) {
            unsigned long long sum = tmp[i + j] + (unsigned)a[i] * (unsigned)b[j] + carry;
            tmp[i + j] = (uint16_t)(sum & 0xFF);
            carry = (unsigned)(sum >> 8);
        }
        tmp[i + bytes] += carry;
    }

    for (int i = 0; i < bytes; ++i) out[i] = (uint8_t)tmp[i];
    normalize(out);
}

static int cmp_unsigned(const uint8_t* a, const uint8_t* b) {
    for (int i = bytes - 1; i >= 0; --i) {
        uint8_t ai = a[i];
        uint8_t bi = b[i];
        if (i == bytes - 1) { ai &= mask_top; bi &= mask_top; }
        if (ai < bi) return -1;
        if (ai > bi) return 1;
    }
    return 0;
}

static void sub_bytes(const uint8_t* a, const uint8_t* b, uint8_t* out) {
    int borrow = 0;
    for (int i = 0; i < bytes; ++i) {
        int val = (int)a[i] - (int)b[i] - borrow;
        if (val < 0) {
            val += 256;
            borrow = 1;
        } else borrow = 0;
        out[i] = (uint8_t)val;
    }
    normalize(out);
}

static bool is_zero(const uint8_t* a) {
    for (int i = 0; i < bytes; ++i)
        if (a[i]) return false;
    return true;
}


int2025_t from_int(int32_t i) { //конва int32_t
    int2025_t res;
    uint8_t fill = (i < 0) ? 0xFF : 0x00;
    for (int k = 0; k < bytes; ++k) res.data[k] = fill;
    res.data[0] = (uint8_t)(i & 0xFF);
    res.data[1] = (uint8_t)((i >> 8) & 0xFF);
    res.data[2] = (uint8_t)((i >> 16) & 0xFF);
    res.data[3] = (uint8_t)((i >> 24) & 0xFF);
    normalize(res.data);
    return res;
}

int2025_t from_string(const char* buff) { //строковая конва
    int2025_t res;
    for (int i = 0; i < bytes; ++i) res.data[i] = 0;

    int sign = 0;
    const char* p = buff;
    if (*p == '-') { sign = 1; ++p; }
    else if (*p == '+') ++p;

    for (; *p; ++p) {
        if (*p < '0' || *p > '9') break; //умножаем текущее число на 10 и + цифра
        uint8_t temp[bytes];
        copy_bytes(temp, res.data);
        for (int i = 0; i < 9; ++i) add_bytes(res.data, temp, res.data); // + цифра
        unsigned carry = *p - '0';
        for (int i = 0; i < bytes && carry; ++i) {
            unsigned sum = res.data[i] + carry;
            res.data[i] = (uint8_t)(sum & 0xFF);
            carry = sum >> 8;
        }
    }

    if (sign) {     //на случай отрицательного делаем дополнение
        uint8_t neg[bytes];
        negate(res.data, neg);
        copy_bytes(res.data, neg);
    }
    normalize(res.data);
    return res;
}

int2025_t operator+(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t res;
    add_bytes(lhs.data, rhs.data, res.data);
    return res;
}

int2025_t operator-(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t res;
    uint8_t neg_rhs[bytes];
    negate(rhs.data, neg_rhs);
    add_bytes(lhs.data, neg_rhs, res.data);
    return res;
}

int2025_t operator*(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t res;
    mul_bytes(lhs.data, rhs.data, res.data);
    return res;
}

int2025_t operator/(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t res;
    zero(res.data);

    // дел на 0 =0
    if (is_zero(rhs.data)) return res;

    bool neg_lhs = (lhs.data[bytes - 1] & mask_top) != 0;
    bool neg_rhs = (rhs.data[bytes - 1] & mask_top) != 0;
    bool neg_res = neg_lhs ^ neg_rhs;

    uint8_t a[bytes], b_orig[bytes]; //привожу к положит
    if (neg_lhs) negate(lhs.data, a);
    else copy_bytes(a, lhs.data);
    if (neg_rhs) negate(rhs.data, b_orig);
    else copy_bytes(b_orig, rhs.data);
    if (cmp_unsigned(a, b_orig) < 0) {
        return res;
    }

    //формирую двоичное деление для ускорения
    const int total_bits = bytes * 8; // 2032

    for (int shift = total_bits - 1; shift >= 0; --shift) {

        uint8_t shifted[bytes]; // построим shifted = b_orig << shift (временный массив)
        for (int i = 0; i < bytes; ++i) shifted[i] = 0; // обнуляю

        int byte_shift = shift / 8;
        int bit_shift = shift % 8;

        for (int i = bytes - 1; i >= 0; --i) { //копия
            int src = i - byte_shift;
            uint8_t low = 0, high = 0;

            if (src >= 0)
                low = (uint8_t)((unsigned)b_orig[src] << bit_shift);
            if (bit_shift != 0 && src - 1 >= 0)
                high = (uint8_t)((unsigned)b_orig[src - 1] >> (8 - bit_shift));

            shifted[i] = (uint8_t)(low | high);
        }
        normalize(shifted);

        if (is_zero(shifted)) continue;

        if (cmp_unsigned(a, shifted) >= 0) { //если a >= shifted  вычитаем и добавляем 1  shift к результату
            sub_bytes(a, shifted, a);

            int bit_index = shift; // добавляем 1 << shift
            int byte_index = bit_index / 8;
            int bit_in_byte = bit_index % 8;

            uint16_t carry = (1u << bit_in_byte);

            while (byte_index < bytes && carry) {
                unsigned sum = (unsigned)res.data[byte_index] + (carry & 0xFFu);
                res.data[byte_index] = (uint8_t)(sum & 0xFFu);
                carry = (sum >> 8);
                ++byte_index;
            }
        }
    }

    if (neg_res) { //корректировка 
        uint8_t negv[bytes];
        negate(res.data, negv);
        copy_bytes(res.data, negv);
    }

    normalize(res.data);
    return res;
}


bool operator==(const int2025_t& lhs, const int2025_t& rhs) {
    for (int i = 0; i < bytes; ++i)
        if (lhs.data[i] != rhs.data[i]) return false;
    return true;
}

bool operator!=(const int2025_t& lhs, const int2025_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2025_t& value) { // вывод числа в поток в dec
    bool is_neg = (value.data[bytes - 1] & mask_top) != 0;
    uint8_t temp[bytes];
    if (is_neg) negate(value.data, temp);
    else copy_bytes(temp, value.data);

    char buf[700];
    int pos = 0;

    while (!is_zero(temp)) { //dec строка
        unsigned rem = 0;
        for (int i = bytes - 1; i >= 0; --i) {
            unsigned cur = (rem << 8) + temp[i];
            temp[i] = (uint8_t)(cur / 10);
            rem = cur % 10;
        }
        buf[pos++] = (char)('0' + rem);
    }

    if (pos == 0) stream << '0';
    else {
        if (is_neg) stream << '-';
        for (int i = pos - 1; i >= 0; --i)
            stream << buf[i];
    }
    return stream;
}
