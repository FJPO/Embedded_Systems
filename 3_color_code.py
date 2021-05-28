import numpy as np
import cv2

class Rect:
    size = 70
    h_sensivity = 20
    thickness = 1
    s_h = 255
    v_h = 255
    s_l = 50
    v_l = 50
    font = cv2.FONT_HERSHEY_SIMPLEX
    fontScale = 0.7
    correct = False
    border_color = (0, 255, 0)




    def __init__(self, start_point, end_point, color_index, color_name):
        self.start_point = start_point
        self.end_point = end_point
        upper = np.array([color_index + self.h_sensivity, self.s_h, self.v_h])
        lower = np.array([color_index - self.h_sensivity, self.s_l, self.v_l])

        self.rect = cv2.rectangle(frame, start_point, end_point, self.border_color, self.thickness)

        mask_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)[start_point[1]:end_point[1] + 1, start_point[0]:end_point[0] + 1]
        self.color_mask = cv2.inRange(mask_frame, lower, upper)
        self.color_name = color_name

    def show_output(self):
        green_rate = np.count_nonzero(self.color_mask) / (self.size * self.size)


        if green_rate > 0.9:
            self.correct = True
            cv2.putText(self.rect, self.color_name, (int(self.start_point[0] - self.size/3), int(self.end_point[1]+self.size/3)), self.font, self.fontScale, self.border_color, self.thickness, cv2.LINE_AA)

        else:
            cv2.putText(self.rect, 'not ' + self.color_name, (int(self.start_point[0] - self.size/3), int(self.end_point[1]+self.size/3)), self.font, self.fontScale, self.border_color, self.thickness, cv2.LINE_AA)

    def isFulfilled(self):
        return self.correct

    def show_correct(self):
        self.rect
        cv2.putText(self.rect, '+', (int(self.start_point[0] - self.size*2/3), int(self.end_point[1]+self.size*2/3)), self.font, self.fontScale, self.border_color, self.thickness, cv2.LINE_AA)


def gstreamer_pipeline(
        capture_width=1280,
        capture_height=720,
        display_width=1280,
        display_height=720,
        framerate=30,
        flip_method=0,
):
    return (
            "nvarguscamerasrc ! "
            "video/x-raw(memory:NVMM), "
            "width=(int)%d, height=(int)%d, "
            "format=(string)NV12, framerate=(fraction)%d/1 ! "
            "nvvidconv flip-method=%d ! "
            "video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! "
            "videoconvert ! "
            "video/x-raw, format=(string)BGR ! appsink max-buffers=1 drop=true"
            % (
                capture_width,
                capture_height,
                framerate,
                flip_method,
                display_width,
                display_height,
            )
    )



def process(frame, code):
    rect_size = 70
    h_sensivity = 20
    s_h = 255
    v_h = 255
    s_l = 50
    v_l = 50
    height, width, channels = frame.shape
    points = [
        (
            (int(height * 1 / 5 - rect_size / 2), int(width / 2 - rect_size / 2)),
            (int(height * 1 / 5 + rect_size / 2), int(width / 2 + rect_size / 2)),
        ),
        (
            (int(height * 2 / 5 - rect_size / 2), int(width / 2 - rect_size / 2)),
            (int(height * 2 / 5 + rect_size / 2), int(width / 2 + rect_size / 2)),
        ),
        (
            (int(height * 3 / 5 - rect_size / 2), int(width / 2 - rect_size / 2)),
            (int(height * 3 / 5 + rect_size / 2), int(width / 2 + rect_size / 2)),
        ),
        (
            (int(height * 4 / 5 - rect_size / 2), int(width / 2 - rect_size / 2)),
            (int(height * 4 / 5 + rect_size / 2), int(width / 2 + rect_size / 2)),
        )
    ]
    colors = [

        ('red', 180),
        ('green', 60),
        ('yellow', 30),
        ('blue', 120),
    ]

    for i in range((len(code))):
        rect = Rect(points[i][0],
                    points[i][1], colors[i][1], colors[i][0])
        if not code[i]:
            rect.show_output()
            code[i] = rect.isFulfilled()
        else:
            rect.show_correct()

    thickness = 2

    return code


print('Press 4 to Quit the Application\n')

# Open Default Camera
cap = cv2.VideoCapture(0)  # gstreamer_pipeline(flip_method=4), cv2.CAP_GSTREAMER)
code = [False for i in range(4)]
while (cap.isOpened()):
    if all(code):
        print('pin correct, exiting program')
        break
    ret, frame = cap.read()

    frame = cv2.flip(frame, 180)
    code = process(frame, code)
    cond = False
    # Show video
    cv2.imshow('Cam', frame)

    # Exit if "4" is pressed
    k = cv2.waitKey(1) & 0xFF
    if k == 52:
        print('Good Bye!')
        break

cap.release()
cv2.destroyAllWindows()
