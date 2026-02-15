#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define MAX_PAGE 999

// ANSI Color Codes
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// Function declarations
int fifo(int pages[], int n, int frames);
int lru(int pages[], int n, int frames);
int optimal(int pages[], int n, int frames);
void init_int_array(int *arr, int size, int value);
int find_empty_slot(int *frame, int frames);
void printSeparator(int width);
void printTableHeader(int frames);
void printTableRow(int page, int frame[], int frames, int found);
void printStats(int page_faults, int n);

int main()
{

    int choice;
    int pages[MAX];
    int n, frames;

    while (1)
    {
        printf("\n" CYAN "==============================" RESET "\n");
        printf(BOLD "  Page Replacement Simulator\n" RESET);
        printf(CYAN "==============================" RESET "\n");
        printf("1. FIFO Algorithm\n");
        printf("2. LRU Algorithm\n");
        printf("3. Optimal Algorithm\n");
        printf("4. Compare All\n");
        printf("5. Exit\n");
        printf(BOLD "Enter your choice: " RESET);
        scanf("%d", &choice);

        if (choice == 5)
        {
            printf("\n" GREEN "Exiting... Goodbye!\n" RESET);
            exit(0);
        }

        if (choice < 1 || choice > 5)
        {
            printf(RED "\nInvalid choice! Please select a valid option.\n" RESET);
            continue;
        }

        printf("\nEnter number of pages (1-%d): ", MAX);
        scanf("%d", &n);
        if (n < 1 || n > MAX)
        {
            printf(RED "\nError: Number of pages must be between 1 and %d!\n" RESET, MAX);
            continue;
        }

        printf("Enter reference string (space-separated, pages 0-%d):\n", MAX_PAGE);
        int invalid_page = 0;
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &pages[i]);
            if (pages[i] < 0 || pages[i] > MAX_PAGE)
                invalid_page = 1;
        }
        if (invalid_page)
        {
            printf(RED "\nError: Page numbers must be between 0 and %d!\n" RESET, MAX_PAGE);
            continue;
        }

        printf("Enter number of frames (1-%d): ", MAX);
        scanf("%d", &frames);
        if (frames < 1 || frames > MAX)
        {
            printf(RED "\nError: Frames must be between 1 and %d!\n" RESET, MAX);
            continue;
        }

        switch (choice)
        {

        case 1:
            fifo(pages, n, frames);
            break;

        case 2:
            lru(pages, n, frames);
            break;

        case 3:
            optimal(pages, n, frames);
            break;

        case 4:
        {
            int fifo_faults = fifo(pages, n, frames);
            int lru_faults = lru(pages, n, frames);
            int opt_faults = optimal(pages, n, frames);
            printf("\n" CYAN "==============================" RESET "\n");
            printf(BOLD "  Comparison Summary\n" RESET);
            printf(CYAN "==============================" RESET "\n");
            printf("Algorithm\tPage Faults\n");
            printf("FIFO\t\t%d\n", fifo_faults);
            printf("LRU\t\t%d\n", lru_faults);
            printf("Optimal\t\t%d\n", opt_faults);
            break;
        }

        default:
            printf("\nInvalid choice!\n");
        }
    }

    return 0;
}

// -------------------- Helper Functions --------------------

void printSeparator(int frames)
{
    printf("+------+");
    for (int i = 0; i < frames; i++)
        printf("------+");
    printf("--------+\n");
}

void printTableHeader(int frames)
{
    printSeparator(frames);
    printf("| " BOLD "Page" RESET " |");
    for (int i = 0; i < frames; i++)
        printf("  " BOLD "F%d" RESET "  |", i + 1);
    printf(" " BOLD "Status" RESET " |\n");
    printSeparator(frames);
}

void printTableRow(int page, int frame[], int frames, int found)
{
    printf("| %4d |", page);
    for (int j = 0; j < frames; j++)
    {
        if (frame[j] != -1)
            printf("  %2d  |", frame[j]);
        else
            printf("   " YELLOW "-" RESET "  |");
    }
    if (found)
        printf("  " GREEN "Hit " RESET "  |\n");
    else
        printf(" " RED "Fault" RESET "  |\n");
}

void printStats(int page_faults, int n)
{
    float hit_ratio = (float)(n - page_faults) / n;
    float fault_rate = (float)page_faults / n;

    printf("\n" BOLD "Total Page Faults" RESET " = " RED "%d" RESET "\n", page_faults);
    printf(BOLD "Hit Ratio" RESET "         = " GREEN "%.2f" RESET " (%.0f%%)\n", hit_ratio, hit_ratio * 100);
    printf(BOLD "Fault Rate" RESET "        = " RED "%.2f" RESET " (%.0f%%)\n", fault_rate, fault_rate * 100);
}

void init_int_array(int *arr, int size, int value)
{
    for (int i = 0; i < size; i++)
        arr[i] = value;
}

