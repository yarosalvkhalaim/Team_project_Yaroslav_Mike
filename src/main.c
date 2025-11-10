#include "qr_scanner.h"

int main(void) {
    qr_scanner_t scanner_data;
    
    if (init_scanner(&scanner_data) != 0) {
        return 1;
    }
    
    run_scanner_loop(&scanner_data);
    
    cleanup_scanner(&scanner_data);

    return 0;
}