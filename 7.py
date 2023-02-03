import cv2

cap = cv2.VideoCapture(0) # Открываем веб-камеру
fourcc = cv2.VideoWriter_fourcc(*'XVID') # Устанавливаем формат видео
out = cv2.VideoWriter('output.avi', fourcc, 20.0, (640, 480)) # Открываем видеофайл для записи

while(cap.isOpened()): # Цикл для чтения кадров из веб-камеры
    ret, frame = cap.read() # Читаем кадр
    if ret==True:
        out.write(frame) # Записываем кадр в файл
        cv2.imshow('frame', frame) # Отображаем кадр на экране
        if cv2.waitKey(1) & 0xFF == ord('q'): # Если нажали "q", выходим из цикла
            break
    else:
        break

# Освобождаем ресурсы
cap.release()
out.release()
cv2.destroyAllWindows()
