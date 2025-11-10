#include "qr_scanner.h"

int init_scanner(qr_scanner_t *scanner_data) {
    scanner_data->capture = NULL;
    scanner_data->grey_frame = NULL;

    scanner_data->capture = cvCaptureFromCAM(CV_CAP_ANY);
    if (!scanner_data->capture) {
        fprintf(stderr, "Помилка: не вдалося відкрити камеру\n");
        return 1;
    }

    cvNamedWindow("QR Scanner YM", CV_WINDOW_AUTOSIZE);

    printf("Вікно камери відкрито. Наведіть QR-код на камеру\n");
    printf("Натисніть ESC, щоб вийти\n\n");

    return 0;
}

int run_scanner_loop(qr_scanner_t *scanner_data) {
    int scan_result = 0;

    while (1) {
        IplImage* frame = cvQueryFrame(scanner_data->capture);
        if (!frame) {
            fprintf(stderr, "Помилка: не вдалося захопити кадр\n");
            break;
        }

        if (!scanner_data->grey_frame) {
            scanner_data->grey_frame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
        }
        cvCvtColor(frame, scanner_data->grey_frame, CV_BGR2GRAY); //Зробив зображення сірим 


        cvShowImage("QR Scanner YM", frame);
        

        if ((cvWaitKey(10) & 255) == 27) {
            scan_result = 1;
            break;
        }
    }
    return scan_result;
}

void cleanup_scanner(qr_scanner_t *scanner_data) {    
    if (scanner_data->grey_frame) {
        cvReleaseImage(&scanner_data->grey_frame);
    }
    
    if (scanner_data->capture) {
        cvReleaseCapture(&scanner_data->capture);
    }
    
    cvDestroyAllWindows();
}