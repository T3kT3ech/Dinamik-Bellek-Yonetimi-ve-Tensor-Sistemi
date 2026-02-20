# Dinamik-Bellek-Yonetimi-ve-Tensor-Sistemi
C dili öğrenme sürecimde, temel konulardan sonra işaretçiler (pointers), dinamik bellek yönetimi (malloc/free) ve veri yapıları (struct/union) gibi daha ileri seviye konuları pratik etmek için hazırladığım bir proje. Temel odak noktam, kısıtlı bellek ortamlarında veriyi en verimli şekilde saklamak ve kullanıcı hatalarını (yanlış veri girişi gibi) program çökmeden yönetebilmekti.

Projenin Amacı
Bu program, kullanıcıdan aldığı sayısal verileri inceliyor ve bellekte (RAM) boşuna yer kaplamamaları için en uygun veri tipini otomatik olarak seçiyor. Girilen sayı küçük bir tam sayıysa 1 byte (INT8), virgüllü ve büyükse 4 byte (FLOAT32) alan ayırarak bellek tasarrufu sağlıyor.

Temel Özellikler ve Uygulanan Konular
Hata Kontrolü (Input Validation): Kullanıcı sayı yerine harf veya özel karakter girdiğinde programın sonsuz döngüye girmesini veya çökmesini önlemek için arabellek (buffer) temizleme mantığı eklendi.

Dinamik Bellek Kullanımı: Dizilerin boyutu koda sabitlenmedi; kullanıcının belirttiği eleman sayısı kadar malloc ve calloc ile bellek ayrıldı ve işlem sonunda free ile temizlendi.

Struct ve Union Optimizasyonu: Farklı veri tiplerini düzenli tutmak için struct, bellekte aynı anda sadece ihtiyaç duyulan veri tipine yer ayırmak için union yapıları kullanıldı.

Kullanılan Teknolojiler
C Programlama Dili

Dinamik Bellek Yönetimi (<stdlib.h>)

Standart Giriş/Çıkış Kontrolleri (<stdio.h>)


Örnek Çıktı
--- Akilli ve Guvenli TinyML Tensor Sistemi ---

Tensor kac elemanli olacak?: 3

Lutfen 3 adet degeri girin (Orn: 5, -20, 3.14, 100000):
Eleman [0]: 12
Eleman [1]: a
[HATA] Lutfen sadece sayi giriniz! (Harf veya sembol kullanmayin)
Eleman [1]: -5
Eleman [2]: 100

[SISTEM ANALIZI] Veriler incelendi...
-> Karar: Degerler kucuk tam sayilar. Bellek tasarrufu icin INT8 (Kuantize) secildi.

--- Tensor Icerigi ve Bellek Durumu ---
Veri [0]: 12 (RAM'de 1 Byte)
Veri [1]: -5 (RAM'de 1 Byte)
Veri [2]: 100 (RAM'de 1 Byte)

Bellek (RAM) optimize edildi ve temizlendi. Islem tamam!
