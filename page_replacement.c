#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <sys/select.h>
#include <unistd.h>
#endif

#define MAX 100
#define MAX_PAGE 999
#define TUI_WIDTH 78

// ANSI Color Codes
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// Function declarations
typedef enum
{
    RENDER_TUI_STEP = 1,
    RENDER_TUI_AUTO = 2
} RenderMode;

typedef enum
{
    ALGO_FIFO = 1,
    ALGO_LRU = 2,
    ALGO_OPT = 3
} AlgoType;

typedef struct
{
    RenderMode mode;
    int delay_ms;
} SimOptions;

int fifo(int pages[], int n, int frames, SimOptions opts);
int lru(int pages[], int n, int frames, SimOptions opts);
int optimal(int pages[], int n, int frames, SimOptions opts);
int simulate(AlgoType algo, int pages[], int n, int frames, SimOptions opts);
void init_int_array(int *arr, int size, int value);
int find_in_frame(int *frame, int frames, int page);
int find_empty_slot(int *frame, int frames);
int get_int(const char *prompt, int min, int max);
void sleep_ms(int ms);
void tui_begin(const char *title);
void tui_end(void);
void tui_enable_input(void);
void tui_disable_input(void);
int tui_kbhit(void);
int tui_getch(void);
void tui_progress_bar(int step, int total);
void tui_render_step(const char *alg, int step, int total, int current_page, int frames, int frame[], int found, int replaced_index, int faults, int pages[], RenderMode mode, int paused, int flash_victim);
int tui_wait_step(RenderMode mode, int *delay_ms, int *paused);
void generate_reference(int pages[], int n, int max_page);

int main()
{

    int choice;
    int pages[MAX];
    int n, frames;
    srand((unsigned)time(NULL));

    while (1)
    {
        printf("\n" CYAN "==============================" RESET "\n");
        printf(BOLD "  Page Replacement Simulator\n" RESET);
        printf(CYAN "==============================" RESET "\n");
        printf("1. FIFO Algorithm\n");
        printf("2. LRU Algorithm\n");
        printf("3. Optimal Algorithm\n");
        printf("4. Exit\n");
        choice = get_int("Enter your choice: ", 1, 4);

        if (choice == 4)
        {
            printf("\n" GREEN "Exiting... Goodbye!\n" RESET);
            exit(0);
        }

        if (choice < 1 || choice > 4)
        {
            printf(RED "\nInvalid choice! Please select a valid option.\n" RESET);
            continue;
        }

        n = get_int("\nEnter number of references (1-100): ", 1, MAX);

        int max_page = get_int("Enter max page number (1-999): ", 1, MAX_PAGE);

        frames = get_int("Enter number of frames (1-100): ", 1, MAX);

        generate_reference(pages, n, max_page);

        int delay_ms = 250;
        printf("\nOutput mode:\n1. TUI step-by-step\n2. TUI auto (animated)\n");
        int mode_choice = get_int("Choose mode (1-2): ", 1, 2);
        if (mode_choice == 2)
            delay_ms = get_int("Auto delay in ms (50-2000): ", 50, 2000);

        SimOptions opts = {(RenderMode)mode_choice, delay_ms};

        switch (choice)
        {

        case 1:
            fifo(pages, n, frames, opts);
            break;

        case 2:
            lru(pages, n, frames, opts);
            break;

        case 3:
            optimal(pages, n, frames, opts);
            break;

        default:
            printf("\nInvalid choice!\n");
        }
    }

    return 0;
}

// -------------------- Helper Functions --------------------

int get_int(const char *prompt, int min, int max)
{
    char buf[128];
    while (1)
    {
        long val;
        char *end;
        printf("%s", prompt);
        fflush(stdout);
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            return min;
        val = strtol(buf, &end, 10);
        if (end == buf)
        {
            printf(RED "Invalid input. Try again.\n" RESET);
            continue;
        }
        if (val < min || val > max)
        {
            printf(RED "Value must be between %d and %d.\n" RESET, min, max);
            continue;
        }
        return (int)val;
    }
}

