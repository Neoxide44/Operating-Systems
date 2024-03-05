#include <stdio.h>

#include "LibQueue.h"

#define MAX_FRAMES 10
#define MAX_PAGES 50
#define MAX_REFERENCES 100

/** Array that calculates number of page faults
 * @param num_frames - number of frames
 * @param num_ref - number of oage references
 * @param page_ref - array of page references
 * @return - number of page faults
 */
int get_page_faults(int num_frames, int num_ref, int *page_ref) {
    int page_faults = 0;
    // Current number of items in the queue
    int num_items = 0;
    Queue queue = newQueue(num_frames);
    // enqueue(420, &queue);
    for (int i = 0; i < num_ref; i++) {
        if (!searchQueue(page_ref[i], queue)) {
            if (num_items >= num_frames) {
                dequeue(&queue);
                num_items--;
            }
            enqueue(page_ref[i], &queue);
            page_faults++;
            num_items++;
        }
    }

    freeQueue(queue);

    return page_faults;
}

int main() {
    int num_frames, num_ref;
    int page_ref[MAX_REFERENCES];

    // Input number of frames
    scanf("%d", &num_frames);

    // Input page references
    for (int i = 0; i < MAX_REFERENCES; i++) {
        if (scanf("%d", &page_ref[i]) == EOF) {
            num_ref = i;
            break;
        }
    }

    int page_faults = get_page_faults(num_frames, num_ref, page_ref);
    printf("%d\n", page_faults);
    return 0;
}