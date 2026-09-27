#include <stdio.h>

void bubbleSort(int arr[], int n);
int binarySearch(int arr[], int n, int key);

int main() {
    int n, i, key, result;

    printf("Enter the size of the array: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid array size.\n");
        return 1;
    }
    int arr[n];

    for (i = 0; i < n; i++) {
        printf("Enter element %d: ", i + 1);
        scanf("%d", &arr[i]); 
    }

    bubbleSort(arr, n);

    printf("\nSorted array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\nEnter the element key to search: ");
    scanf("%d", &key);

    result = binarySearch(arr, n, key);

    if (result == -1) {
         printf("Result: -1 (Element not found)\n");
    } else {
        printf("Possion: %d (Element found in the array)\n",result);
    }

    return 0;
}

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


int binarySearch(int arr[], int n, int key) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == key) {
            return mid; 

        }
        else if (arr[mid] < key) {
            low = mid + 1; 
        }
        else {
            high = mid - 1; 
        }
    }

    return -1; 
}
