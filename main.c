#include <efi.h>
#include <efilib.h>


static unsigned long int next = 1;


uint64_t rdtsc(){
   uint32_t hi, lo;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
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


int pow_pos(unsigned int base, unsigned int exponent){
	unsigned int j = base;
	if(exponent>1){
		for(int i = exponent; i>1; --i)
			j = j*base;
	}
	return j;
}


EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	CHAR16 **Argv;
	EFI_TIME* time = NULL;
	
	InitializeLib(ImageHandle, SystemTable);
	GetShellArgcArgv(ImageHandle, &Argv);
	
	uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
	uint64_t tick;
	uefi_call_wrapper(RT->GetTime, 2, time, NULL);
	srand(time->Hour * time->Minute * time->Second * rdtsc());
	
	UINTN inputsize;
	for(inputsize = 0; Argv[1][inputsize] != '\0'; ++inputsize);
	
	UINTN size = 0;
	for(int i = inputsize-1; i>=0; --i){
		size = size + (pow_pos(10, inputsize-i) * ((unsigned int)(Argv[1][i] - '0')));
	}
	size = size/10;
	
	//GetTime Nanosecond doesn't work on all hardware hence TPS, which can be used to obtain desired time from ticks
	tick = rdtsc();
	uefi_call_wrapper(BS->Stall, 1, 1000000);
	uint64_t TPS = rdtsc()-tick;
	uint64_t TPNS = TPS/1000000000;
	Print(L"Ticks Per NanoSecond: %lu.%lu\n", TPNS, TPS-TPNS*1000000000);
	Print(L"Results for %u sized array\n", size);
	int arr[size];

	//bubblesort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	tick = rdtsc();
	bubble_sort(arr, size);
	Print(L"Bubble Sort ticks: %lu\n", rdtsc()-tick);

	//insertsort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	tick = rdtsc();
	insertion_sort(arr, size);
	Print(L"Insertion Sort ticks: %lu\n", rdtsc()-tick);

	//quicksort
	for(int i=0; i<size; i++){
		arr[i] = rand();
	}
	tick = rdtsc();
	quicksort(arr, size);
	Print(L"Quicksort ticks: %lu\n", rdtsc()-tick);
	
	return EFI_SUCCESS;
}
