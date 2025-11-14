#include "qr_scanner.h"

int init_scanner(qr_scanner_t *scanner_data) {
    scanner_data->capture = NULL;
    scanner_data->grey_frame = NULL;
    scanner_data->scanner = NULL;

    scanner_data->capture = cvCaptureFromCAM(CV_CAP_ANY);
    if (!scanner_data->capture) {
        fprintf(stderr, "Помилка: не вдалося відкрити камеру\n");
        return 1;
    }

    cvNamedWindow("QR Scanner YM", CV_WINDOW_AUTOSIZE);

    scanner_data->scanner = zbar_image_scanner_create();
    zbar_image_scanner_set_config(scanner_data->scanner, 0, ZBAR_CFG_ENABLE, 1);

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

        int width = scanner_data->grey_frame->width;
        int height = scanner_data->grey_frame->height;
        void *raw_data = scanner_data->grey_frame->imageData;

        zbar_image_t *zbar_image = zbar_image_create();
        zbar_image_set_format(zbar_image, zbar_fourcc('Y', '8', '0', '0'));
        zbar_image_set_size(zbar_image, width, height);
        zbar_image_set_data(zbar_image, raw_data, width * height, NULL);

        int n = zbar_scan_image(scanner_data->scanner, zbar_image);
        // щоб відкрити усі коди що були в кадрі, якщо їх більше 1, n кількість знайдених кодів

        if (n > 0) {
            const zbar_symbol_t *symbol = zbar_image_first_symbol(zbar_image);
            for (; symbol; symbol = zbar_symbol_next(symbol)) {
                printf("================================\n");
                printf("QR-КОД:\n%s\n", zbar_symbol_get_data(symbol));
                printf("================================\n\n");
            }
            zbar_image_destroy(zbar_image);
            scan_result = 0;
            break; 
        }

        cvShowImage("QR Scanner YM", frame);

        zbar_image_destroy(zbar_image);
        

        if ((cvWaitKey(10) & 255) == 27) {
            scan_result = 1;
            break;
        }
    }
    return scan_result;
}

void cleanup_scanner(qr_scanner_t *scanner_data) {  
    
    if (scanner_data->scanner) {
        zbar_image_scanner_destroy(scanner_data->scanner);
    }
    
    if (scanner_data->grey_frame) {
        cvReleaseImage(&scanner_data->grey_frame);
    }
    
    if (scanner_data->capture) {
        cvReleaseCapture(&scanner_data->capture);
    }
    
    cvDestroyAllWindows();
}