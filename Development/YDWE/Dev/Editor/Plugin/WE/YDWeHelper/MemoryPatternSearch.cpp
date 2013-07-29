#include <algorithm>
#include <functional>
#include <aero/aero.hpp>

#include "MemoryPatternSearch.h"

namespace NYDWE
{

/** \brief Pattern search predicate
 *
 * Pattern search predicate for searching memories
 */
struct CPatternSearchPredicate
{
	bool operator()(uint8_t b, uint16_t pattern) const
	{
		uint8_t m = aero::number_split<2, 1>(pattern);
		uint8_t v = aero::number_split<2, 0>(pattern);

		return (m & b) == (m & v);
	}
};

void *MemoryPatternSearch(void *first1, void *last1, const uint16_t *first2, const uint16_t *last2)
{
	CPatternSearchPredicate pred;

	void *result = std::search(
		reinterpret_cast<uint8_t *>(first1),
		reinterpret_cast<uint8_t *>(last1),
		first2,
		last2,
		pred
	);

	if (result == last1)
		result = NULL;

	return result;
}

void *MemoryPatternSearch(void *searchStart, uintptr_t searchLength, uint16_t *patchPattern, uint32_t patternLength)
{
	return MemoryPatternSearch(
		searchStart,
		aero::p_sum<aero::pointer_type>(searchStart, searchLength),
		&patchPattern[0],
		&patchPattern[patternLength / sizeof(uint16_t)]
	);
}

} // namespace NYDWE
