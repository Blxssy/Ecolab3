/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include "IdEcoList1.h"
#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"

#include <stdio.h>
#include <stdint.h>
#include <time.h>

static int32_t* newArr(
    IEcoMemoryAllocator1* mem,
    uint32_t n,
    uint32_t seed
) {
    uint32_t i;
    uint32_t x = seed ? seed : 1u;
    int32_t* a = mem->pVTbl->Alloc(mem, n * sizeof(int32_t));
    if (!a) {
        return 0;
    }
    for (i = 0; i < n; ++i) {
        x = x * 1664525u + 1013904223u;
        a[i] = (int32_t)(x % 1000u);
    }
    return a;
}

static int checkSorted(const int32_t* a, uint32_t n) {
    uint32_t i;
    for (i = 0; i + 1 < n; ++i) {
        if (a[i] > a[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;

    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;

    IEcoLab1* pIEcoLab1_1 = 0;
    IEcoLab1* pIEcoLab1_2 = 0;
    IEcoLab1* pIEcoLab1_3 = 0;

    IEcoLab1Events* pIEcoLab1Sink1 = 0;
    IEcoLab1Events* pIEcoLab1Sink2 = 0;

    IEcoConnectionPointContainer* pICPC1 = 0;
    IEcoConnectionPointContainer* pICPC2 = 0;
    IEcoConnectionPointContainer* pICPC3 = 0;

    IEcoConnectionPoint* pICP1 = 0;
    IEcoConnectionPoint* pICP2 = 0;
    IEcoConnectionPoint* pICP3 = 0;

    uint32_t cAdvise1_1 = 0;
    uint32_t cAdvise1_2 = 0;
    uint32_t cAdvise2_1 = 0;
    uint32_t cAdvise3_1 = 0;

    uint32_t arrayLengthSmall = 5;
    uint32_t arrayLengthMedium = 1000;
    uint32_t seed = 123;

    int32_t* array1 = 0;
    int32_t* array2 = 0;
    int32_t* array3 = 0;

    clock_t start;
    clock_t end;
    double workingTime;
    int sortedCorrectly;
    uint32_t i;

    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void**)&pISys);
    if (result != 0 || pISys == 0) {
        goto Release;
    }

    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }

    result = pIBus->pVTbl->QueryComponent(
        pIBus,
        &CID_EcoMemoryManager1,
        0,
        &IID_IEcoMemoryAllocator1,
        (void**)&pIMem
    );
    if (result != 0 || pIMem == 0) {
        goto Release;
    }

    result = pIBus->pVTbl->QueryComponent(
        pIBus,
        &CID_EcoLab1,
        0,
        &IID_IEcoLab1,
        (void**)&pIEcoLab1_1
    );
    if (result != 0 || pIEcoLab1_1 == 0) {
        goto Release;
    }

    result = pIBus->pVTbl->QueryComponent(
        pIBus,
        &CID_EcoLab1,
        0,
        &IID_IEcoLab1,
        (void**)&pIEcoLab1_2
    );
    if (result != 0 || pIEcoLab1_2 == 0) {
        goto Release;
    }

    result = pIBus->pVTbl->QueryComponent(
        pIBus,
        &CID_EcoLab1,
        0,
        &IID_IEcoLab1,
        (void**)&pIEcoLab1_3
    );
    if (result != 0 || pIEcoLab1_3 == 0) {
        goto Release;
    }

    result = createCEcoLab1Sink(pIMem, &pIEcoLab1Sink1);
    if (result != 0 || pIEcoLab1Sink1 == 0) {
        goto Release;
    }

    result = createCEcoLab1Sink(pIMem, &pIEcoLab1Sink2);
    if (result != 0 || pIEcoLab1Sink2 == 0) {
        goto Release;
    }

    printf("Component 1 with single event handler\n");
    printf("\nTest 1: Sorting with single event handler\n");

    result = pIEcoLab1_1->pVTbl->QueryInterface(
        pIEcoLab1_1,
        &IID_IEcoConnectionPointContainer,
        (void**)&pICPC1
    );
    if (result == 0 && pICPC1 != 0) {
        result = pICPC1->pVTbl->FindConnectionPoint(
            pICPC1,
            &IID_IEcoLab1Events,
            &pICP1
        );
        if (result == 0 && pICP1 != 0) {
            pICP1->pVTbl->Advise(
                pICP1,
                (IEcoUnknown*)pIEcoLab1Sink1,
                &cAdvise1_1
            );
            printf("Sink 1 connected to Component 1\n");
        }
    }

        array1 = newArr(pIMem, arrayLengthSmall, seed);
    if (array1) {
        start = clock();
        pIEcoLab1_1->pVTbl->BubbleSort(
            pIEcoLab1_1,
            array1,
            (int16_t)arrayLengthSmall
        );
        end = clock();

        workingTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time: %f seconds\n", workingTime);
    }

    printf("\nComponent 2 with multiple event handlers\n");
    printf("Test 2: Sorting with multiple event handlers\n");

    result = pIEcoLab1_2->pVTbl->QueryInterface(
        pIEcoLab1_2,
        &IID_IEcoConnectionPointContainer,
        (void**)&pICPC2
    );
    if (result == 0 && pICPC2 != 0) {
        result = pICPC2->pVTbl->FindConnectionPoint(
            pICPC2,
            &IID_IEcoLab1Events,
            &pICP2
        );
        if (result == 0 && pICP2 != 0) {
            pICP2->pVTbl->Advise(
                pICP2,
                (IEcoUnknown*)pIEcoLab1Sink1,
                &cAdvise2_1
            );
            printf("Sink 1 connected to Component 2\n");

            pICP2->pVTbl->Advise(
                pICP2,
                (IEcoUnknown*)pIEcoLab1Sink2,
                &cAdvise2_2
            );
            printf("Sink 2 connected to Component 2\n");
        }
    }

    array2 = newArr(pIMem, arrayLengthSmall, seed + 100);
    if (array2) {
        start = clock();
        pIEcoLab1_2->pVTbl->BubbleSort(
            pIEcoLab1_2,
            array2,
            (int16_t)arrayLengthSmall
        );
        end = clock();
        workingTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time: %f seconds\n", workingTime);
    }

    printf("\nComponent 3 using shared event handler\n");
    printf("Test 3: Shared event handler for multiple components\n");

    result = pIEcoLab1_3->pVTbl->QueryInterface(
        pIEcoLab1_3,
        &IID_IEcoConnectionPointContainer,
        (void**)&pICPC3
    );
    if (result == 0 && pICPC3 != 0) {
        result = pICPC3->pVTbl->FindConnectionPoint(
            pICPC3,
            &IID_IEcoLab1Events,
            &pICP3
        );
        if (result == 0 && pICP3 != 0) {
            pICP3->pVTbl->Advise(
                pICP3,
                (IEcoUnknown*)pIEcoLab1Sink1,
                &cAdvise3_1
            );
            printf("Sink 1 connected to Component 3\n");
        }
    }

    array3 = genArray(pIMem, arrayLengthSmall, seed + 200);
    if (array3) {
        start = clock();
        pIEcoLab1_3->pVTbl->BubbleSort(
            pIEcoLab1_3,
            array3,
            (int16_t)arrayLengthSmall
        );
        end = clock();
        workingTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time: %f seconds\n", workingTime);
    }

Release:
    if (pICP1) pICP1->pVTbl->Unadvise(pICP1, cAdvise1_1);
    if (pICP1) pICP1->pVTbl->Unadvise(pICP1, cAdvise1_2);
    if (pICP2) pICP2->pVTbl->Unadvise(pICP2, cAdvise2_1);
    if (pICP3) pICP3->pVTbl->Unadvise(pICP3, cAdvise3_1);

    if (pICP1) pICP1->pVTbl->Release(pICP1);
    if (pICP2) pICP2->pVTbl->Release(pICP2);
    if (pICP3) pICP3->pVTbl->Release(pICP3);

    if (pICPC1) pICPC1->pVTbl->Release(pICPC1);
    if (pICPC2) pICPC2->pVTbl->Release(pICPC2);
    if (pICPC3) pICPC3->pVTbl->Release(pICPC3);

    if (pIEcoLab1Sink1) pIEcoLab1Sink1->pVTbl->Release(pIEcoLab1Sink1);
    if (pIEcoLab1Sink2) pIEcoLab1Sink2->pVTbl->Release(pIEcoLab1Sink2);

    if (pIEcoLab1_1) pIEcoLab1_1->pVTbl->Release(pIEcoLab1_1);
    if (pIEcoLab1_2) pIEcoLab1_2->pVTbl->Release(pIEcoLab1_2);
    if (pIEcoLab1_3) pIEcoLab1_3->pVTbl->Release(pIEcoLab1_3);

    if (array1) pIMem->pVTbl->Free(pIMem, array1);
    if (array2) pIMem->pVTbl->Free(pIMem, array2);
    if (array3) pIMem->pVTbl->Free(pIMem, array3);

    if (pIMem) pIMem->pVTbl->Release(pIMem);
    if (pIBus) pIBus->pVTbl->Release(pIBus);
    if (pISys) pISys->pVTbl->Release(pISys);

    return result;
}
