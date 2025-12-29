#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Ürün bilgilerini tutan yapı
typedef struct {
    char urunAdi[30];    // Ürün adı
    int stok;            // Stok miktarı
    float fiyat;         // Ürün fiyatı
} Urun;

// Fonksiyon prototipleri
void urunEkle(Urun **urunler, int *urunSayisi, int *kapasite);
void stokListele(Urun *urunler, int urunSayisi);
void satisYap(Urun *urunler, int urunSayisi, float *kazanc);
void dosyadanOku(Urun **urunler, int *urunSayisi, int *kapasite, float *kazanc);
void dosyayaYaz(Urun *urunler, int urunSayisi, float kazanc);

int main() {
    int kapasite = 2;        // Başlangıçta tutulabilecek maksimum ürün sayısı
    int urunSayisi = 0;     // Sistemdeki mevcut ürün sayısı
    float kazanc = 0;       // Toplam satış kazancı
    int secim;

    // Ürünler için dinamik bellek ayırma
    Urun *urunler = (Urun *)malloc(kapasite * sizeof(Urun));

    // Program açılırken önceki verileri dosyadan okur
    dosyadanOku(&urunler, &urunSayisi, &kapasite, &kazanc);

    while (1) { // Sonsuz döngü (menü sürekli gösterilir)
        printf("\n--- STOK TAKIP SISTEMI ---\n");
        printf("1- Urun Ekle\n2- Stok Listele\n3- Satis Yap\n4- Toplam Kazanc\n5- Cikis\n");
        printf("Seciminiz: ");

        // Kullanıcı sayı dışında giriş yaparsa kontrol edilir
        if (scanf("%d", &secim) != 1) {
            printf("Lutfen gecerli bir rakam giriniz!\n");
            while (getchar() != '\n'); // Giriş tamponunu temizler
            continue;                  // Döngünün başına döner
        }

        // Çıkış seçilirse dosyaya yazıp programdan çıkar
        if (secim == 5) {
            dosyayaYaz(urunler, urunSayisi, kazanc);
            printf("Programdan cikiliyor...\n");
            break; // Döngüyü sonlandırır
        }

        // Menü seçimine göre ilgili işlemi çağırır
        if (secim == 1)
            urunEkle(&urunler, &urunSayisi, &kapasite);
        else if (secim == 2)
            stokListele(urunler, urunSayisi);
        else if (secim == 3)
            satisYap(urunler, urunSayisi, &kazanc);
        else if (secim == 4)
            printf("Toplam kazanc: %.2f TL\n", kazanc);
        else
            printf("Hatali secim!\n");
    }

    // Program sonunda ayrılan belleği serbest bırakır
    free(urunler);
    return 0;
}

void urunEkle(Urun **urunler, int *urunSayisi, int *kapasite) {

    // Kapasite dolduysa bellek alanını genişletir
    if (*urunSayisi >= *kapasite) {
        *kapasite += 2; // Kapasiteyi artırır
        *urunler = (Urun *)realloc(*urunler, (*kapasite) * sizeof(Urun));
    }

    printf("Urun adi: ");
    scanf("%s", (*urunler)[*urunSayisi].urunAdi); // Ürün adını alır

    // Stok miktarı negatif girilirse tekrar sorar
    do {
        printf("Stok miktari (En az 0): ");
        scanf("%d", &((*urunler)[*urunSayisi].stok));
    } while ((*urunler)[*urunSayisi].stok < 0);

    printf("Fiyat: ");
    scanf("%f", &((*urunler)[*urunSayisi].fiyat)); // Ürün fiyatını alır

    (*urunSayisi)++; // Ürün sayısını artırır
}

void stokListele(Urun *urunler, int urunSayisi) {

    // Hiç ürün yoksa kullanıcıyı bilgilendirir
    if (urunSayisi == 0) {
        printf("Stokta urun yok.\n");
        return;
    }

    // Tüm ürünleri listeleyen for döngüsü
    for (int i = 0; i < urunSayisi; i++) {
        printf("%d- %s | Stok: %d | Fiyat: %.2f TL\n",
               i + 1,
               urunler[i].urunAdi,
               urunler[i].stok,
               urunler[i].fiyat);
    }
}

void satisYap(Urun *urunler, int urunSayisi, float *kazanc) {
    int no, miktar;

    // Satılacak ürün yoksa işlem yapılmaz
    if (urunSayisi == 0) {
        printf("Satilacak urun yok.\n");
        return;
    }

    stokListele(urunler, urunSayisi); // Ürünleri kullanıcıya gösterir

    printf("Satis yapilacak urun no: ");
    scanf("%d", &no);

    // Girilen ürün numarası geçerli mi kontrol eder
    if (no < 1 || no > urunSayisi) {
        printf("Hatali urun no!\n");
        return;
    }

    printf("Satis miktari: ");
    scanf("%d", &miktar);

    // Yeterli stok varsa satış işlemi yapılır
    if (urunler[no - 1].stok >= miktar) {
        urunler[no - 1].stok -= miktar;                // Stoktan düşer
        *kazanc += miktar * urunler[no - 1].fiyat;     // Kazancı günceller
        printf("Satis tamamlandi.\n");
    } else {
        printf("Yetersiz stok!\n");
    }
}

void dosyadanOku(Urun **urunler, int *urunSayisi, int *kapasite, float *kazanc) {

    // Dosya okuma modunda açılır
    FILE *fp = fopen("stok.txt", "r");
    if (fp == NULL) return; // Dosya yoksa işlem yapılmaz

    // Dosyadaki toplam kazancı okur
    if (fscanf(fp, "%f", kazanc) == EOF) {
        fclose(fp);
        return;
    }

    // Dosyadaki tüm ürünleri okuyup diziye ekler
    while (fscanf(fp, "%s %d %f",
           (*urunler)[*urunSayisi].urunAdi,
           &((*urunler)[*urunSayisi].stok),
           &((*urunler)[*urunSayisi].fiyat)) != EOF) {

        (*urunSayisi)++; // Okunan ürün sayısını artırır

        // Kapasite dolduysa belleği genişletir
        if (*urunSayisi >= *kapasite) {
            *kapasite += 2;
            *urunler = (Urun *)realloc(*urunler, (*kapasite) * sizeof(Urun));
        }
    }

    fclose(fp); // Dosyayı kapatır
}

void dosyayaYaz(Urun *urunler, int urunSayisi, float kazanc) {

    // Dosya yazma modunda açılır
    FILE *fp = fopen("stok.txt", "w");
    if (fp == NULL) return;

    // Toplam kazancı dosyaya yazar
    fprintf(fp, "%.2f\n", kazanc);

    // Tüm ürünleri dosyaya kaydeder
    for (int i = 0; i < urunSayisi; i++) {
        fprintf(fp, "%s %d %.2f\n",
                urunler[i].urunAdi,
                urunler[i].stok,
                urunler[i].fiyat);
    }

    fclose(fp); // Dosyayı kapatır
}