void sleep_ms(int ms)
{
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    usleep((useconds_t)(ms * 1000));
#endif
}

void tui_begin(const char *title)
{
    printf("\033[2J\033[H\033[?25l");
    printf(BOLD "%s" RESET "\n", title);
    fflush(stdout);
    tui_enable_input();
}

void tui_end(void)
{
    tui_disable_input();
    printf("\033[?25h");
    fflush(stdout);
}

#ifdef _WIN32
void tui_enable_input(void) {}
void tui_disable_input(void) {}
int tui_kbhit(void) { return _kbhit(); }
int tui_getch(void) { return _getch(); }
#else
static struct termios tui_orig_termios;
static int tui_termios_enabled = 0;

void tui_enable_input(void)
{
    struct termios raw;
    if (tui_termios_enabled)
        return;
    tcgetattr(STDIN_FILENO, &tui_orig_termios);
    raw = tui_orig_termios;
    raw.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    tui_termios_enabled = 1;
}

void tui_disable_input(void)
{
    if (!tui_termios_enabled)
        return;
    tcsetattr(STDIN_FILENO, TCSANOW, &tui_orig_termios);
    tui_termios_enabled = 0;
}

int tui_kbhit(void)
{
    fd_set set;
    struct timeval tv;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    return select(STDIN_FILENO + 1, &set, NULL, NULL, &tv) > 0;
}

int tui_getch(void)
{
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) == 1)
        return c;
    return -1;
}
#endif

void tui_progress_bar(int step, int total)
{
    int bar_width = TUI_WIDTH - 20;
    int filled = (total == 0) ? 0 : (step * bar_width) / total;
    printf("[");
    for (int i = 0; i < bar_width; i++)
        putchar(i < filled ? '#' : '-');
    printf("] %3d%%\n", total == 0 ? 0 : (step * 100) / total);
}

void tui_render_step(const char *alg, int step, int total, int current_page, int frames, int frame[], int found, int replaced_index, int faults, int pages[], RenderMode mode, int paused, int flash_victim)
{
    float hit_ratio = (float)(step - faults) / step;
    const char *status_color = found ? GREEN : RED;
    const char *status_text = found ? "HIT" : "FAULT";

    printf("\033[H");
    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("=");
    printf("+\n");
    printf("|" BOLD " Page Replacement Simulator " RESET "- " CYAN "%s" RESET, alg);
    for (int i = 0; i < TUI_WIDTH - 2 - 29 - (int)strlen(alg); i++)
        printf(" ");
    printf("|\n");
    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("=");
    printf("+\n");
    printf("Step: " BOLD "%d/%d" RESET "  Page: " BOLD "%d" RESET "  Status: %s%s" RESET, step, total, current_page, status_color, status_text);
    if (mode == RENDER_TUI_AUTO)
        printf("  Mode: %s%s" RESET, paused ? YELLOW : GREEN, paused ? "PAUSED" : "AUTO");
    else
        printf("  Mode: " CYAN "STEP" RESET);
    printf("\n");
    tui_progress_bar(step, total);

    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("-");
    printf("+\n");
    printf("|" BOLD " Frames " RESET);
    for (int i = 0; i < TUI_WIDTH - 2 - 8; i++)
        printf(" ");
    printf("|\n");
    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("-");
    printf("+\n");
    for (int i = 0; i < frames; i++)
    {
        const char *color = RESET;
        if (!found && i == replaced_index)
            color = RED;
        else if (frame[i] == current_page)
            color = GREEN;
        printf("| F%-2d: %s", i + 1, color);
        if (frame[i] == -1)
            printf("-");
        else
            printf("%d", frame[i]);
        printf(RESET);
        for (int s = 0; s < TUI_WIDTH - 2 - 7; s++)
            printf(" ");
        printf("|\n");
    }

    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("-");
    printf("+\n");
    printf("|" BOLD " Stats " RESET);
    for (int i = 0; i < TUI_WIDTH - 2 - 7; i++)
        printf(" ");
    printf("|\n");
    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("-");
    printf("+\n");
    printf("| Faults: " RED "%d" RESET "  Hits: " GREEN "%d" RESET "  Hit Ratio: " GREEN "%.2f" RESET,
           faults, step - faults, hit_ratio);
    for (int i = 0; i < TUI_WIDTH - 2 - 44; i++)
        printf(" ");
    printf("|\n");

    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("-");
    printf("+\n");
    printf("|" BOLD " Reference String " RESET);
    for (int i = 0; i < TUI_WIDTH - 2 - 20; i++)
        printf(" ");
    printf("|\n");
    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("-");
    printf("+\n");
    printf("| ");
    int col = 2;
    for (int i = 0; i < total; i++)
    {
        char buf[8];
        if (i == step - 1)
            snprintf(buf, sizeof(buf), "[%d]", pages[i]);
        else
            snprintf(buf, sizeof(buf), "%d", pages[i]);
        int len = (int)strlen(buf);
        if (col + len + 1 > TUI_WIDTH - 1)
        {
            for (int s = col; s < TUI_WIDTH - 1; s++)
                printf(" ");
            printf("|\n| ");
            col = 2;
        }
        if (i == step - 1)
            printf(BOLD CYAN "%s" RESET, buf);
        else
            printf("%s", buf);
        printf(" ");
        col += len + 1;
    }
    for (int s = col; s < TUI_WIDTH - 1; s++)
        printf(" ");
    printf("|\n");

    printf("+");
    for (int i = 0; i < TUI_WIDTH - 2; i++)
        printf("-");
    printf("+\n");
    printf("Legend: " GREEN "HIT" RESET ", " RED "FAULT" RESET ", " YELLOW "-" RESET " empty\n");
    if (mode == RENDER_TUI_AUTO)
        printf("Controls: " BOLD "P" RESET " pause/resume, " BOLD "N" RESET " step, " BOLD "+/-" RESET " speed, " BOLD "Q" RESET " quit\n");
    else
        printf("Controls: " BOLD "Enter" RESET " next step, " BOLD "Q" RESET " quit\n");
    fflush(stdout);
}

