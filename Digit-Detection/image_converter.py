import matplotlib.pyplot as plt
import numpy as np
import os
import random
from skimage.color import rgb2gray
import skimage.io


rgb_weights = [0.2989, 0.5870, 0.1140]

# Array with the converted images with only 0/255 values.
converted_images = []

# Loop through files in tmp folder
for filename in os.listdir('./tmp/'):
    if filename.endswith('.jpg'):
        original = skimage.io.imread('./tmp/'+filename)
    # Conver to grayscale
    grayscale_image = np.dot(original[..., :3], rgb_weights)
    output_array = []
    if filename.find('m') != -1:
        output_array.append(0)
    else:
        output_array.append(1)
    # Make it only black and white nodes to make a digital image
    for i in range(grayscale_image.shape[0]):
        for j in range(grayscale_image.shape[1]):
            if grayscale_image[i][j] <= 110:
                grayscale_image[i][j] = 0.0
            else:
                grayscale_image[i][j] = 255
            output_array.append(grayscale_image[i][j])
    # Plot images
    fig = plt.imshow(grayscale_image, cmap=plt.get_cmap("gray"))
    plt.show()
    converted_images.append(output_array)

# Suffle inputs
random.shuffle(converted_images)

# Create a text file to write input and outputs for every image

file_object = open("./tmp/train.txt", "w+")
L = ['topology: 100 200  2 \n']
for i in range(len(converted_images)):
    string = 'in: '
    for j in range(1, len(converted_images[i])):
        string += (str(converted_images[i][j])+' ')
    # If first item is 0 then it is letter M
    if converted_images[i][0] == 0:
        string += ('\nout: 1.0 0.0 \n')
    # If first item is 1 then it is letter P
    else:
        string += ('\nout: 0.0 1.0 \n')
    L.append(string)

file_object.writelines(L)
