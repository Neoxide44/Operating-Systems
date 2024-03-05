#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50
#define MAX_REFERENCES 100

/** Helper function that checks if a value is in an array
 * @param val - value that is searched for
 * @param array - array that is searched in
 * @param array_len - length of array
 * @return - '-1' if the value is not in the array, the index of the element
 * otherwise
 */
int search_array(int val, int *array, int array_len) {
    for (int i = 0; i < array_len; i++) {
        if (val == array[i] || -val == array[i])
            return i;
    }
    return -1;
}

/** Array that calculates number of page faults
 * @param num_frames - number of frames
 * @param num_ref - number of oage references
 * @param page_ref - array of page references
 * @return - number of page faults
 */
int get_page_faults(int num_frames, int num_ref, int *page_ref) {
    // Array for the clock elements
    int clock[num_frames];
    // Initialize the clock array to zeros
    for (int i = 0; i < num_frames; i++) {
        clock[i] = 0;
    }
    // Index for the "clock hand"
    int pointer = 0;
    // Self explanatory
    int page_faults = 0;

    for (int i = 0; i < num_ref; i++) {
        int check = search_array(page_ref[i], clock, num_frames);
        if (check == -1) {
            if (clock[pointer] < 0) {
                clock[pointer] *= -1;
                i--;
            } else {
                page_faults++;
                clock[pointer] = page_ref[i];
            }
            pointer++;
        } else if (page_ref[check] > 0) {
            clock[check] *= -1;
        }
        pointer %= num_frames;

        printf("page faults: %d\n Clock: ", page_faults);
        for (int i = 0; i < num_frames; i++) {
            printf("%d ", clock[i]);
        }
        printf("\n");
    }

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