int tui_wait_step(RenderMode mode, int *delay_ms, int *paused)
{
    if (mode == RENDER_TUI_AUTO)
    {
        int elapsed = 0;
        while (elapsed < *delay_ms)
        {
            if (tui_kbhit())
            {
                int c = tui_getch();
                if (c == 'q' || c == 'Q')
                    return 1;
                if (c == 'p' || c == 'P')
                    *paused = !(*paused);
                if (c == 'n' || c == 'N')
                    *paused = 1;
                if (c == '+')
                    *delay_ms = (*delay_ms > 50) ? (*delay_ms - 50) : 50;
                if (c == '-')
                    *delay_ms = (*delay_ms < 2000) ? (*delay_ms + 50) : 2000;
            }
            if (*paused)
                break;
            sleep_ms(25);
            elapsed += 25;
        }
        while (*paused)
        {
            if (tui_kbhit())
            {
                int c = tui_getch();
                if (c == 'q' || c == 'Q')
                    return 1;
                if (c == 'p' || c == 'P')
                {
                    *paused = 0;
                    break;
                }
                if (c == 'n' || c == 'N')
                    break;
                if (c == '+')
                    *delay_ms = (*delay_ms > 50) ? (*delay_ms - 50) : 50;
                if (c == '-')
                    *delay_ms = (*delay_ms < 2000) ? (*delay_ms + 50) : 2000;
            }
            sleep_ms(25);
        }
        return 0;
    }
    if (mode == RENDER_TUI_STEP)
    {
        while (1)
        {
            if (tui_kbhit())
            {
                int c = tui_getch();
                if (c == 'q' || c == 'Q')
                    return 1;
                if (c == '\n' || c == '\r')
                    return 0;
            }
            sleep_ms(25);
        }
    }
    return 0;
}

void generate_reference(int pages[], int n, int max_page)
{
    for (int i = 0; i < n; i++)
        pages[i] = rand() % (max_page + 1);
}

