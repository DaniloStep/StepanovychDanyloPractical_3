#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

struct Statistics {
    long long comparisons = 0;
    long long movements = 0;
    long long swaps = 0;
    long long passes = 0;
};

void copyArray(const int* source, int* destination, int n) {
    for (int i = 0; i < n; i++)
        destination[i] = source[i];
}

void printArray(const int* a, int n) {
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;
}

bool isSorted(const int* a, int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i])
            return false;
    }
    return true;
}

bool sameElements(const int* a, const int* b, int n) {
    int* x = new int[n];
    int* y = new int[n];

    copyArray(a, x, n);
    copyArray(b, y, n);

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (x[j] > x[j + 1]) {
                int t = x[j];
                x[j] = x[j + 1];
                x[j + 1] = t;
            }

            if (y[j] > y[j + 1]) {
                int t = y[j];
                y[j] = y[j + 1];
                y[j + 1] = t;
            }
        }
    }

    bool result = true;

    for (int i = 0; i < n; i++) {
        if (x[i] != y[i]) {
            result = false;
            break;
        }
    }

    delete[] x;
    delete[] y;

    return result;
}

bool validate(const int* result, const int* original, int n) {
    return isSorted(result, n) &&
        sameElements(result, original, n);
}

void swapElements(int& a, int& b, Statistics& s) {
    int temp = a;
    a = b;
    b = temp;

    s.movements += 3;
    s.swaps++;
}

// Binary search for insertion position.
// Equal elements are placed before the new element.
int binarySearchPosition(
    const int* a,
    int left,
    int right,
    int key,
    Statistics& s
) {
    while (left <= right) {
        int mid = left + (right - left) / 2;

        s.comparisons++;

        if (a[mid] <= key)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return left;
}

// 1. Regular insertion sort
void insertionSort(int* a, int n, Statistics& s) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;

        s.passes++;

        while (j >= 0) {
            s.comparisons++;

            if (a[j] > key) {
                a[j + 1] = a[j];
                s.movements++;
                j--;
            }
            else {
                break;
            }
        }

        a[j + 1] = key;
        s.movements++;
    }
}

// 2. Binary insertion sort
void binaryInsertionSort(int* a, int n, Statistics& s) {
    for (int i = 1; i < n; i++) {
        int key = a[i];

        s.passes++;

        int position = binarySearchPosition(
            a, 0, i - 1, key, s
        );

        int j = i;

        while (j > position) {
            a[j] = a[j - 1];
            s.movements++;
            j--;
        }

        a[position] = key;
        s.movements++;
    }
}

// 3. Bubble sort
void bubbleSort(int* a, int n, Statistics& s) {
    for (int i = 0; i < n - 1; i++) {
        bool changed = false;

        s.passes++;

        for (int j = 0; j < n - i - 1; j++) {
            s.comparisons++;

            if (a[j] > a[j + 1]) {
                swapElements(a[j], a[j + 1], s);
                changed = true;
            }
        }

        if (!changed)
            break;
    }
}

// 4. Shaker sort
void shakerSort(int* a, int n, Statistics& s) {
    int left = 0;
    int right = n - 1;

    while (left < right) {
        bool changed = false;

        // Forward pass
        s.passes++;

        for (int i = left; i < right; i++) {
            s.comparisons++;

            if (a[i] > a[i + 1]) {
                swapElements(a[i], a[i + 1], s);
                changed = true;
            }
        }

        right--;

        if (!changed)
            break;

        changed = false;

        // Backward pass
        s.passes++;

        for (int i = right; i > left; i--) {
            s.comparisons++;

            if (a[i - 1] > a[i]) {
                swapElements(a[i - 1], a[i], s);
                changed = true;
            }
        }

        left++;

        if (!changed)
            break;
    }
}

typedef void (*SortFunction)(int*, int, Statistics&);

void runAlgorithm(
    const string& name,
    SortFunction algorithm,
    const int* original,
    int n
) {
    int* a = new int[n];
    copyArray(original, a, n);

    Statistics s;
    algorithm(a, n, s);

    cout << "\n" << name << endl;
    cout << "Result: ";
    printArray(a, n);

    cout << "Comparisons: " << s.comparisons << endl;
    cout << "Movements: " << s.movements << endl;
    cout << "Swaps: " << s.swaps << endl;
    cout << "Passes: " << s.passes << endl;

    cout << "Validation: "
        << (validate(a, original, n) ? "PASSED" : "FAILED")
        << endl;

    delete[] a;
}

bool runOneTest(const int* source, int n) {
    SortFunction algorithms[4] = {
        insertionSort,
        binaryInsertionSort,
        bubbleSort,
        shakerSort
    };

    for (int k = 0; k < 4; k++) {
        int* a = new int[n];
        copyArray(source, a, n);

        Statistics s;
        algorithms[k](a, n, s);

        bool ok = validate(a, source, n);
        delete[] a;

        if (!ok)
            return false;
    }

    return true;
}

// 12 automatic sorting tests
void sortingTests() {
    const int testCount = 12;

    int tests[testCount][5] = {
        {1, 2, 3, 4, 5},
        {5, 4, 3, 2, 1},
        {3, 1, 4, 2, 5},
        {1, 1, 1, 1, 1},
        {-5, -3, -1, -4, -2},
        {0, -1, 5, -3, 2},
        {9, 8, 7, 6, 5},
        {2, 2, 1, 1, 3},
        {10, 0, -10, 5, -5},
        {4, 1, 4, 2, 4},
        {7, 3, 9, 1, 6},
        {0, 0, 0, 0, 0}
    };

    int passed = 0;

    cout << "\n========== SORTING TESTS ==========\n";

    for (int i = 0; i < testCount; i++) {
        bool ok = runOneTest(tests[i], 5);

        if (ok)
            passed += 4;

        cout << "Test " << i + 1 << ": "
            << (ok ? "PASSED" : "FAILED") << endl;
    }

    cout << "\nTotal checks: " << testCount * 4 << endl;
    cout << "Passed checks: " << passed << endl;
}

