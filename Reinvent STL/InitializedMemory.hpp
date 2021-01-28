#pragma once

/**
 * @brief Assigns the given value to the first count elements in the range beginning at first if count > 0. Does nothing otherwise
 * @param first the beginning of the range of elements to modify
 * @param count number of elements to modify
 * @param value the value to be assigned
 */
template<typename OutputIt, typename Size, typename T>
OutputIt fill_n(OutputIt first, Size count, const T& value)
{
    for (Size i = 0; i < count; ++i)
        *first++ = value;
    return first;
}