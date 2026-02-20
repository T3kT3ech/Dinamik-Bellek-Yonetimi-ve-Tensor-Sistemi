#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* --- 1. VERÝ TÝPLERÝ (ENUM) --- */
typedef enum {
    TENSOR_FLOAT32,
    TENSOR_FLOAT16,
    TENSOR_INT8
} TensorDataType;

/* --- 2. VERÝ ALANI (UNION) --- */
typedef union {
    float *f32_data;
    uint16_t *f16_data; 
    int8_t *i8_data;    
} TensorData;

/* --- 3. ANA TENSÖR YAPISI (STRUCT) --- */
typedef struct {
    TensorDataType type; 
    size_t size;         
    TensorData data;     
} Tensor;

/* --- 4. YARDIMCI FONKSÝYONLAR --- */

/* Klavyeden girilen hatali (harf, ozel karakter) verileri hafizadan temizler */
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Tensor* create_tensor(TensorDataType type, size_t size) {
    Tensor *t = (Tensor*)malloc(sizeof(Tensor));
    if (t == NULL) return NULL; 

    t->type = type;
    t->size = size;

    switch (type) {
        case TENSOR_FLOAT32:
            t->data.f32_data = (float*)calloc(size, sizeof(float));
            break;
        case TENSOR_FLOAT16:
            t->data.f16_data = (uint16_t*)calloc(size, sizeof(uint16_t));
            break;
        case TENSOR_INT8:
            t->data.i8_data = (int8_t*)calloc(size, sizeof(int8_t));
            break;
    }
    return t;
}

void free_tensor(Tensor *t) {
    if (t != NULL) {
        free(t->data.f32_data); 
        free(t); 
    }
}

/* --- TÝP KARAR VERÝCÝ ALGORÝTMA --- */
TensorDataType auto_determine_type(float *temp_array, size_t size) {
    size_t i;
    int is_int8_possible = 1;     
    int is_float16_possible = 1;  

    for (i = 0; i < size; i++) {
        float val = temp_array[i];

        if (val != (int)val || val < -128 || val > 127) {
            is_int8_possible = 0;
        }

        if (val < -65500.0f || val > 65500.0f) {
            is_float16_possible = 0;
        }
    }

    if (is_int8_possible) {
        return TENSOR_INT8;
    } else if (is_float16_possible) {
        return TENSOR_FLOAT16;
    } else {
        return TENSOR_FLOAT32;
    }
}


/* --- 5. ANA PROGRAM --- */
int main() {
    size_t size = 0, i;
    float *temp_inputs;
    TensorDataType decided_type;
    Tensor *my_tensor = NULL;

    printf("--- Akilli ve Guvenli TinyML Tensor Sistemi ---\n\n");

    /* 1. Aþama: Boyutu alýrken HATA KONTROLÜ */
    while (1) {
        printf("Tensor kac elemanli olacak?: ");
        /* scanf 1 döndürürse (baþarýlý okuma) ve sayý 0'dan büyükse döngüden çýk */
        if (scanf("%zu", &size) == 1 && size > 0) {
            clear_buffer(); /* Baþarýlý okumadan sonra 'Enter' tuþunu temizle */
            break; 
        } else {
            printf("[HATA] Gecersiz giris! Lutfen 0'dan buyuk bir SAYI giriniz.\n\n");
            clear_buffer(); /* Yanlýþ girilen harf veya sembolleri temizle */
        }
    }

    temp_inputs = (float*)malloc(size * sizeof(float));
    if (temp_inputs == NULL) {
        printf("Hata: Gecici dizi icin bellek ayrilamadi!\n");
        return 1;
    }

    /* 2. Aþama: Kullanýcýdan deðerleri alýrken HATA KONTROLÜ */
    printf("\nLutfen %zu adet degeri girin (Orn: 5, -20, 3.14, 100000):\n", size);
    for (i = 0; i < size; i++) {
        while (1) {
            printf("Eleman [%zu]: ", i);
            /* scanf float bir sayý okuyabildi mi kontrol et */
            if (scanf("%f", &temp_inputs[i]) == 1) {
                clear_buffer();
                break; /* Geçerli sayý girildi, bu eleman için döngüden çýk */
            } else {
                printf("[HATA] Lutfen sadece sayi giriniz! (Harf veya sembol kullanmayin)\n");
                clear_buffer(); /* Yanlýþ veriyi temizle ve ayný elemaný tekrar sor */
            }
        }
    }

    /* 3. Aþama: Karar ve Atama iþlemleri (Ayný kalýyor) */
    decided_type = auto_determine_type(temp_inputs, size);

    printf("\n[SISTEM ANALIZI] Veriler incelendi...\n");
    if (decided_type == TENSOR_INT8) {
        printf("-> Karar: Degerler kucuk tam sayilar. Bellek tasarrufu icin INT8 (Kuantize) secildi.\n");
    } else if (decided_type == TENSOR_FLOAT16) {
        printf("-> Karar: Degerler kusuratli ama cok buyuk degil. FLOAT16 (Yari Hassasiyet) secildi.\n");
    } else {
        printf("-> Karar: Degerler cok buyuk veya yuksek hassasiyet gerektiriyor. FLOAT32 secildi.\n");
    }

    my_tensor = create_tensor(decided_type, size);

    printf("\n--- Tensor Icerigi ve Bellek Durumu ---\n");
    for (i = 0; i < size; i++) {
        if (decided_type == TENSOR_INT8) {
            my_tensor->data.i8_data[i] = (int8_t)temp_inputs[i];
            printf("Veri [%zu]: %d (RAM'de 1 Byte)\n", i, my_tensor->data.i8_data[i]);
            
        } else if (decided_type == TENSOR_FLOAT16) {
            my_tensor->data.f16_data[i] = (uint16_t)temp_inputs[i];
            printf("Veri [%zu]: %hu (RAM'de 2 Byte - FP16 Simule edildi)\n", i, my_tensor->data.f16_data[i]);
            
        } else {
            my_tensor->data.f32_data[i] = temp_inputs[i];
            printf("Veri [%zu]: %.4f (RAM'de 4 Byte)\n", i, my_tensor->data.f32_data[i]);
        }
    }

    /* Temizlik */
    free(temp_inputs);
    free_tensor(my_tensor);
    
    printf("\nBellek (RAM) optimize edildi ve temizlendi. Islem tamam!\n");

    return 0;
}
