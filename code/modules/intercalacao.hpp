#pragma once
#include "common.hpp"

namespace sort
{
    class Intercalacao
    {
    private:
        static void intercala(int array[], int left, int mid, int right, loginfo_t &loginfo)
        {
            auto const subArrayOne = mid - left + 1;
            auto const subArrayTwo = right - mid;

            // Create temp arrays
            auto *leftArray = new int[subArrayOne],
                 *rightArray = new int[subArrayTwo];

            // Copy data to temp arrays leftArray[] and rightArray[]
            for (auto i = 0; i < subArrayOne; i++)
                leftArray[i] = array[left + i];
            for (auto j = 0; j < subArrayTwo; j++)
                rightArray[j] = array[mid + 1 + j];

            auto indexOfSubArrayOne = 0,   // Initial index of first sub-array
                indexOfSubArrayTwo = 0;    // Initial index of second sub-array
            int indexOfMergedArray = left; // Initial index of merged array

            // Merge the temp arrays back into array[left..right]
            while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
            {
                if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo])
                {
                    array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                    indexOfSubArrayOne++;
                }
                else
                {
                    array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                    indexOfSubArrayTwo++;
                }
                indexOfMergedArray++;
            }
            // Copy the remaining elements of
            // left[], if there are any
            while (indexOfSubArrayOne < subArrayOne)
            {
                array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                indexOfSubArrayOne++;
                indexOfMergedArray++;
            }
            // Copy the remaining elements of
            // right[], if there are any
            while (indexOfSubArrayTwo < subArrayTwo)
            {
                array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                indexOfSubArrayTwo++;
                indexOfMergedArray++;
            }
            delete[] leftArray;
            delete[] rightArray;
        }

        static void mergeSort(array_t &array, int const begin, int const end, loginfo_t &loginfo)
        {
            if (begin >= end)
                return; // Returns recursively

            auto mid = begin + (end - begin) / 2;
            mergeSort(array, begin, mid, loginfo);
            mergeSort(array, mid + 1, end, loginfo);
            intercala(array.data(), begin, mid, end, loginfo);
        }

    public:
        static void merge(const array_t &array1, const array_t &array2, array_t &array_final, loginfo_t &loginfo)
        {
            int i = 0, j = 0;
            int qtd_a1 = array1.size();
            int qtd_a2 = array2.size();

            array_final.clear();
            while (i < qtd_a1 && j < qtd_a2)
            {
                if (array1[i] <= array2[j])
                    array_final.push_back(array1[i++]);
                else
                    array_final.push_back(array2[j++]);
            }

            if (j < qtd_a2 && i >= qtd_a1) // array 1 terminou
                for (auto e = j; e < qtd_a2; e++)
                    array_final.push_back(array2[e]);

            if (i < qtd_a1 && j >= qtd_a2) // array 2 terminou
                for (auto e = i; e < qtd_a1; e++)
                    array_final.push_back(array1[e]);
            // TODO: atualizar loginfo
        }

        static void two_way_merge(const std::vector<array_t> arrays, array_t &array_final, loginfo_t &loginfo)
        {
            array_final.clear();

            for (long long unsigned int i = 0; i < arrays.size(); i += 2)
            {
                array_t array1 = arrays[i];
                array_t array2 = arrays[i + 1];
                array_t array_final_temp;
                merge(array1, array2, array_final_temp, loginfo);
                array_final = array_final_temp;
            }
            // TODO: atualizar loginfo
        }

        static void multi_way_merge(const std::vector<array_t> arrays, array_t &array_final, loginfo_t &loginfo)
        {
            array_final.clear();

            // multi way merge
            while (true)
            {
                long long unsigned int i = 0;
                for (auto array : arrays)
                {
                    if (array.empty())
                    {
                        i++;
                        break;
                    }
                    array_final.emplace_back(array[0]);
                    array.erase(array.begin());
                }

                if (i == arrays.size())
                {
                    break;
                }
            }

            heap::buildheap(array_final, loginfo);
            // TODO: atualizar loginfo
        }

        static void mergesort(array_t &array, loginfo_t &loginfo)
        {
            mergeSort(array, 0, array.size() - 1, loginfo);
        }
    };
}