#include <stdio.h>
#include <stdlib.h>

struct node {
    int id;
    int start;
    int end;
    struct node* link;
}* block_ptr = NULL;

typedef struct node block_type;

int pm_size;
int remaining;

void enterParameters() {
    printf("Enter size of physical memory: ");
    scanf("%d", &pm_size);
    
    remaining = pm_size;
    block_ptr = (block_type *)malloc(sizeof(block_type));
    block_ptr -> id = -1;
    block_ptr -> start = 0;
    block_ptr -> end = 0;
    block_ptr -> link = NULL;
    return;
}

void output() {
    block_type* current_ptr = block_ptr->link;
    
    printf("\nID\tStart\tEnd");
    printf("\n-------------------\n");
    while (current_ptr != NULL) {
        printf("%d\t%d\t%d\n", current_ptr -> id, current_ptr -> start, current_ptr -> end);
        current_ptr = current_ptr -> link;
    }
    return;
}

void firstFit() {
    int block_id;
    int block_size;
    block_type* current_ptr = block_ptr;
    block_type* new_block_ptr;
    int hole_start, hole_end, hole_size;
    
    printf("Enter block id: ");
    scanf("%d", &block_id);
    
    printf("Enter block size: ");
    scanf("%d", &block_size);
    
    if (block_size > remaining) {
        printf("Not enough memory...");
        return;
    }

    new_block_ptr = (block_type *)malloc(sizeof(block_type));
    new_block_ptr -> id = block_id;

    if (block_ptr -> link == NULL) {
        new_block_ptr -> start = block_ptr -> end;
        new_block_ptr -> end = new_block_ptr -> start + block_size;
        new_block_ptr -> link = NULL;
        block_ptr -> link = new_block_ptr;
        remaining -= block_size;
        return;
    }
    
    while (current_ptr != NULL) {
        if (block_id == current_ptr -> id) {
            printf("Duplicate ID...");
            free(new_block_ptr);
            return;
        }
        current_ptr = current_ptr -> link;
    }
    current_ptr = block_ptr;

    while (current_ptr != NULL) {
        hole_start = current_ptr -> end;
        if (current_ptr -> link != NULL) {
            hole_end = current_ptr -> link -> start;
        } else {
            hole_end = pm_size;
        }
        hole_size = hole_end - hole_start;
        
        if (block_size <= hole_size) {
            new_block_ptr -> start = hole_start;
            new_block_ptr -> end = hole_start + block_size;
            new_block_ptr -> link = current_ptr -> link;
            current_ptr -> link = new_block_ptr;
            remaining -= block_size;
            return;
        }
        current_ptr = current_ptr -> link;
    }
    printf("\nThere is no fitting hole\n");
    return;
}

void bestFit() {
    int block_id;
    int block_size;
    block_type* current_ptr = block_ptr;
    block_type* new_block_ptr;
    block_type* best_block_ptr;
    int hole_start; 
    int hole_end; 
    int hole_size;
    int at_least_one = 0;
    int best_so_far;
    best_so_far = pm_size;
    int best_start;

    printf("Enter block id: ");
    scanf("%d", &block_id);
    
    printf("Enter block size: ");
    scanf("%d", &block_size);
    
    if (block_size > remaining) {
        printf("Not enough memory...");
        return;
    }
    
    new_block_ptr = (block_type *)malloc(sizeof(block_type));
    new_block_ptr -> id = block_id;
    
    if (block_ptr -> link == NULL) {
        new_block_ptr -> start = block_ptr -> end;
        new_block_ptr -> end = new_block_ptr -> start + block_size;
        new_block_ptr -> link = NULL;
        block_ptr -> link = new_block_ptr;
        remaining -= block_size;
        return;
    }
    
    while (current_ptr != NULL) {
        if (block_id == current_ptr -> id) {
            printf("Duplicate ID...");
            free(new_block_ptr);
            return;
        }
        current_ptr = current_ptr -> link;
    }
    current_ptr = block_ptr;
    
    while (current_ptr != NULL) {
        hole_start = current_ptr -> end;
        if (current_ptr -> link != NULL) {
            hole_end = current_ptr -> link -> start;
        } else {
            hole_end = pm_size;
        }
        hole_size = hole_end - hole_start;
        
        if (block_size <= hole_size) {
            at_least_one = 1;
            if (hole_size < best_so_far) {
                best_so_far = hole_size;
                best_start = hole_start;
                best_block_ptr = current_ptr;
            }
        }
        current_ptr = current_ptr -> link;
    }
    
    if (at_least_one == 0) {
        printf("There is no fitting hole");
        free(new_block_ptr);
        free(best_block_ptr);
        return;
    }

    new_block_ptr -> start = best_start;
    new_block_ptr -> end = new_block_ptr -> start + block_size;
    new_block_ptr -> link = best_block_ptr -> link;
    best_block_ptr -> link = new_block_ptr;
    remaining -= block_size;
    return;
}

void deallocate() {
    block_type* current_ptr = block_ptr;
    block_type* previous_ptr;
    int block_size,block_id;

    printf("Enter block id: ");
    scanf("%d", &block_id);
    
    while ((current_ptr != NULL) && (block_id != current_ptr -> id)) {
        previous_ptr = current_ptr;
        current_ptr = current_ptr -> link;
    }

    if (current_ptr == NULL) {
        printf("ID not found...");
        return;
    }

    previous_ptr -> link = current_ptr -> link;
    block_size = current_ptr -> end - current_ptr -> start;
    remaining += block_size;
    free(current_ptr);
    return;
}

void defragment() {
    block_type* current_ptr = block_ptr;
    int block_size;
    int prev_block_end = block_ptr -> end;
    
    while (current_ptr != NULL) {
        block_size = current_ptr -> end - current_ptr -> start;
        current_ptr -> start = prev_block_end;
        current_ptr -> end = current_ptr -> start + block_size;
        prev_block_end = current_ptr -> end;
        current_ptr = current_ptr -> link;
    }
    return;
}

int main() {
    int option;
    
    do {
        printf("\nHole-Fitting Algorithms");
        printf("\n-----------------------");
        printf("\n1) Enter parameters ");
        printf("\n2) Allocate memory for block using First-fit");
        printf("\n3) Allocate memory for block using Best-fit");
        printf("\n4) Deallocate memory for block");
        printf("\n5) Defragment memory");
        printf("\n6) Quit program");
        printf("\n\nEnter selection: ");
        scanf("%d", &option);
        
        switch (option) {
            case 1:
                enterParameters();
                break;
            
            case 2:
                firstFit();
                output();
                break;
            
            case 3:
                bestFit();
                output();
                break;
            
            case 4:
                deallocate();
                output();
                break;
            
            case 5:
                defragment();
                output();
                break;
            
            case 6:
                 printf("Quitting program...");
                break;
            
            default:
                printf("\nNot a valid option");
        }
    }
    while (option != 6);
    return 0;
}
