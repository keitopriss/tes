#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define UKURAN_BUFFER 5
#define JUMLAH_PRODUKSI 10

int buffer[UKURAN_BUFFER];
int masuk = 0;
int keluar = 0;

// Deklarasi mutex dan semaphore
pthread_mutex_t kunci;
sem_t kosong;
sem_t penuh;

// Fungsi untuk produsen
void *produsen(void *param) {
    for (int i = 0; i < JUMLAH_PRODUKSI; i++) {
        int item = rand() % 100;  // Data yang diproduksi

        sem_wait(&kosong);             // Tunggu jika buffer penuh
        pthread_mutex_lock(&kunci);    // Kunci akses ke buffer

        buffer[masuk] = item;
        printf("Produsen menghasilkan: %d\n", item);
        masuk = (masuk + 1) % UKURAN_BUFFER;

        pthread_mutex_unlock(&kunci);  // Lepas kunci
        sem_post(&penuh);              // Tambah jumlah item di buffer
    }
    pthread_exit(0);
}

// Fungsi untuk konsumen
void *konsumen(void *param) {
    for (int i = 0; i < JUMLAH_PRODUKSI; i++) {
        sem_wait(&penuh);              // Tunggu jika buffer kosong
        pthread_mutex_lock(&kunci);    // Kunci akses ke buffer

        int item = buffer[keluar];
        printf("Konsumen mengambil: %d\n", item);
        keluar = (keluar + 1) % UKURAN_BUFFER;

        pthread_mutex_unlock(&kunci);  // Lepas kunci
        sem_post(&kosong);             // Tambah slot kosong di buffer
    }
    pthread_exit(0);
}

int main() {
    pthread_t id_produsen, id_konsumen;

    // Inisialisasi mutex dan semaphore
    pthread_mutex_init(&kunci, NULL);
    sem_init(&kosong, 0, UKURAN_BUFFER);
    sem_init(&penuh, 0, 0);

    // Membuat thread produsen dan konsumen
    pthread_create(&id_produsen, NULL, produsen, NULL);
    pthread_create(&id_konsumen, NULL, konsumen, NULL);

    // Menunggu kedua thread selesai
    pthread_join(id_produsen, NULL);
    pthread_join(id_konsumen, NULL);

    // Menghancurkan mutex dan semaphore
    pthread_mutex_destroy(&kunci);
    sem_destroy(&kosong);
    sem_destroy(&penuh);

    return 0;
}
