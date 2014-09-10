#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

class S_DES {
private:
	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef std::pair<unsigned int, unsigned int> PII;
	typedef std::pair<PII, PII> PPI;
	typedef std::vector<unsigned int> VI;
	typedef std::vector<VI> VVI;
	typedef std::vector<unsigned char> VC;

	const static VI P_10;
	const static VI P_8;
	const static VI P_4;
	const static VI IP;
	const static VI IP_1;
	const static VI EP;
	const static VVI S_1;
	const static VVI S_2;

	// MakePII
	inline PII MakePII(uint L, uint R) {
		return std::make_pair(L, R);
	}

	// MakePPI
	inline PPI MakePPI(uint A, uint B, uint C, uint D) {
		return std::make_pair(std::make_pair(A, B), std::make_pair(C, D));
	}

	// 置换，长度为X
	int Permutation(uint Bit, int X, const VI &P) {
		uint res = 0;
		int n = P.size();
		for (int i = 0; i < n; i++) {
			res <<= 1;
			if (Bit & (1 << (X - P[i]))) res |= 1;
		}
		return res;
	}

	// 以X为单位分割二进制数为两份
	PII Split(uint Bit, int X) {
		uint L = 0, R = 0;
		uint mask = (1 << X) - 1;
		R = Bit & mask;
		L = Bit >> X;
		return MakePII(L, R);
	}
	
	// 将四位二进制数转化为S-BOX的坐标
	PII GetBoxXY(uint Bit) {
		uint x = 0, y = 0;
		if (Bit&(1 << 3)) x |= 1;
		if (Bit&(1 << 2)) y |= 1;
		x <<= 1;
		y <<= 1;
		if (Bit&(1 << 0)) x |= 1;
		if (Bit&(1 << 1)) y |= 1;
		return MakePII(x, y);
	}

	// 将八位二进制数转化为S-BOX的坐标
	PPI GetExBox(uint Bit) {
		PII pii = Split(Bit,4);
		PII xy1 = GetBoxXY(pii.first);
		PII xy2 = GetBoxXY(pii.second);
		return MakePPI(xy1.first, xy1.second, xy2.first, xy2.second);
	}

	// 合并两个长度为X的二进制数
	uint Merge(uint lBit, uint rBit, int X) {
		return (lBit << X) | rBit;
	}

	// 将长度为L的二进制数，循环左移X次
	uint LS(uint Bit, int L, int X) {
		X %= L;
		uint mask = (1 << L) - 1;
		uint ans = ((Bit << X) & mask) | (Bit >> (L - X));
		return ans;
	}

	// S-DES 子密码生成过程
	PII GetSubPsw(uint MasterKey) {
		uint K = Permutation(MasterKey, 10, P_10);
		PII pii = Split(K, 5);
		uint L = pii.first;
		uint R = pii.second;
		L = LS(L, 5, 1);
		R = LS(R, 5, 1);
		uint K_1 = Merge(L, R, 5);
		K_1 = Permutation(K_1, 10, P_8);
		L = LS(L, 5, 2);
		R = LS(R, 5, 2);
		uint K_2 = Merge(L, R, 5);
		K_2 = Permutation(K_2, 10, P_8);
		return MakePII(K_1, K_2);
	}

	// S-DES的f函数
	uint Function(uint Ipt, uint K) {
		uint ex = Permutation(Ipt, 4, EP);
		ex ^= K;
		PPI ppi = GetExBox(ex);
		uint x1 = ppi.first.first;
		uint y1 = ppi.first.second;
		uint x2 = ppi.second.first;
		uint y2 = ppi.second.second;
		uint s1 = S_1[x1][y1];
		uint s2 = S_2[x2][y2];
		uint res = Merge(s1, s2, 2);
		res = Permutation(res, 4, P_4);
		return res;
	}

