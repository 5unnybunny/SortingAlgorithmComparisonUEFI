#include <efi.h>
#include <efilib.h>



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



EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	EFI_TIME* time;
	UINTN Hours;
	UINTN Minutes;
	UINTN Seconds;
	uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
	uint64_t tick;
	srand(rdtsc());
	UINTN size = rand();
	
	tick = rdtsc();
	uefi_call_wrapper(BS->Stall, 1, 1000000);
	uint64_t TPS = rdtsc()-tick;
	Print(L"Ticks Per Second: %lu\n", TPS);
	Print(L"Results for %u sized array\n", size);
	int arr[size];
	
	//bubblesort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	uefi_call_wrapper(RT->GetTime, 2, time, NULL);
	tick = rdtsc();
	bubble_sort(arr, size);
	Print(L"Bubble Sort ticks: %lu\n", rdtsc()-tick);
	Hours = time->Hour;
	Minutes = time->Minute;
	Seconds = time->Second;
	uefi_call_wrapper(RT->GetTime, 2, time, NULL);
	if(Hours>time->Hour)
		Hours = 24 + time->Hour - Hours;
	else
		Hours = time->Hour - Hours;
	if(Minutes>time->Minute)
		Minutes = 60 + time->Minute - Minutes;
	else
		Minutes = time->Minute - Minutes;
	if(Seconds > time->Second)
		Seconds = 60 + time->Second - Seconds;
	else
		Seconds = time->Second - Seconds;
	Print(L"Bubble Sort time: H:%u M:%u S:%u\n", Hours, Minutes, Seconds);
	
	//insertsort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	uefi_call_wrapper(RT->GetTime, 2, time, NULL);
	tick = rdtsc();
	insertion_sort(arr, size);
	Print(L"Insertion Sort ticks: %lu\n", rdtsc()-tick);
	Hours = time->Hour;
	Minutes = time->Minute;
	Seconds = time->Second;
	uefi_call_wrapper(RT->GetTime, 2, time, NULL);
	if(Hours>time->Hour)
		Hours = 24 + time->Hour - Hours;
	else
		Hours = time->Hour - Hours;
	if(Minutes>time->Minute)
		Minutes = 60 + time->Minute - Minutes;
	else
		Minutes = time->Minute - Minutes;
	if(Seconds > time->Second)
		Seconds = 60 + time->Second - Seconds;
	else
		Seconds = time->Second - Seconds;
	Print(L"Insertion Sort time: H:%u M:%u S:%u\n", Hours, Minutes, Seconds);
	
	//quicksort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	uefi_call_wrapper(RT->GetTime, 2, time, NULL);
	tick = rdtsc();
	quicksort(arr, size);
	Print(L"Quicksort ticks: %lu\n", rdtsc()-tick);
	Hours = time->Hour;
	Minutes = time->Minute;
	Seconds = time->Second;
	uefi_call_wrapper(RT->GetTime, 2, time, NULL);
	if(Hours>time->Hour)
		Hours = 24 + time->Hour - Hours;
	else
		Hours = time->Hour - Hours;
	if(Minutes>time->Minute)
		Minutes = 60 + time->Minute - Minutes;
	else
		Minutes = time->Minute - Minutes;
	if(Seconds > time->Second)
		Seconds = 60 + time->Second - Seconds;
	else
		Seconds = time->Second - Seconds;
	Print(L"Quicksort time: H:%u M:%u S:%u\n", Hours, Minutes, Seconds);
	
	return EFI_SUCCESS;
}
