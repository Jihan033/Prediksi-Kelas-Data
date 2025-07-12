#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define nMax 10

typedef struct {
    int UTS;
    int TUGAS;
    int UAS;
    char KELAS;
} DataNilai;

typedef struct {
    DataNilai Info[nMax+1];
    int neff;
} TabData;

typedef struct {
    double distance;
    char kelas;
} Neighbor;

void CreateTabData(TabData *P) {
    (*P).neff = 0;
}

bool IsEmpty(TabData P) {
    return (P.neff == 0);
}

bool IsFull(TabData P) {
    return (P.neff == nMax);
}

void AddData(TabData *P, int uts, int tugas, int uas, char kelas) {
    if (!IsFull(*P)) {
        (*P).neff++;
        (*P).Info[(*P).neff].UTS = uts;
        (*P).Info[(*P).neff].TUGAS = tugas;
        (*P).Info[(*P).neff].UAS = uas;
        (*P).Info[(*P).neff].KELAS = kelas;
    } else {
        printf("Tabel Data Sudah Memenuhi Kapasitas!\n");
    }
}

void PrintData(TabData P) {
    int i;
    printf(" ");
    for (i = 0; i < 32; i++) printf("_");
    printf("\n");

    printf("| %-2s | %-3s | %-5s | %-3s | %-5s |\n", "NO", "UTS", "TUGAS", "UAS", "KELAS");
    printf("|----|-----|-------|-----|-------|\n");

    for (i = 1; i <= P.neff; i++) {
        printf("| %-2d | %-3d | %-5d | %-3d | %-5c |\n", i, P.Info[i].UTS, P.Info[i].TUGAS, P.Info[i].UAS, P.Info[i].KELAS);
    }

    printf(" ");
    for (i = 0; i < 32; i++) printf("-");
    printf("\n");
}

int compare(const void *a, const void *b) {
    Neighbor *neighborA = (Neighbor *)a;
    Neighbor *neighborB = (Neighbor *)b;
    return (neighborA->distance > neighborB->distance) - (neighborA->distance < neighborB->distance);
}

void PrediksiData(TabData P, int uts, int tugas, int uas, int k) {
    Neighbor neighbors[nMax+1];
    int i;
    char kelas;
    int countA = 0, countB = 0, countC = 0;

    for (i = 1; i <= P.neff; i++) {
        double distance = sqrt(
            pow(P.Info[i].UTS - uts, 2) + 
            pow(P.Info[i].TUGAS - tugas, 2) + 
            pow(P.Info[i].UAS - uas, 2)
        );
        neighbors[i].distance = distance;
        neighbors[i].kelas = P.Info[i].KELAS;
    }

    qsort(neighbors + 1, P.neff, sizeof(Neighbor), compare);

    for (i = 1; i <= k; i++) {
        if (neighbors[i].kelas == 'A') {
            countA++;
        } else if (neighbors[i].kelas == 'B') {
            countB++;
        } else if (neighbors[i].kelas == 'C') {
            countC++;
        }
    }

    if (countA >= countB && countA >= countC) {
        kelas = 'A';
    } else if (countB >= countA && countB >= countC) {
        kelas = 'B';
    } else if (countC >= countA && countC >= countB){
        kelas = 'C';
    }

    printf("Kelas untuk data baru dengan nilai UTS=%d, TUGAS=%d, UAS=%d adalah: %c\n", uts, tugas, uas, kelas);
}

int main() {
    TabData MyTabData;
    int uts, tugas, uas, n, i, k, pilihan;
    char kelas;

    CreateTabData(&MyTabData);

    do {
        printf("\nMenu:\n");
        printf("1. Input Data\n");
        printf("2. Tampilkan Data\n");
        printf("3. Prediksi Nilai Baru\n");
        printf("4. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                printf("Masukkan jumlah data yang ingin diinput [tidak lebih dari 10]: ");
                scanf("%d", &n);
                if (n > nMax) {
                    printf("MASUKAN TIDAK VALID!\n");
                } else {
                    for (i = 1; i <= n; i++) {
                        printf("Masukkan data ke-%d\n", i);
                        printf("Masukan Nilai UTS: ");
                        scanf("%d", &uts);
                        printf("Masukan Nilai TUGAS: ");
                        scanf("%d", &tugas);
                        printf("Masukan Nilai UAS: ");
                        scanf("%d", &uas);
                        printf("Masukan Kelas[A, B, C]: ");
                        scanf(" %c", &kelas);  // Spasi sebelum %c untuk mengabaikan whitespace
                        AddData(&MyTabData, uts, tugas, uas, kelas);
                    }
                }
                break;
            case 2:
                if (IsEmpty(MyTabData)) {
                    printf("Data Kosong!\n");
                } else {
                    PrintData(MyTabData);
                }
                break;
            case 3:
                if (IsEmpty(MyTabData)) {
                    printf("Data Kosong! Tidak dapat melakukan prediksi.\n");
                } else {
                    printf("Masukkan nilai data baru untuk diprediksi kelasnya:\n");
                    printf("Nilai UTS: ");
                    scanf("%d", &uts);
                    printf("Nilai TUGAS: ");
                    scanf("%d", &tugas);
                    printf("Nilai UAS: ");
                    scanf("%d", &uas);
                    printf("Masukkan nilai k (jumlah tetangga terdekat): ");
                    scanf("%d", &k);
                    if (k > MyTabData.neff) {
                        printf("Nilai k tidak valid! Tidak boleh lebih dari jumlah data yang ada.\n");
                    } else {
                        PrediksiData(MyTabData, uts, tugas, uas, k);
                    }
                }
                break;
            case 4:
                printf("Terima kasih! Program berakhir.\n");
                break;
            default:
                printf("Pilihan tidak valid! Silakan coba lagi.\n");
        }
    } while (pilihan != 4);

    return 0;
}
