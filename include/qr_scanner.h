#ifndef QR_SCANNER_H
#define QR_SCANNER_H

#include <zbar.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <stdio.h>

typedef struct {
    CvCapture* capture;
    IplImage* grey_frame;
} qr_scanner_t;


int init_scanner(qr_scanner_t *scanner_data);

int run_scanner_loop(qr_scanner_t *scanner_data);

void cleanup_scanner(qr_scanner_t *scanner_data);

#endif 