void init_int_array(int *arr, int size, int value)
{
    for (int i = 0; i < size; i++)
        arr[i] = value;
}

int find_in_frame(int *frame, int frames, int page)
{
    for (int i = 0; i < frames; i++)
    {
        if (frame[i] == page)
            return i;
    }
    return -1;
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

int simulate(AlgoType algo, int pages[], int n, int frames, SimOptions opts)
{
    int *frame = (int *)malloc(sizeof(int) * frames);
    if (frame == NULL)
    {
        printf(RED "\nError: Memory allocation failed.\n" RESET);
        return 0;
    }

    int *time = NULL;
    int counter = 0;
    int front = 0;

    if (algo == ALGO_LRU)
    {
        time = (int *)malloc(sizeof(int) * frames);
        if (time == NULL)
        {
            printf(RED "\nError: Memory allocation failed.\n" RESET);
            free(frame);
            return 0;
        }
        init_int_array(time, frames, 0);
    }

    init_int_array(frame, frames, -1);

    const char *title = (algo == ALGO_FIFO)  ? "FIFO Page Replacement"
                        : (algo == ALGO_LRU) ? "LRU Page Replacement"
                                             : "Optimal Page Replacement";

    int page_faults = 0;
    int steps_done = 0;
    int aborted = 0;
    int paused = 0;
    int delay_ms = opts.delay_ms;

    tui_begin(title);

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int idx = find_in_frame(frame, frames, page);
        int found = (idx != -1);
        int replaced_index = -1;
        int flash = 0;

        if (found)
        {
            if (algo == ALGO_LRU)
                time[idx] = ++counter;
        }
        else
        {
            int pos = -1;

            if (algo == ALGO_FIFO)
            {
                pos = front;
                front = (front + 1) % frames;
            }
            else
            {
                int empty = find_empty_slot(frame, frames);
                if (empty != -1)
                    pos = empty;
                else if (algo == ALGO_LRU)
                    pos = findLRU(time, frames);
                else
                    pos = findOptimal(pages, frame, n, i + 1, frames);
            }

            if (frame[pos] != -1)
            {
                replaced_index = pos;
                flash = 1;
                tui_render_step(title, i + 1, n, page, frames, frame, found, replaced_index, page_faults, pages, opts.mode, paused, flash);
                sleep_ms(60);
            }

            frame[pos] = page;
            replaced_index = pos;
            page_faults++;

            if (algo == ALGO_LRU)
                time[pos] = ++counter;
        }

        tui_render_step(title, i + 1, n, page, frames, frame, found, replaced_index, page_faults, pages, opts.mode, paused, 0);
        steps_done = i + 1;
        if (tui_wait_step(opts.mode, &delay_ms, &paused))
        {
            aborted = 1;
            break;
        }
    }

    if (aborted)
        printf("\n\nSimulation aborted at step %d of %d.\n", steps_done, n);
    else
        printf("\n\nSimulation complete.\n");
    {
        int total = steps_done > 0 ? steps_done : n;
        float hit_ratio = (float)(total - page_faults) / total;
        float fault_rate = (float)page_faults / total;
        printf("\n" BOLD "Total Page Faults" RESET " = " RED "%d" RESET "\n", page_faults);
        printf(BOLD "Hit Ratio" RESET "         = " GREEN "%.2f" RESET " (%.0f%%)\n", hit_ratio, hit_ratio * 100);
        printf(BOLD "Fault Rate" RESET "        = " RED "%.2f" RESET " (%.0f%%)\n", fault_rate, fault_rate * 100);
    }
    tui_end();

    free(frame);
    free(time);
    return page_faults;
}

int fifo(int pages[], int n, int frames, SimOptions opts)
{
    return simulate(ALGO_FIFO, pages, n, frames, opts);
}

int lru(int pages[], int n, int frames, SimOptions opts)
{
    return simulate(ALGO_LRU, pages, n, frames, opts);
}

int optimal(int pages[], int n, int frames, SimOptions opts)
{
    return simulate(ALGO_OPT, pages, n, frames, opts);
}
