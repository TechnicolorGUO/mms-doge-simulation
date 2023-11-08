#ifndef BitVector256_h
#define BitVector256_h

#include <stdint.h> // uint16_t
#include <cstring> // memset

class BitVector256 {
public:
	static const unsigned VECTOR_SIZE = 16;

	BitVector256() {
		clearAll();//将vector的所有元素设置为0
	}
// vector 是一个长为16的数组

// vector[x] 的第 y 位设置为 1，而不影响其他位
//e.g. 0000000000000000->0000000000001000
	inline void set(unsigned x, unsigned y) {
		if (x < VECTOR_SIZE && y < VECTOR_SIZE)
			vector[x] |= 1 << y;
	}



//~(1 << y)：对 1 << y 进行按位取反操作，即将二进制表示中的所有位取反，包括第 y 位。因此，得到一个只有第 y 位为 0，其他位都为 1 的二进制值
//将 vector[x] 的第 y 位设置为 0，而保持其他位的值不变
	inline void clear(unsigned x, unsigned y) {
		if (x < VECTOR_SIZE && y < VECTOR_SIZE)
			vector[x] &= ~(1 << y);
	}



//获取vector[x]的第y位的值.
	inline bool get(unsigned x, unsigned y) const {
		if (x < VECTOR_SIZE && y < VECTOR_SIZE)
			return (vector[x] & 1 << y) != 0;

		return 0;
	}



//clearAll() 函数的作用是将 vector 数组中的所有元素清零，也即0000000000000000
	inline void clearAll() {
		memset(vector, 0, sizeof(vector));
	}


//setAll() 函数的作用是将 vector 数组中的所有元素设置为全 1,也即1111111111111111
	inline void setAll() {
		memset(vector, ~0, sizeof(vector));
	}


//vector中每一个元素占16bits
protected:
	uint16_t vector[(VECTOR_SIZE * VECTOR_SIZE) / (8 * sizeof(uint16_t))];
};

#endif