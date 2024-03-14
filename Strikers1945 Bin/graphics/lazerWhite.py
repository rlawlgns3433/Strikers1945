from PIL import Image

def remove_white_background(image_path, output_path):
    # 이미지를 열고 RGBA 모드로 변환
    img = Image.open(image_path).convert("RGBA")
    datas = img.getdata()

    newData = []
    for item in datas:
        # 흰색을 찾으면, 해당 픽셀을 투명하게 만듦
        if item[0] < 2 and item[1] < 2 and item[2] < 2:  # RGB 각각의 값이 220보다 크면 흰색으로 간주
            newData.append((255, 255, 255, 0))  # 투명한 픽셀로 변경
        else:
            newData.append(item)

    # 수정된 데이터로 새 이미지 생성
    img.putdata(newData)
    img.save(output_path, "PNG")  # PNG 형식으로 저장

# 사용 예시
remove_white_background("D:/Kyungil/SFML/Strikers1945/Strikers1945 Bin/graphics/lazer.png", "D:/Kyungil/SFML/Strikers1945/Strikers1945 Bin/graphics/lazer1.png")
