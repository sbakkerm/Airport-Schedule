#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#define ROWS 2208


// PARSE THE FLIGHTS_FILE CSV AND STORE INTO ARRAY OF STRUCTS
typedef struct flight {
	char airline_code[3];
	char flight_number[5];
	char departure_airport[4];
	char arrival_airport[4];
	char departure_time[5];
	char arrival_time[5];
} dict;

// INITIALIZE FUNCTIONS
void printValues(dict values[]);
int readFile(dict values[]);
void swap(int *a, int *b);
void heapify_up(int array[], int i);
int heapify_down(int array[], int j);
void merge(int *Array, int p, int q, int r);
void mergeSort(int *Array, int p, int r);
void insertSort(int array[], int numElements);
void heapSort( int array[], int numElements);
void ComputeMaxAndMin(int arriveTimes[], int departTimes[], int numArrivals, int numDeparts);



/*-------------------------------------MAIN----------------------------------------------*/
int main(int argc, char* argv[]){

	dict values[ROWS]; // array of structs to store all data

	readFile(values); // reads file and updates values

	int departTimes[ROWS];
	int arriveTimes[ROWS];
	int numDeparts = 0;
	int numArrivals = 0;

	// Get arrival and departure time data
	for (int i = 0; i < ROWS; i++){
		if ( strcmp(values[i].departure_airport, "ATL") == 0){

			departTimes[numDeparts] = atoi( values[i].departure_time);
			numDeparts += 1; // plane is departing from ATL
		}
		else if ( strcmp(values[i].arrival_airport, "ATL") == 0){

			arriveTimes[numArrivals] = atoi( values[i].arrival_time);
			numArrivals += 1; // plane is arriving in ATL
		}
	}

	// Sort the respective arrays
	if (argc > 1){

		char* str = argv[1];

		// If "p" is an input argument
		if (strcmp(str, "heap") == 0){

			heapSort(arriveTimes, numArrivals);
			heapSort(departTimes, numDeparts);
		}
		else if (strcmp(str, "merge") == 0){

			mergeSort(arriveTimes, 0, numArrivals - 1);
			mergeSort(departTimes, 0, numDeparts - 1);
		}
		else {
			printf("Insertion sort executed by default.\n");
			insertSort(arriveTimes, numArrivals);
			insertSort(departTimes, numDeparts);
		}
	}
	else {
		// default is insertion sort
		insertSort(arriveTimes, numArrivals);
		insertSort(departTimes, numDeparts);
	}

	// print solution
	ComputeMaxAndMin(arriveTimes, departTimes, numArrivals, numDeparts);

	return 0;
}


/*-------------------------------------FUNCTIONS------------------------------------------*/

// Function prints the data
void printValues(dict values[]){

	for (int i = 0; i < ROWS; i++){
		printf("0->%s, ", values[i].airline_code);
		printf("1->%s, ", values[i].flight_number);
		printf("2->%s, ", values[i].departure_airport);
		printf("3->%s, ", values[i].arrival_airport);
		printf("4->%s, ", values[i].departure_time);
		printf("5->%s\n", values[i].arrival_time);
	}
}

// Function reads the data into the struct `dict`
int readFile(dict values[]){

	// Initialize the file pointer
	FILE *fp = fopen("flights_file.csv", "r"); // open in read mode

	if(!fp){
		printf("Error occurred");
		return 0;
	}

	char buff[ROWS]; // stores the first 2208 lines into buff
	int row_count = 0;
	int field_count = 0;

	int i=0;
	while(fgets(buff, ROWS, fp)){
		//printf("%s", buff); // this will parse all data

		field_count = 0;
		row_count++;

		strtok(buff, "\n");
		char *field = strtok(buff, ","); // separates buff at commas
		while (field){
			if (field_count == 0){ strcpy(values[i].airline_code, field); }
			else if (field_count == 1){ strcpy(values[i].flight_number, field); }
			else if (field_count == 2){ strcpy(values[i].departure_airport, field); }
			else if (field_count == 3){ strcpy(values[i].arrival_airport, field); }
			else if (field_count == 4){ strcpy(values[i].departure_time, field); }
			else if (field_count == 5){ strcpy(values[i].arrival_time, field); }

			field = strtok(NULL, ",");
			field_count++;
		}
		i++;
	}

	fclose(fp);
	return 0;
}

// Function swaps the values of two variables
void swap(int *a, int *b){

	int temp = *a;
	*a = *b;
	*b = temp;
}

// `i` is the element "inserted" into the heap
// Function executes heapify up
void heapify_up(int array[], int i){

	int parentIndex;

	// While `i` is not the root
	while (i > 0){

		parentIndex = (int)floor((i-1) / 2);

		// if the current node node is greater than its parent
		if (array[i] > array[parentIndex]){

			swap(&array[i], &array[parentIndex]);
			i = parentIndex;
		}
		else {
			i = 0;
		}
	}
}

