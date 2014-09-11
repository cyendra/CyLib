#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

class S_DES {
private:
	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef std::pair<unsigned int, unsigned int> PII;
	typedef std::pair<PII, PII> PPI;
	typedef std::vector<unsigned int> VI;
	typedef std::vector<VI> VVI;
	typedef std::vector<unsigned char> VC;
	typedef std::string String;

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

	// �û�������ΪX
	uint Permutation(uint Bit, int X, const VI &P) {
		uint res = 0;
		int n = P.size();
		for (int i = 0; i < n; i++) {
			res <<= 1;
			if (Bit & (1 << (X - P[i]))) res |= 1;
		}
		return res;
	}

	// ���û�
	uint InversePermutation(uint Bit, int X, const VI &P) {
		uint res = 0;
		int n = P.size();
		for (int i = 0; i < n; i++) {
			if (Bit&(1 << (n - i - 1))) {
				res |= 1 << (X - P[i]);
			}
		}
		return res;
	}

	// ��XΪ��λ�ָ��������Ϊ����
	PII Split(uint Bit, int X) {
		uint L = 0, R = 0;
		uint mask = (1 << X) - 1;
		R = Bit & mask;
		L = Bit >> X;
		return MakePII(L, R);
	}
	
	// ����λ��������ת��ΪS-BOX������
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

	// ����λ��������ת��ΪS-BOX������
	PPI GetExBox(uint Bit) {
		PII pii = Split(Bit,4);
		PII xy1 = GetBoxXY(pii.first);
		PII xy2 = GetBoxXY(pii.second);
		return MakePPI(xy1.first, xy1.second, xy2.first, xy2.second);
	}

	// �ϲ���������ΪX�Ķ�������
	uint Merge(uint lBit, uint rBit, int X) {
		return (lBit << X) | rBit;
	}

	// ������ΪL�Ķ���������ѭ������X��
	uint LS(uint Bit, int L, int X) {
		X %= L;
		uint mask = (1 << L) - 1;
		uint ans = ((Bit << X) & mask) | (Bit >> (L - X));
		return ans;
	}

	// S-DES ���������ɹ��̣�MasterKey��10λ������Կ��
	PII GetSubPsw(uint MasterKey) {
		uint K = Permutation(MasterKey, 10, P_10);// ����ԿK����P10�û�
		PII pii = Split(K, 5);                    // �ֳ���5λ��5λ
		uint L = pii.first;						  // 
		uint R = pii.second;					  // 
		L = LS(L, 5, 1);						  // �ֱ����LS-1����
		R = LS(R, 5, 1);						  // ����һ������Ϊ��һ�ֵĳ�ʼֵ
		uint K_1 = Merge(L, R, 5);                // ��һ�������P8�û�
		K_1 = Permutation(K_1, 10, P_8);          // �õ�K1
		L = LS(L, 5, 2);						  // �ٷֱ���ѭ��2λ
		R = LS(R, 5, 2);						  // 
		uint K_2 = Merge(L, R, 5);				  // 
		K_2 = Permutation(K_2, 10, P_8);		  // ����P8�û����õ�K2
		return MakePII(K_1, K_2);
	}

	// S-DES��f����
	uint Function(uint Ipt, uint K) {
		uint ex = Permutation(Ipt, 4, EP);// E/P��չ���û�����4λR��չΪ8λ
		ex ^= K;						  // ��չ���8λ�����ԿK
		PPI ppi = GetExBox(ex);           // ���4λ��ΪS1�����룬�ұ���λ��ΪS2������
		uint x1 = ppi.first.first;		  // ��S1��S2�У���һλ�����λ����γ�2λ����S�е��к�
		uint y1 = ppi.first.second;       // �ڶ�λ�����λ����γ�2λ����S�е��к�
		uint x2 = ppi.second.first;       //
		uint y2 = ppi.second.second;      //
		uint s1 = S_1[x1][y1];			  // �õ�S�е����
		uint s2 = S_2[x2][y2];            //
		uint res = Merge(s1, s2, 2);      //
		res = Permutation(res, 4, P_4);   // ����P4�û����õ�f���������
		return res;
	}

