#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import cv2
import numpy as np
from pyzbar.pyzbar import decode
import math

class QRCodeScanner:
    def __init__(self):
        self.cap = cv2.VideoCapture(0)
        self.cap.set(3, 640)
        self.cap.set(4, 480)
        self.center_x = 320  # Assuming center of the image is at (320, 240)
        self.center_y = 240

    def calculate_angle(self, qr_x, qr_y):
        dx = qr_x - self.center_x
        dy = qr_y - self.center_y
        angle = math.atan2(dy, dx) * 180 / math.pi
        return angle

    def scan_qr_codes(self):
        while True:
            success, img = self.cap.read()
            for barcode in decode(img):
                qr_x = barcode.rect.left + barcode.rect.width / 2
                qr_y = barcode.rect.top + barcode.rect.height / 2
                angle = self.calculate_angle(qr_x, qr_y)
                print(f"QR Code Angle: {angle:.2f} degrees")

                myData = barcode.data.decode('utf-8')
                print(myData)
                pts = np.array([barcode.polygon], np.int32)
                pts = pts.reshape((-1, 1, 2))
                cv2.polylines(img, [pts], True, (255, 0, 255), 5)
                pts2 = barcode.rect
                cv2.putText(img, myData, (pts2[0], pts2[1]), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (255, 0, 255), 2)

            cv2.imshow('Result', img)
            cv2.waitKey(1)

    def close(self):
        self.cap.release()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    qr_scanner = QRCodeScanner()
    qr_scanner.scan_qr_codes()
    qr_scanner.close()

