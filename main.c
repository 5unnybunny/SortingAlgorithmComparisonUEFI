#include <efi.h>
#include <efilib.h>


//ALGORITHMS
static unsigned long int next = 1;


uint64_t rdtsc(){
	unsigned int lo,hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)hi << 32) | lo;
}


void srand(unsigned int seed){
	next = seed;
}


int rand(void){
	next = next * 1103515245 + 12345;
	return (unsigned int)(next/65536) % 32768;
}


void bubble_sort (int *a, int n) {
	int i, t, j = n, s = 1;
	while (s) {
		s = 0;
		for (i = 1; i < j; i++) {
			if (a[i] < a[i - 1]) {
				t = a[i];
				a[i] = a[i - 1];
				a[i - 1] = t;
				s = 1;
			}
		}
		j--;
	}
}


void insertion_sort(int *a, const int n) {
	for(int i = 1; i < n; ++i) {
		int key = a[i];
		int j = i;
		while( (j > 0) && (key < a[j - 1]) ) {
			a[j] = a[j - 1];
			--j;
		}
		a[j] = key;
	}
}
 


void quicksort(int *A, int len) {
	if (len < 2) return;
 
	int pivot = A[len / 2];
 
	int i, j;
	for (i = 0, j = len - 1; ; i++, j--) {
 		while (A[i] < pivot) i++;
		while (A[j] > pivot) j--;
 
		if (i >= j) break;
 
		int temp = A[i];
		A[i]     = A[j];
		A[j]     = temp;
	}

	quicksort(A, i);
	quicksort(A + i, len - i);
}
//END OF ALGORITHMS



//EFI
EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	InitializeLib(ImageHandle, SystemTable);
	
	srand(rdtsc());
	uint64_t tick;
	uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);

	UINTN size = rand();
	
	Print(L"Results for %d sized array\n", size);
	int arr[size];
	
	//bubblesort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	
	tick = rdtsc();
	bubble_sort(arr, size);
	Print(L"Bubblesort cycles: %ld\n", rdtsc()-tick);
	
	//insertsort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	tick = rdtsc();
	insertion_sort(arr, size);
	Print(L"Insertion Sort cycles: %ld\n", rdtsc()-tick);
	
	
	//quicksort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	tick = rdtsc();
	quicksort(arr, size);
	Print(L"Quicksort cycles: %ld\n", rdtsc()-tick);

	return EFI_SUCCESS;
}
