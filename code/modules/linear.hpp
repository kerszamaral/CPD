#pragma once
#include "common.hpp"

namespace sort
{
    class Linear
    {
    private:
        static int getMax(array_t &array)
        {
            int max = array[0];
            for (size_t i = 0; i < array.size(); i++)
            {
                if (array[i] > max)
                {
                    max = array[i];
                }
            }
            return max;
        }

        static void countingSortPLC(array_t &array, int place)
        {
            const int max = 10;
            std::vector<int> output(array.size(), 0);
            std::vector<int> count(max, 0);

            // Calculate count of elements
            for (size_t i = 0; i < array.size(); i++)
                count[(array[i] / place) % 10]++;

            // Calculate cumulative count
            for (int i = 1; i < max; i++)
                count[i] += count[i - 1];

            // Place the elements in sorted order
            for (int i = array.size() - 1; i >= 0; i--)
            {
                output[count[(array[i] / place) % 10] - 1] = array[i];
                count[(array[i] / place) % 10]--;
            }

            for (size_t i = 0; i < array.size(); i++)
                array[i] = output[i];
        }

        static void radixSortMSD(array_t &array)
        {
            // Get maximum element
            int max = getMax(array);
            int start;
            for (start = 1; start < max; start *=10)
            {
            }

            // Apply counting sort to sort elements based on place value.
            for (int place = start; place >= 1; place /= 10)
            {
                countingSortPLC(array, place);
            }
        }

        static void radixSortLSD(array_t &array)
        {
            // Get maximum element
            int max = getMax(array);

            // Apply counting sort to sort elements based on place value.
            for (int place = 1; max / place > 0; place *= 10)
                countingSortPLC(array, place);
        }

        static void countingSort(array_t &array)
        {
            array_t ordered;
            ordered.resize(array.size());

            std::vector<int> count(getMax(array) + 1, 0);
            for (size_t i = 0; i < array.size(); i++)
            {
                count[array[i]]++;
            }
            for (size_t i = 1; i < count.size(); i++)
            {
                count[i] += count[i - 1];
            }
            for (size_t i = array.size() - 1; i >= 0; i--)
            {
                ordered[count[array[i]] - 1] = array[i];
                count[array[i]]--;
            }
            array = ordered;
        }

    public:
        static void radixsortMSD(array_t &array, loginfo_t &loginfo)
        {
            radixSortMSD(array);
        }

        static void radixsortLSD(array_t &array, loginfo_t &loginfo)
        {
            radixSortLSD(array);
        }

        static void countingsort(array_t &array, loginfo_t &loginfo)
        {
            countingSort(array);
        }
    };
}