int find_empty_slot(int *frame, int frames)
{
    for (int i = 0; i < frames; i++)
    {
        if (frame[i] == -1)
            return i;
    }
    return -1;
}

// -------------------- FIFO Algorithm --------------------

int fifo(int pages[], int n, int frames)
{
    int *frame = (int *)malloc(sizeof(int) * frames);
    if (frame == NULL)
    {
        printf(RED "\nError: Memory allocation failed.\n" RESET);
        return 0;
    }

    int frame_index[MAX_PAGE + 1];
    init_int_array(frame_index, MAX_PAGE + 1, -1);

    int i;
    int page_faults = 0;
    int front = 0; // Points to the oldest page (FIFO)

    // Initialize frames with -1 (empty)
    init_int_array(frame, frames, -1);

    printf("\n" CYAN "===== FIFO Page Replacement =====" RESET "\n\n");

    printTableHeader(frames);

    for (i = 0; i < n; i++)
    {
        int idx = frame_index[pages[i]];
        int found = (idx != -1);

        if (!found)
        {
            // Replace oldest page
            if (frame[front] != -1)
                frame_index[frame[front]] = -1;
            frame[front] = pages[i];
            frame_index[pages[i]] = front;
            front = (front + 1) % frames;
            page_faults++;
        }

        // Print current state
        printTableRow(pages[i], frame, frames, found);
    }

    printSeparator(frames);
    printStats(page_faults, n);
    free(frame);
    return page_faults;
}

int findLRU(int time[], int frames)
{
    int i, pos = 0, min = time[0];

    for (i = 1; i < frames; i++)
    {
        if (time[i] < min)
        {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}

// -------------------- LRU Algorithm --------------------

int lru(int pages[], int n, int frames)
{
    int *frame = (int *)malloc(sizeof(int) * frames);
    int *time = (int *)malloc(sizeof(int) * frames);
    if (frame == NULL || time == NULL)
    {
        printf(RED "\nError: Memory allocation failed.\n" RESET);
        free(frame);
        free(time);
        return 0;
    }

    int frame_index[MAX_PAGE + 1];
    init_int_array(frame_index, MAX_PAGE + 1, -1);

    int counter = 0;
    int page_faults = 0;

    init_int_array(frame, frames, -1);
    init_int_array(time, frames, 0);

    printf("\n" CYAN "===== LRU Page Replacement =====" RESET "\n\n");

    printTableHeader(frames);

    for (int i = 0; i < n; i++)
    {
        int idx = frame_index[pages[i]];
        int found = (idx != -1);

        if (found)
        {
            counter++;
            time[idx] = counter;
        }

        if (!found)
        {
            int pos = find_empty_slot(frame, frames);
            if (pos == -1)
            {
                pos = findLRU(time, frames);
            }

            if (frame[pos] != -1)
                frame_index[frame[pos]] = -1;
            frame[pos] = pages[i];
            frame_index[pages[i]] = pos;
            counter++;
            time[pos] = counter;
            page_faults++;
        }

        printTableRow(pages[i], frame, frames, found);
    }

    printSeparator(frames);
    printStats(page_faults, n);
    free(frame);
    free(time);
    return page_faults;
}

int findOptimal(int pages[], int frame[], int n, int index, int frames)
{

    int pos = -1;
    int farthest = index;

    for (int i = 0; i < frames; i++)
    {

        int j;
        for (j = index; j < n; j++)
        {
            if (frame[i] == pages[j])
            {
                if (j > farthest)
                {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }

        // Page not found in future
        if (j == n)
            return i;
    }

    if (pos == -1)
        return 0;

    return pos;
}

// -------------------- Optimal Algorithm --------------------

int optimal(int pages[], int n, int frames)
{
    int *frame = (int *)malloc(sizeof(int) * frames);
    if (frame == NULL)
    {
        printf(RED "\nError: Memory allocation failed.\n" RESET);
        return 0;
    }

    int frame_index[MAX_PAGE + 1];
    init_int_array(frame_index, MAX_PAGE + 1, -1);

    int page_faults = 0;

    init_int_array(frame, frames, -1);

    printf("\n" CYAN "===== Optimal Page Replacement =====" RESET "\n\n");

    printTableHeader(frames);

    for (int i = 0; i < n; i++)
    {
        int idx = frame_index[pages[i]];
        int found = (idx != -1);

        if (!found)
        {
            int empty = find_empty_slot(frame, frames);
            if (empty != -1)
            {
                frame[empty] = pages[i];
                frame_index[pages[i]] = empty;
            }
            else
            {
                int pos = findOptimal(pages, frame, n, i + 1, frames);
                if (frame[pos] != -1)
                    frame_index[frame[pos]] = -1;
                frame[pos] = pages[i];
                frame_index[pages[i]] = pos;
            }

            page_faults++;
        }

        printTableRow(pages[i], frame, frames, found);
    }

    printSeparator(frames);
    printStats(page_faults, n);
    free(frame);
    return page_faults;
}