	uint S_DES_Main(uint Plaintext, uint K_1, uint K_2) {
		Plaintext = Permutation(Plaintext, 8, IP);
		PII pii = Split(Plaintext, 4);
		uint L0 = pii.first;
		uint R0 = pii.second;
		uint L1 = R0;
		uint R1 = L0 ^ (Function(R0, K_1));
		uint L2 = L1 ^ (Function(R1, K_2));
		uint R2 = R1;
		uint res = Merge(L2, R2, 4);
		res = Permutation(res, 8, IP_1);
		return res;
	}

public:
	
	// 将数字以二进制形式输出
	void PrintBinary(uint b) {
		if (b == 0) {
			printf("0\n");
			return;
		}
		VI vec;
		vec.clear();
		while (b > 0) {
			if (b & 1) vec.push_back(1);
			else vec.push_back(0);
			b >>= 1;
		}
		for (auto it = vec.rbegin(); it != vec.rend(); it++) {
			printf("%d", *it);
		}
		printf("\n");
	}

	// 将二进制字符串转换为数字
	uint StringToBinary(const std::string &Str) {
		uint res = 0;
		uint len = Str.length();
		for (uint i = 0; i < len; i++) {
			res <<= 1;
			if (Str[i] == '1') res |= 1;
		}
		return res;
	}

	// 加密一个单位的数据
	uint EncryptInt(uint Text, uint MasterKey) {
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		uint res = S_DES_Main(Text, K_1, K_2);
		return res;
	}
	
	// 按字符加密一个字符串
	VI EncryptString(const std::string &Str, uint MasterKey) {
		VI res;
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		int len = Str.length();
		for (int i = 0; i < len; i++) {
			uint e = S_DES_Main((uint)Str[i], K_1, K_2);
			res.push_back(e);
		}
		return res;
	}

	// 加密一组vector中的数字
	VI EncryptVector(const VI &Arr, uint MasterKey) {
		VI res;
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		int len = Arr.size();
		for (int i = 0; i < len; i++) {
			uint e = S_DES_Main(Arr[i], K_1, K_2);
			res.push_back(e);
		}
		return res;
	}

	// 加密长度为n的数组中的数字
	VI EncryptArray(const uint Arr[], int n, uint MasterKey) {
		VI res;
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		for (int i = 0; i < n; i++) {
			uint e = S_DES_Main(Arr[i], K_1, K_2);
			res.push_back(e);
		}
		return res;
	}

	// 加密一个文件中的数据
	VI EncryptFile(char FileName[], uint MasterKey) {
		VI res;
		std::ifstream fin(FileName, std::ios::binary);
		if (!fin.is_open()) return res;
		int n = (int)fin.gcount(); 
		uint bit = 0;
		for (int i = 0; i < n; i++) {
			char pc;
			fin.read(&pc, sizeof(uchar));
			bit <<= 8;
			bit |= pc;
			if (i & 1) {
				bit = EncryptInt(bit, MasterKey);
				res.push_back(bit);
				bit = 0;
			}
		}
		if (n & 1) {
			bit <<= 8;
			bit = EncryptInt(bit, MasterKey);
			res.push_back(bit);
		}
		return res;
	}


};

const S_DES::VI S_DES::P_10{ 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 };
const S_DES::VI S_DES::P_8{ 6, 3, 7, 4, 8, 5, 10, 9 };
const S_DES::VI S_DES::P_4{ 2, 4, 3, 1 };
const S_DES::VI S_DES::IP{ 2, 6, 3, 1, 4, 8, 5, 7 };
const S_DES::VI S_DES::IP_1{ 4, 1, 3, 5, 7, 2, 8, 6 };
const S_DES::VI S_DES::EP{ 4, 1, 2, 3, 2, 3, 4, 1 };
const S_DES::VVI S_DES::S_1{ { 1, 0, 3, 2 },
							 { 3, 2, 1, 0 },
							 { 0, 2, 1, 3 },
							 { 3, 1, 3, 2 } };
const S_DES::VVI S_DES::S_2{ { 0, 1, 2, 3 },
							 { 2, 0, 1, 3 }, 
							 { 3, 0, 1, 0 },
							 { 2, 1, 0, 3 } };

