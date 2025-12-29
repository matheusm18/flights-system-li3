#ifndef MIN_HEAP_H
#define MIN_HEAP_H

typedef struct MinHeap MinHeap;

typedef struct aircraftcount AircraftCount;


MinHeap* create_min_heap(int capacity);
void destroy_min_heap(MinHeap* h);


void insert_min_heap(MinHeap* h, const char* aircraft_id, int flight_count);


int min_heap_size(const MinHeap* h);
const char* get_aicraft_id_min_heap(const MinHeap* h, int index);
int get_flight_count_min_heap(const MinHeap* h, int index);

void sort_desc_min_heap(MinHeap* h);

#endif