	// S-DES ����
	uint S_DES_Main(uint Plaintext, uint K_1, uint K_2) {
		Plaintext = Permutation(Plaintext, 8, IP);// ��ʼ�û�IP����8λ���İ����û�˳�����λ�ñ仯��
		PII pii = Split(Plaintext, 4);			  // �û����
		uint L0 = pii.first;					  // ��4λL0
		uint R0 = pii.second;					  // ��4λR0
		uint L1 = R0;							  // ��һ�����㣬R0��Ϊ��һ�ֵ�L1
		uint R1 = L0 ^ (Function(R0, K_1));		  // R0��Ϊf������������8λ����ԿK1���뺯�����㣬����ṹ��L0��򣬽����Ϊ��һ�ֵ�R1
		uint R2 = R1;							  // �ڶ������㣬R1��Ϊ��һ�ֵ�R2
		uint L2 = L1 ^ (Function(R1, K_2));       // R1��Ϊf������������8λ����ԿK2���뺯�����㣬��������L1��򣬽����Ϊ��һ�ֵ�L2
		uint res = Merge(L2, R2, 4);			  // 
		res = Permutation(res, 8, IP_1);		  // ���û�IP-1
		return res;
	}

	// S-DES ����
	uint Decryption(uint Cip, uint K_1, uint K_2) {
		Cip = InversePermutation(Cip, 8, IP_1);
		PII pii = Split(Cip, 4);
		uint L2 = pii.first;
		uint R2 = pii.second;
		uint R1 = R2;
		uint L1 = L2 ^ (Function(R1, K_2));
		uint R0 = L1;
		uint L0 = R1 ^ (Function(R0, K_1));
		uint res = Merge(L0, R0, 4);
		res = InversePermutation(res, 8, IP);
		return res;
	}
public:
	// �������Զ�������ʽ���
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

	// ���������ַ���ת��Ϊ����
	uint StringToBinary(const std::string &Str) {
		uint res = 0;
		uint len = Str.length();
		for (uint i = 0; i < len; i++) {
			res <<= 1;
			if (Str[i] == '1') res |= 1;
		}
		return res;
	}

	// ����һ����λ������
	uint EncryptInt(uint Text, uint MasterKey) {
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		uint res = S_DES_Main(Text, K_1, K_2);
		return res;
	}
	
	// ���ַ�����һ���ַ���
	VI EncryptString(const String &Str, uint MasterKey) {
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

	// ����һ��vector�е�����
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

	// ���ܳ���Ϊn�������е�����
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

	// ���ܳ���Ϊn��Byte����
	VI EncryptBytes(const uchar Arr[], int n, uint MasterKey) {
		VI res;
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		for (int i = 0; i < n; i++) {
			uint e = S_DES_Main((uint)Arr[i], K_1, K_2);
			res.push_back(e);
		}
		return res;
	}

	// ����һ���ļ��е�����
	VI EncryptFile(char FileName[], uint MasterKey) {
		VI res;
		std::ifstream fin(FileName, std::ios::binary);
		if (!fin.is_open()) return res;
		int n = (int)fin.gcount(); 
		char pc;
		for (int i = 0; i < n; i++) {
			fin.read(&pc, sizeof(uchar));
			res.push_back((uint)pc);
		}
		return res;
	}

	// ����һ����λ������
	uint DecryptInt(uint Cip, uint MasterKey) {
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		uint res = Decryption(Cip, K_1, K_2);
		return res;
	}

	// ����vector�е�����
	VI DecryptVector(const VI& Cips, uint MasterKey) {
		auto p = GetSubPsw(MasterKey);
		uint K_1 = p.first;
		uint K_2 = p.second;
		VI res;
		for (auto it = Cips.begin(); it != Cips.end(); it++) {
			uint des = Decryption(*it, K_1, K_2);
			res.push_back(des);
		}
		return res;
	}

	// ��vector�е�����ת��Ϊstring����
	String VectorToString(const VI& vec) {
		String res;
		for (auto it = vec.begin(); it != vec.end(); it++) {
			res.push_back((char)*it);
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

