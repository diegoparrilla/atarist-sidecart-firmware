#include "include/floppyselector.h"

//================================================================
// Floppy selector

__uint8_t floppy_selector()
{
    PRINT_APP_HEADER(VERSION);

    printf("\r\n");

    send_command(LIST_FLOPPIES, NULL, 0);

    please_wait("Loading available Floppy images...", WAIT_TIME);

    printf("\r\n");

    int num_files = -1;
    __uint32_t file_list_mem = FILE_LIST_START_ADDRESS;

#ifdef _DEBUG
    printf("Reading file list from memory address: 0x%08X\r\n", file_list_mem);
#endif
    char *file_array = read_files_from_memory((__uint8_t *)file_list_mem);

    if (!file_array)
    {
        printf("No files found. Check if you have Floppie images copied in your microSD card!\r\n");
        printf("Press any key to exit...\r\n");
        // Back to main menu
        return 0; // 0 is go to menu
    }
    __int16_t floppy_number = display_paginated_content(file_array, get_file_count(file_array), ELEMENTS_PER_PAGE, "Floppy images");

    if (floppy_number <= 0)
    {
        // Back to main menu
        return 0; // 0 is go to menu
    }

    locate(0, 22);

    printf("\033KSelected the Floppy image file: %d. ", floppy_number);

    print_file_at_index(file_array, floppy_number - 1, 0);

    send_command(LOAD_FLOPPY, &floppy_number, 2);

    printf("\033KFloppy image file loaded. ");

    return 1; // Positive is OK
}
