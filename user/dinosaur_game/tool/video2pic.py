
import cv2
import time
from pathlib import Path

resize = (128, 64)

def video2imgs(videoPath, outfile_name, rate, start, end):
    # if not imgPath.is_exists():
    #     imgPath.mkdir()

    try:
        cap = cv2.VideoCapture(videoPath)
        # time.sleep(1)
    except:
        print("can't open video")


    fps = cap.get(cv2.CAP_PROP_FPS)      # get fps
    print('fps:',fps)

    frames_counter = 1
    pic_count = 0

    # Calculate the corresponding frame numbers
    start_frame = int(start * fps)
    end_frame = int(end * fps)

    with open(outfile_name, 'w') as f:

        f.write('#include \"stdint.h\"\n\n')
        f.write("const uint8_t img[] = {\n")
        while(True):
            ret, frame = cap.read()

            if not ret or frames_counter > end_frame:
                print("process finished!")
                break
            else:
                if frames_counter >= start_frame and frames_counter % rate == 0:
                    resized_frame = cv2.resize(frame, resize)
                    # img = cv2.imread(resized_frame)
                    gray = cv2.cvtColor(resized_frame, cv2.COLOR_BGR2GRAY)
                    _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

                    for i in range(64):
                        for j in range(128):
                            if binary[i][j] == 255:
                                print('1',end='')
                            else:
                                print('0', end='')
                        print()
                    print("\n")

                    try:
                        data = 0

                        for i in range(8):
                            f.write("    ")
                            for j in range(128):
                                for k in range(8):
                                    # data |= 1<<k if(binary[i * 8 + k][j] == 255) else 0<<k
                                    if binary[k + i * 8][j] == 255:
                                        data |= 1 << k
                                    else:
                                        data |= 0 << k

                                # print(f"{data:#02x}",end=",")
                                f.write(f"{data:#02x},")
                                data = 0
                            f.write("\n")
                            # print()
                            
                        f.write("\n")
                        # print("\n")
                    except:
                        print("error")

                    # imgname = 'jpgs_' + str(pic_count).rjust(3,'0') + ".jpg"
                    # newPath = imgPath / imgname
                    # print(imgname)
                    # cv2.imwrite(newPath, frame, [cv2.IMWRITE_JPEG_QUALITY, 100])

                    pic_count += 1
            frames_counter += 1
        f.write("};\n")
        f.write(f"    //total {pic_count} pics")
        # f.seek(len('#include \"stdint.h\"\n\n') + 1)
        # f.write(f"const uint8_t img[{pic_count}] = " + "{\n")

    cap.release()
    
    print(f"total {frames_counter}, sampled {pic_count} pics")

    with open(outfile_name.stem + ".h", 'w') as f:
        f.write('#include \"stdint.h\"\n\n')
        f.write("#ifndef __" + outfile_name.stem.upper() + "_H\n")
        f.write("#define __" + outfile_name.stem.upper() + "_H\n\n")
        f.write(f"#define FRAME_SIZE {pic_count}\n\n")

        f.write("extern const uint8_t img[];\n\n")
        f.write("#endif\n")


video_name, outfile_name, r, start, end = input("Enter video name, outfile name, sample_rate, start_time, end_time:\n ").split()

p1 = Path.cwd() / video_name
p2 = Path.cwd() / outfile_name
rate = int(r)

video2imgs(p1, p2, rate, int(start), int(end))
input('finished, press enter')


###############################################################
# import cv2
# from pathlib import Path

# img_path = Path.cwd() / 'img'/ 'jpgs_001.jpg'

# img = cv2.imread(img_path)
# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

# # cv2.imshow('Binary Image', binary)
# cv2.imwrite(Path.cwd() / 'img'/'binary.png', binary)

# for i in range(64):
#     for j in range(128):
#         if binary[i][j] == 255:
#             print('X',end='')
#         else:
#             print('O', end='')
#     print()

# # print(binary)

# x = input('Enter your name:')


###############################################################
# import cv2
# import numpy as np
# from pathlib import Path

# img_path = Path.cwd() / 'img'/ 'jpgs_001.jpg'

# # Read image
# img = cv2.imread(img_path)

# # Reshape the image
# pixels = img.reshape((-1, 3))
# pixels = np.float32(pixels)

# # Define criteria and apply K-means
# criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 100, 0.2)
# _, labels, centers = cv2.kmeans(pixels, 2, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)

# # Convert back to 8-bit values
# centers = np.uint8(centers)
# result = centers[labels.flatten()]
# result = result.reshape(img.shape)

# print(result)
# x = input('Enter your name:')
# # Display or save the result
# # cv2.imshow('2-Color Image', result)
###############################################################

# import cv2
# videoCapture = cv2.VideoCapture('v.mp4')

# fps = 25  # 保存视频的帧率
# size = (128, 64)  # 保存视频的大小

# videoWriter = cv2.VideoWriter('resizex1.mp4', cv2.VideoWriter_fourcc('X', 'V', 'I', 'D'), fps, size)
# i = 0
# while True:
#     success, frame = videoCapture.read()
#     if success:
#         i += 1
#         if (i >= 1 and i <= 8000):
#             frame = cv2.resize(frame, size)
#             videoWriter.write(frame)
#         if (i > 8000):
#             print("success resize")
#             break
#     else:
#         print('end')
#         break