// `j` is the new location of the element "removed" from the heap
// Function executes heapify down
int heapify_down(int array[], int j){

	//int i = 0; // begin current index at the root
	int i = 0;
	int leftChild; // holds index of left child
	int rightChild; // holds index of right child
	int numChildren; // can be 0, 1, 2

	// While `i` is in the heap
	while (i < j){

		leftChild = 2*i + 1;
		rightChild = 2*i + 2;

		// Count Children
		numChildren = 0; // no children initially
		if (leftChild < j){ numChildren += 1; } // has a left child
		if (rightChild < j){ numChildren += 1; } // has a right child

		// Heapify is complete if there are no more children to compare with
		if (numChildren == 0){
			return 0;
		}
		else if (numChildren == 2){

			// Either of the children are larger than the current element
			if ( (array[leftChild] > array[i]) || (array[rightChild] > array[i])){

				// left child has larger value
				if (array[leftChild] > array[rightChild]){

					swap(&array[i], &array[leftChild]);
					i = leftChild;
				}
				else {

					swap(&array[i], &array[rightChild]);
					i = rightChild;
				}
			}
			else {
				return 0; // Heap property is satisfied
			}
		}

		else if (numChildren == 1){

			// Child is larger than the current element
			if (array[leftChild] > array[i]){

				swap(&array[i], &array[leftChild]);
				i = leftChild;
			}
			else{
				return 0; // Heap property is satisfied
			}
		}

	} // end while loop

	return 0;
}

// p = first index of left array
// q = last index of left array
// r = last index of right array
// Function executes the merge used for merge sort
void merge(int *Array, int p, int q, int r){

	// create a temp array
	int temp[r - p + 1];

	// crawlers for both intervals and for temp
	int i = p, j = q+1, k = 0;

	// traverse both arrays and insert items into temp
	while(i <= q && j <= r) {
		if(Array[i] <= Array[j]) {
			temp[k] = Array[i];
			k += 1; i += 1;
		}
		else {
			temp[k] = Array[j];
			k += 1; j += 1;
		}
	}

	// add elements leftover from the left interval
	while(i <= q) {
		temp[k] = Array[i];
		k += 1; i += 1;
	}

	// add elements leftover in the right interval
	while(j <= r) {
		temp[k] = Array[j];
		k += 1; j += 1;
	}

	// copy temp to original array
	for(i = p; i <= r; i += 1) {
		Array[i] = temp[i - p];
	}
}

// Function executes merge sort
void mergeSort(int *Array, int p, int r){

	if(p < r) {

		int q = (p + r) / 2;
		mergeSort(Array, p, q);
		mergeSort(Array, q+1, r);
		merge(Array, p, q, r);
	}
}

// Function executes insertion sort
void insertSort(int array[], int numElements){

	int key, i, j;

	for (i = 1; i < numElements; i++){

		key = array[i];

		// insert key into sorted part array[0...i-1]
		for (j = i-1; j >= 0 && array[j] > key; j--){

			array[j+1] = array[j];
		}
		array[j+1] = key;
	}
}

// Function executes heap sort
void heapSort( int array[], int numElements){

	if (numElements > 1){

		for(int i = 1; i < numElements; i++){

			heapify_up(array, i);
		}

		for(int j = (numElements - 1); j > 0; j--){

			swap(&array[0], &array[j]);
			heapify_down(array, j);
		}
	}
	else {
		printf("Array is too small for heap sort.");
	}
}

// Function computes max and min values and prints the gates needed, planes parked by midnight
void ComputeMaxAndMin(int arriveTimes[], int departTimes[], int numArrivals, int numDeparts){

	int departIndex = 0;
	int arriveIndex = 0;
	int gates = 0;
	int minimum = INT_MAX;
	int maximum = INT_MIN;

	// arrivals always occur before departures (at the same time)
	// iterate through all the sorted arrivals and departures
	while (arriveIndex + departIndex < ROWS){

		if (departIndex >= numDeparts){
			gates += 1; // plane arrives
			arriveIndex += 1;
		}
		else if (arriveIndex >= numArrivals){
			gates -= 1; // plane departs
			departIndex += 1;
		}
		else {
			if(arriveTimes[arriveIndex] <= departTimes[departIndex]){
				gates += 1; // plane arrives
				arriveIndex += 1;
			}
			else {
				gates -=1; // plane departs
				departIndex += 1;
			}
		}
		if (gates < minimum){ minimum = gates; }
		if (gates > maximum){maximum = gates; }
	}

	printf("Gates needed = %d\n", maximum - minimum);
	printf("Planes parked by midnight = %d", abs(minimum));
}







