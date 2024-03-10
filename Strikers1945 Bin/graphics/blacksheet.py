# Re-importing the Image module as the execution state has been reset.
from PIL import Image

# Create a black image of 540x960 pixels
black_image = Image.new('RGB', (540, 960), color = 'black')

# Save the image to a file
black_image_path = 'D:/Kyungil/SFML/Strikers1945/Strikers1945 Bin/graphics/black_screen_540x960.png'
black_image.save(black_image_path)

black_image_path
