#include <stdlib.h>
#include <string.h>
#include "utils/min_heap.h"



struct aircraftcount {
    char* aircraft_id;
    int flight_count;
};

struct MinHeap {
    AircraftCount* heap;
    int size;
    int capacity;
};

MinHeap* create_min_heap(int capacity) {
    if (capacity <= 0) return NULL;

    MinHeap* h = malloc(sizeof(MinHeap));
    if (!h) return NULL;

    h->heap = malloc(sizeof(AircraftCount) * capacity);
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void destroy_min_heap(MinHeap* h) {
    if (!h) return;

    for (int i = 0; i < h->size; i++) {
        free(h->heap[i].aircraft_id);
    }
    free(h->heap);
    free(h);
}


static void swap_aircraft(AircraftCount* a, AircraftCount* b) {
    AircraftCount tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heapify_down(MinHeap* h, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    // comparar com lado esquerdo
    if (left < h->size) {
        if (h->heap[left].flight_count < h->heap[smallest].flight_count ||
            (h->heap[left].flight_count == h->heap[smallest].flight_count &&
             strcmp(h->heap[left].aircraft_id, h->heap[smallest].aircraft_id) > 0)) {

            smallest = left;
        }
    }
    
    // comparar com lado direito
    if (right < h->size) {
        if (h->heap[right].flight_count < h->heap[smallest].flight_count ||
            (h->heap[right].flight_count == h->heap[smallest].flight_count &&
             strcmp(h->heap[right].aircraft_id, h->heap[smallest].aircraft_id) > 0)) {

            smallest = right;
        }
    }
    
    if (smallest != idx) {
        swap_aircraft(&h->heap[idx], &h->heap[smallest]);
        heapify_down(h, smallest);
    }
}

static void heapify_up(MinHeap* h, int idx) {
    if (idx == 0) return;
    int parent = (idx - 1) / 2;
    
    // Subir se: menor flight_count OU (mesmo flight_count E maior ID)
    if (h->heap[idx].flight_count < h->heap[parent].flight_count ||
        (h->heap[idx].flight_count == h->heap[parent].flight_count &&
         strcmp(h->heap[idx].aircraft_id, h->heap[parent].aircraft_id) > 0)) {

        swap_aircraft(&h->heap[idx], &h->heap[parent]);

        heapify_up(h, parent);
    }
}



static int compare_aircraft_desc(const void* a, const void* b) {
    const AircraftCount* ac1 = a;
    const AircraftCount* ac2 = b;

    if (ac2->flight_count != ac1->flight_count)
        return ac2->flight_count - ac1->flight_count;

    return strcmp(ac1->aircraft_id, ac2->aircraft_id);
}



void insert_min_heap(MinHeap* h, const char* aircraft_id, int flight_count) {
    if (!h || !aircraft_id) return;
    
    if (h->size < h->capacity) { // se a heap não está cheia vai adicionar normalmente
        h->heap[h->size].aircraft_id = strdup(aircraft_id);

        if (!h->heap[h->size].aircraft_id) return;

        h->heap[h->size].flight_count = flight_count;

        heapify_up(h, h->size);

        h->size++;
    } else { // se a heap está cheia vai comparar com o topo que é o menor elemento
        
        int should_replace = 0;
        
        if (flight_count > h->heap[0].flight_count) {
            should_replace = 1;
        } else if (flight_count == h->heap[0].flight_count) {   // se tem o mesmo número de voos ent mantem o menor id
            if (strcmp(aircraft_id, h->heap[0].aircraft_id) < 0) {
                should_replace = 1;
            }
        }
        
        if (should_replace) {
            free(h->heap[0].aircraft_id);

            h->heap[0].aircraft_id = strdup(aircraft_id);

            if (!h->heap[0].aircraft_id) return;

            h->heap[0].flight_count = flight_count;

            heapify_down(h, 0);
        }
    }
}

int min_heap_size(const MinHeap* h) {
    return h ? h->size : 0;
}

const char* get_aicraft_id_min_heap(const MinHeap* h, int index) {
    if (!h || index < 0 || index >= h->size) return NULL;

    return h->heap[index].aircraft_id;
}

int get_flight_count_min_heap(const MinHeap* h, int index) {
    if (!h || index < 0 || index >= h->size) return 0;

    return h->heap[index].flight_count;
}

void sort_desc_min_heap(MinHeap* h) {
    if (!h || h->size <= 1) return; 

    qsort(h->heap, h->size, sizeof(AircraftCount), compare_aircraft_desc);
}