// Binary insertion position tests
void positionTests() {
    int a[5] = { 1, 3, 5, 7, 9 };

    int keys[6] = { 0, 4, 10, 5, 3, 6 };
    int expected[6] = { 0, 2, 5, 3, 2, 3 };

    int passed = 0;

    cout << "\n========== POSITION TESTS ==========\n";

    for (int i = 0; i < 6; i++) {
        Statistics s;

        int position = binarySearchPosition(
            a, 0, 4, keys[i], s
        );

        bool ok = (position == expected[i]);

        if (ok)
            passed++;

        cout << "Key: " << keys[i]
            << ", position: " << position
            << ", expected: " << expected[i]
            << " - " << (ok ? "PASSED" : "FAILED")
            << endl;
    }

    Statistics s;

    int emptyPosition = binarySearchPosition(
        a, 0, -1, 5, s
    );

    if (emptyPosition == 0)
        passed++;

    int duplicates[4] = { 2, 2, 2, 2 };

    int duplicatePosition = binarySearchPosition(
        duplicates, 0, 3, 2, s
    );

    if (duplicatePosition == 4)
        passed++;

    cout << "Empty range: "
        << (emptyPosition == 0 ? "PASSED" : "FAILED")
        << endl;

    cout << "Duplicate elements: "
        << (duplicatePosition == 4 ? "PASSED" : "FAILED")
        << endl;

    cout << "Passed position tests: " << passed << " / 8" << endl;
}

// Create experiment arrays
void createArray(int* a, int n, int type) {
    for (int i = 0; i < n; i++) {
        if (type == 0) {
            a[i] = i;
        }
        else if (type == 1) {
            a[i] = n - i;
        }
        else if (type == 2) {
            a[i] = i;
        }
        else {
            a[i] = (i * 7 + i / 3) % 17;
        }
    }

    // Make the third type almost sorted
    if (type == 2) {
        for (int i = 0; i + 1 < n; i += 10) {
            int temp = a[i];
            a[i] = a[i + 1];
            a[i + 1] = temp;
        }
    }
}

// Run all experiments
void experiments() {
    int sizes[3] = { 10, 100, 1000 };

    string types[4] = {
        "Ascending",
        "Descending",
        "Almost sorted",
        "Mixed with duplicates"
    };

    string names[4] = {
        "Insertion sort",
        "Binary insertion sort",
        "Bubble sort",
        "Shaker sort"
    };

    SortFunction algorithms[4] = {
        insertionSort,
        binaryInsertionSort,
        bubbleSort,
        shakerSort
    };

    cout << "\n========== EXPERIMENTS ==========\n";

    for (int sizeIndex = 0; sizeIndex < 3; sizeIndex++) {
        int n = sizes[sizeIndex];

        cout << "\nArray size: " << n << endl;

        for (int type = 0; type < 4; type++) {
            int* original = new int[n];
            createArray(original, n, type);

            cout << "\nArray type: " << types[type] << endl;

            for (int alg = 0; alg < 4; alg++) {
                int* a = new int[n];
                copyArray(original, a, n);

                Statistics s;
                algorithms[alg](a, n, s);

                cout << "\n" << names[alg] << endl;
                cout << "Comparisons: " << s.comparisons << endl;
                cout << "Movements: " << s.movements << endl;
                cout << "Swaps: " << s.swaps << endl;
                cout << "Passes: " << s.passes << endl;

                cout << "Validation: "
                    << (validate(a, original, n)
                        ? "PASSED" : "FAILED")
                    << endl;

                delete[] a;
            }

            delete[] original;
        }
    }
}

// Manual array input
void manualInput() {
    int n;

    cout << "Enter array size (1-1000): ";
    cin >> n;

    if (cin.fail() || n < 1 || n > 1000) {
        cout << "Invalid array size!" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    int* a = new int[n];

    cout << "Enter " << n << " integers:\n";

    for (int i = 0; i < n; i++) {
        cin >> a[i];

        if (cin.fail()) {
            cout << "Invalid input!" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            delete[] a;
            return;
        }
    }

    cout << "\nOriginal array: ";
    printArray(a, n);

    runAlgorithm("Insertion sort", insertionSort, a, n);
    runAlgorithm("Binary insertion sort", binaryInsertionSort, a, n);
    runAlgorithm("Bubble sort", bubbleSort, a, n);
    runAlgorithm("Shaker sort", shakerSort, a, n);

    delete[] a;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice;

    do {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Enter array and sort it\n";
        cout << "2. Run automatic sorting tests\n";
        cout << "3. Test binary search position\n";
        cout << "4. Run experiments\n";
        cout << "0. Exit\n";

        cout << "\nYour choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!" << endl;
            continue;
        }

        switch (choice) {
        case 1:
            manualInput();
            break;

        case 2:
            sortingTests();
            break;

        case 3:
            positionTests();
            break;

        case 4:
            experiments();
            break;

        case 0:
            cout << "Program finished." << endl;
            break;

        default:
            cout << "Invalid menu option!" << endl;
        }

    } while (choice != 0);

    return 0;
}
