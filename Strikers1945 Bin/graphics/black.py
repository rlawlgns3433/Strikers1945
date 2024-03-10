from PIL import Image, ImageOps

# Open the uploaded image
image = Image.open('D:/Kyungil/SFML/Strikers1945/Strikers1945 Bin/graphics/pauseWindow.png')

# Define the new image height (original image width is kept)
new_image_height = 960

# Calculate the difference in height between the new and old images
delta_height = new_image_height - image.height

# Add a black border to the top and bottom to match the new height
# Since we want an equal border on top and bottom, we divide the delta by 2
border = (0, delta_height // 2, 0, delta_height // 2)

# Create a new image with black borders
new_image = ImageOps.expand(image, border=border, fill='black')

# Save the new image
new_image_path = 'D:/Kyungil/SFML/Strikers1945/Strikers1945 Bin/graphics/pauseWindow_with_black_borders.png'
new_image.save(new_image_path)

new_image_path
