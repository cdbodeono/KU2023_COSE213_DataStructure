#include <stdlib.h> // malloc

#include "adt_dlist.h"


/////
static int _insert(LIST *pList, NODE *pPre, void *dataInPtr){
    if (pList == NULL || dataInPtr == NULL) {
        return 0; 
    }
    
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    if (newNode == NULL) {
        return 0;  //overflow
    }
    
    newNode->dataPtr = dataInPtr;
    newNode->llink = pPre;
    newNode->rlink = pPre->rlink;
    
    if (pPre->rlink != NULL) {
        pPre->rlink->llink = newNode;
    }
    pPre->rlink = newNode;
    
    if (pList->rear == pPre) {
        pList->rear = newNode;
    }
    
    pList->count++;
    
    return 1; 
}

/////
static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr)
{
    if (pList == NULL || pPre == NULL || pLoc == NULL || dataOutPtr == NULL) {
        return; 
    }

    *dataOutPtr = pLoc->dataPtr;

    if (pPre != NULL) {
        pPre->rlink = pLoc->rlink;
    }

    if (pLoc->rlink != NULL) {
        pLoc->rlink->llink = pPre;
    }
    // 필요한 경우 리스트의 head 및 rear 포인터를 업데이트
    if (pLoc == pList->head) {
        pList->head = pLoc->rlink;
    }

    if (pLoc == pList->rear) {
        pList->rear = pPre;
    }

    // 삭제된 노드와 연결된 메모리 해제
    free(pLoc);

    // 리스트 카운트 감소
    pList->count--;
}
/////
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu)
{
    if (pList == NULL || pArgu == NULL || pPre == NULL || pLoc == NULL) {
        return 0; 
    }

    // 검색을 리스트의 head에서 시작
    *pPre = NULL;
    *pLoc = pList->head;

    while (*pLoc != NULL) {
        int cmp = pList->compare((*pLoc)->dataPtr, pArgu);

        if (cmp == 0) {
            return 1; // 대상 데이터를 찾음
        } else if (cmp > 0) {
            *pPre = *pLoc;
            *pLoc = (*pLoc)->rlink;
        } else {
            *pLoc = NULL;
            return 0; // 대상 데이터를 찾지 못함
        }
    }

    return 0; // 대상 데이터를 찾지 못함
}
/////
LIST *createList(int (*compare)(const void *, const void *)) {
    LIST *pList = (LIST *)malloc(sizeof(LIST));

    if (pList == NULL) {
        // Memory allocation failed, return NULL
        return NULL;
    }

    pList->count = 0; // Initialize count to zero
    pList->head = NULL;
    pList->rear = NULL;
    pList->compare = compare;

    return pList;
}

/////

void destroyList(LIST *pList, void (*callback)(void *))
{
    NODE *pPre = NULL;
    NODE *pLoc = NULL;

    pPre = pList -> head;

    while (pPre != NULL){
        pLoc = pPre -> rlink;
        (*callback)(pPre -> dataPtr);
        free(pPre);
        pPre = pLoc;
    }
    free(pList);
}
/////
int addNode(LIST *pList, void *dataInPtr)
{
    if (pList == NULL) {
        return 0; // Overflow
    }

    NODE *pPre = NULL;
    NODE *pLoc = pList->head;
    while (pLoc != NULL) {
        int cmp = pList->compare(pLoc->dataPtr, dataInPtr);
        if (cmp == 0) {
            return 2; // Duplicated key
        }
        else if (cmp > 0) {
            break;
        }
        pPre = pLoc;
        pLoc = pLoc->rlink;
    }

    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    if (newNode == NULL) {
        return 0; // Overflow
    }
    newNode->dataPtr = dataInPtr;

    if (pPre == NULL) {
        newNode->rlink = pList->head;
        newNode->llink = NULL;
        pList->head = newNode;

        if (pList->count == 0) {
            pList->rear = newNode;
        } else {
            pList->head->rlink->llink = newNode;
        }
    } else {
        pPre->rlink = newNode;
        newNode->llink = pPre;
        newNode->rlink = pLoc;

        if (pLoc == NULL) {
            pList->rear = newNode;
        } else {
            pLoc->llink = newNode;
        }
    }

    pList->count++;
    return 1; // Successful
}

/////

int removeNode(LIST *pList, void *keyPtr, void **dataOutPtr)
{
    if (pList == NULL || pList->count == 0) {
        return 0; // Not found
    }

    NODE *pPre = NULL;
    NODE *pLoc = pList->head;

    while (pLoc != NULL) {
        int cmp = pList->compare(pLoc->dataPtr, keyPtr);
        if (cmp == 0) {
            // Node found, remove it
            if (pPre == NULL) {
                pList->head = pLoc->rlink;
            } else {
                pPre->rlink = pLoc->rlink;
            }

            if (pLoc->rlink == NULL) {
                pList->rear = pPre;
            } else {
                pLoc->rlink->llink = pPre;
            }

            *dataOutPtr = pLoc->dataPtr;
            free(pLoc);
            pList->count--;
            return 1; // Deleted
        }
        pPre = pLoc;
        pLoc = pLoc->rlink;
    }

    return 0; // Not found
}
/////
int searchNode(LIST *pList, void *pArgu, void **dataOutPtr)
{
    if (pList == NULL || pList->count == 0) {
        return 0; // Not found
    }

    NODE *pLoc = pList->head;

    while (pLoc != NULL) {
        int cmp = pList->compare(pLoc->dataPtr, pArgu);
        if (cmp == 0) {
            *dataOutPtr = pLoc->dataPtr;
            return 1; // Successful
        }
        pLoc = pLoc->rlink;
    }

    return 0; // Not found
}
/////

int countList(LIST *pList)
{
    if (pList == NULL)
    {
        return 0; // 무효한 리스트 포인터
    }
    // 노드 수량 반환
    return pList->count;
}
/////
int emptyList(LIST *pList)
{
    if (pList == NULL)
    {
        return 1; // 무효한 리스트 포인터
    }
    return 1;
    return 0;
}
/////
void traverseList(LIST *pList, void (*callback)(const void *))
{
    if (pList == NULL)
    {
        return; // 무효한 리스트 포인터
    }

    NODE *current = pList->head;
    while (current != NULL)
    {
        if (callback != NULL)
        {
            callback(current->dataPtr);
        }
        current = current->rlink; 
    }
}
/////
void traverseListR(LIST *pList, void (*callback)(const void *))
{
    if (pList == NULL)
    {
        return; // 무효한 리스트 포인터
    }

    NODE *current = pList->rear;
    while (current != NULL)
    {
        if (callback != NULL)
        {
            callback(current->dataPtr);
        }
        current = current->llink; 
    }
}