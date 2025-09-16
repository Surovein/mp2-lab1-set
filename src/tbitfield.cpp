// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(unsigned int len)
{
	BitLen = len;
	MemLen = (len + BITS_IN_ONE_MEM - 1) / BITS_IN_ONE_MEM;
	TELEM* pMem = new unsigned int[MemLen];// len = bitlen
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = (BitLen + BITS_IN_ONE_MEM - 1) / BITS_IN_ONE_MEM;// 8*sizeof(TELEM)
	TELEM* pMem = new unsigned int[MemLen];
	for (int i = 0; i < bf.MemLen; i++)			///????? указатель
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

struct Index TBitField::GetMemIndex(const unsigned int n) const // индекс Мем для бита n
{
	struct Index I;
	I.Index_Mem = n / BITS_IN_ONE_MEM;
	I.Index_in_Mem = (n % BITS_IN_ONE_MEM > 0)? n % BITS_IN_ONE_MEM - 1: BITS_IN_ONE_MEM; // проверка для отрицательных и 0
	return I;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	struct Index I = GetMemIndex(n);
	TELEM mask = (1u << I.Index_in_Mem);
	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n > BitLen))
		throw 2;
	TELEM mask, struct Index I;
	mask = GetMemMask(n);
	I = GetMemIndex(n);
	pMem[I.Index_Mem] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	TELEM mask, struct Index I;
	mask = GetMemMask(n);
	I = GetMemIndex(n);
	pMem[I.Index_Mem] = pMem[I.Index_Mem] & (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	TELEM mask, struct Index I;
	mask = GetMemMask(n);
	I = GetMemIndex(n);
	if ((pMem[I.Index_Mem] & mask) != 0)
	{
		return 1;
	}
	return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField & bf) // присваивание
{
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	for (int i = 0; i < bf.MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

bool TBitField::operator==(const TBitField & bf) const // сравнение
{
	if (BitLen != bf.BitLen)// что делать с мусором
	{
		return false;
	}
	else
	{
		for (int i = 0; i < MemLen -1; i++)
		{
			if (pMem[i] != bf.pMem[i])
			{
				return false;
			}
		}
	}
	for (int i = BITS_IN_ONE_MEM * MemLen; i < BitLen - 1; i++)
	{
		if (GetBit(i) != bf.GetBit(i))
		{
			return false;
		}
	}
	return true;
}

bool TBitField::operator!=(const TBitField & bf) const // сравнение
{
	if (BitLen != bf.BitLen)// что делать с мусором
	{
		return true;
	}
	else
	{
		for (int i = 0; i < MemLen - 1; i++)
		{
			if (pMem[i] != bf.pMem[i])
			{
				return true;
			}
		}
	}
	for (int i = BITS_IN_ONE_MEM * MemLen; i < BitLen - 1; i++)
	{
		if (GetBit(i) != bf.GetBit(i))
		{
			return true;
		}
	}
	return false;
}

TBitField TBitField::operator|(const TBitField & bf) // операция "или"
{
	int i, len = BitLen;
	if (bf.BitLen > len)
	{
		len = bf.BitLen;
	}
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++)
	{
		temp.pMem[i] |= bf.pMem[i];
	}
	return temp;
}

TBitField TBitField::operator&(const TBitField & bf) // операция "и"
{
	int i, len = BitLen;
	if (bf.BitLen > len)
	{
		len = bf.BitLen;
	}
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = pMem[i];
	}
	for (int i = 0; i < MemLen; i++)
	{
		temp.pMem[i] &= bf.pMem[i];
	}
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	int i, len = BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = ~pMem[i];
	}
	return temp;
}

// ввод/вывод

istream& operator>>(istream & istr, TBitField & bf) // ввод
{
	char c;
	for (int i = 0; i < bf.BitLen; i++)
	{
		istr >> c;
		if (c == '0')
		{
			bf.ClrBit(i);
		}
		else
		{
			bf.SetBit(i);
		}
	}
  return istr;
}

ostream& operator<<(ostream & ostr, const TBitField & bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
