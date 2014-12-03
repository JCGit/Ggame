

#ifndef _BITSET_H_
#define _BITSET_H_

#include "../def/define.h"

class BitSet32 
{
private:
	uint32 mbits;

public:
	BitSet32()                         { mbits = 0; }
	BitSet32(const BitSet32& in_rCopy) { mbits = in_rCopy.mbits; }
	BitSet32(const uint32 in_mask)     { mbits = in_mask; }

	operator uint32() const             { return mbits; }
	uint32 getMask() const              { return mbits; }

	/// Set all bits to true.
	void set()                          { mbits  = 0xFFFFFFFFUL; }

	/// Set the specified bit(s) to true.
	void set(const uint32 m)            { mbits |= m; }

	/// Masked-set the bitset; ie, using s as the mask and then setting the masked bits
	/// to b.
	void set(BitSet32 s, bool b)       { mbits = (mbits&~(s.mbits))|(b?s.mbits:0); }

	/// Clear all bits.
	void clear()                       { mbits  = 0; }

	/// Clear the specified bit(s).
	void clear(const uint32 m)          { mbits &= ~m; }

	/// Toggle the specified bit(s).
	void toggle(const uint32 m)         { mbits ^= m; }

	/// Are any of the specified bit(s) set?
	bool test(const uint32 m) const     { return (mbits & m) != 0; }

	/// Are ALL the specified bit(s) set?
	bool testStrict(const uint32 m) const { return (mbits & m) == m; }

	/// @name Operator Overloads
	/// @{
	BitSet32& operator =(const uint32 m)  { mbits  = m;  return *this; }
	BitSet32& operator|=(const uint32 m)  { mbits |= m; return *this; }
	BitSet32& operator&=(const uint32 m)  { mbits &= m; return *this; }
	BitSet32& operator^=(const uint32 m)  { mbits ^= m; return *this; }

	BitSet32 operator|(const uint32 m) const { return BitSet32(mbits | m); }
	BitSet32 operator&(const uint32 m) const { return BitSet32(mbits & m); }
	BitSet32 operator^(const uint32 m) const { return BitSet32(mbits ^ m); }
	/// @}
};


#endif //_NBITSET_H_
