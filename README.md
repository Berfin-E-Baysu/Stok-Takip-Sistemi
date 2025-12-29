 # Stok Takip Sistemi (C)

Bu proje, **C programlama dili** kullanılarak geliştirilmiş basit bir **stok takip sistemi**dir.  
Ürün ekleme, stok listeleme, satış yapma ve toplam kazancı takip etme işlemlerini destekler.  
Veriler dosyaya kaydedilir ve program yeniden çalıştırıldığında otomatik olarak yüklenir.

---

## Özellikler

- Ürün ekleme (dinamik bellek kullanımı)
- Stok listeleme
- Satış yapma ve stok düşürme
- Toplam kazanç hesaplama
- Dosyaya kaydetme ve dosyadan okuma
- Hatalı girişler için kontrol mekanizmaları

---

## Kullanılan Konular

- `struct` (Yapılar)
- Dinamik bellek yönetimi (`malloc`, `realloc`, `free`)
- Pointer kullanımı
- Dosya işlemleri (`fopen`, `fprintf`, `fscanf`, `fclose`)
- Fonksiyonlar
- Döngüler ve koşul yapıları
- Menü tabanlı programlama

---

## Nasıl Çalıştırılır?

```bash
gcc stok_takip.c -o stok
./stok
