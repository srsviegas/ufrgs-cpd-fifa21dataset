#include <vector>

/**
 * Sorts a vector in descending order using insertion sort.
 *
 * @tparam T The type of elements in the vector.
 * @param vector The vector to be sorted.
 */
template <typename T>
void sort_descending(std::vector<T>& vector) {
    int i, j;
    for (j = 1; j < vector.size(); j++) {
        T key = vector[j];
        i = j - 1;
        while (i >= 0 && vector[i] < key) {
            vector[i + 1] = vector[i];
            i--;
        }
        vector[i + 1] = key;
    }
}