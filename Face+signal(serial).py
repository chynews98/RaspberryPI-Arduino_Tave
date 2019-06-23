
# 라즈베리 파이에서 카메라(라즈베리파이 전용) 인식 후 본인 확인 후 본인이 맞으면 아두이노로 신호 보내는 시스템

import cv2
import numpy as np
from os import listdir
from os.path import isfile, join
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import serial



 # 카메라 얼굴 촬영에 앞서 기본 설정

data_path = '/home/pi/faces/'       # 본인 사진 저장되어 있는 폴더

onlyfiles = [f for f in listdir(data_path) if isfile(join(data_path,f))]        # list comprehension


ard = serial.Serial('/dev/ttyUSB0') # 아두이노와 라즈베리 파이 serial 유선 통신 연결




 # 본인 얼굴 사진 100장 미리 학습시키기

Training_Data, Labels = [], []

 

for i, files in enumerate(onlyfiles):                       # i -> index 

    image_path = data_path + onlyfiles[i]

    images = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)       

    Training_Data.append(np.asarray(images, dtype=np.uint8))

    Labels.append(i)

Labels = np.asarray(Labels, dtype=np.int32)

model = cv2.face.LBPHFaceRecognizer_create()

model.train(np.asarray(Training_Data), np.asarray(Labels))          # 본인 사진 100장 model 학습


print("Model Training Complete!!!!!")


 
face_classifier = cv2.CascadeClassifier('opencv/opencv-4.0.1/data/haarcascades/haarcascade_frontalface_default.xml')    # 얼굴 인식 코드


def face_detector(img, size = 0.5):     # 얼굴 인식 함수

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)        # 흑백 처리

    faces = face_classifier.detectMultiScale(gray,1.3,5)

    if faces is():

        return img,[]

    for(x,y,w,h) in faces:

        cv2.rectangle(img, (x,y),(x+w,y+h),(0,255,255),2)       # 얼굴 인식 영역 사각형 표시

        roi = img[y:y+h, x:x+w]

        roi = cv2.resize(roi, (200,200))

     return img,roi


    

  # 라즈베리 파이 카메라 설정

 
camera = PiCamera()   

camera.resolution = (640, 480)

camera.framerate = 32

rawCapture = PiRGBArray(camera, size=(640, 480))


time.sleep(0.1)

 
count = 0




# 카메라로 얼굴 촬영 

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):          

    print("going on camera.capture")

    

    img = frame.array

    

    image, face = face_detector(img)        # 얼굴 인식 함수 호출

    count += 1

    print(count, " Face dectecting complete")

    try:

        face = cv2.cvtColor(face, cv2.COLOR_BGR2GRAY)

        result = model.predict(face)

 

        if result[1] < 500:

            confidence = int(100*(1-(result[1])/300))
            display_string = str(confidence)+'% Confidence it is user'
            
        cv2.putText(image,display_string,(100,120), cv2.FONT_HERSHEY_COMPLEX,1,(250,120,255),2)


        if confidence > 75:             # 동일인 확률 75% 이상이면

            cv2.putText(image, "Unlocked", (250, 450), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)

            ard.write('u\r\n'.encode())         # 아두이노에 신호 보냄 - C언어 : \r은 커서를  맨 앞으로 이동, \n은 엔터(개행) 의미

            cv2.imshow('Face Cropper', image)

        else:

            cv2.putText(image, "Locked", (250, 450), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255), 2)

            ard.write('l\r\n'.encode())         # 아두이노에 신호 보냄

            cv2.imshow('Face Cropper', image)

    except:

        cv2.putText(image, "Face Not Found", (250, 450), cv2.FONT_HERSHEY_COMPLEX, 1, (255, 0, 0), 2)       # 얼굴 인식 실패할 때

        cv2.imshow('Face Cropper', image)

    key = cv2.waitKey(1) & 0xFF    


 

    rawCapture.truncate(0)

    if key == ord("q"):                 # 프로그램 종료키
        
        cv2.destroyAllWindows() 

        break
