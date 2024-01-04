import matplotlib.pyplot as plt
import keras as kr
import numpy as np
import struct

# Define a function to read MNIST images from a file
def read_mnist_images(file_path):
    with open(file_path, 'rb') as f:
        # Read magic number, number of images, rows, and columns
        magic_number, num_images, rows, cols = struct.unpack('>IIII', f.read(16))

        # Read image data
        image_size = rows * cols
        images = np.frombuffer(f.read(image_size * num_images), dtype=np.uint8)
        images = images.reshape(num_images, rows, cols)

    return images, magic_number, num_images, rows, cols

# Load the reduced dataset and queryset
reduced_dataset_file = 'input/10k_images_REDUCED.dat'  # Change this path to your reduced dataset file
reduced_queryset_file = 'input/100from60k_REDUCED.dat'  # Change this path to your reduced queryset file

reduced_dataset, d_magic_number, d_num_images, d_rows, d_cols = read_mnist_images(reduced_dataset_file)
reduced_queryset, q_magic_number, q_num_images, q_rows, q_cols = read_mnist_images(reduced_queryset_file)

print(d_magic_number, d_num_images, d_rows, d_cols)

# print the first images as pixel values
print(reduced_dataset[0])
print(reduced_queryset[0])

# convert the dataset and queryset to float32
reduced_dataset = reduced_dataset.astype('float32')
reduced_queryset = reduced_queryset.astype('float32')

# normalize the dataset and queryset
max_value = 255
reduced_dataset = reduced_dataset / max_value
reduced_queryset = reduced_queryset / max_value

decoder = kr.models.load_model('decoder.h5')

# use the decoder model to restore the dimension of the dataset and queryset
temp_dataset = decoder.predict(reduced_dataset)
temp_queryset = decoder.predict(reduced_queryset)

# restore the temp_dataset and temp_queryset
temp_dataset = temp_dataset * 255
temp_queryset = temp_queryset * 255

# convert the temp_dataset and temp_queryset to uint8
temp_dataset = temp_dataset.astype('uint8')
temp_queryset = temp_queryset.astype('uint8')

# Display a few reconstructed images from the restored dataset
for i in range(5):
    plt.subplot(2, 5, i + 1)
    plt.imshow(temp_dataset[i], cmap='gray')
    plt.axis('off')

# Display a few reconstructed images from the restored queryset
for i in range(5):
    plt.subplot(2, 5, 5 + i + 1)
    plt.imshow(temp_queryset[i], cmap='gray')
    plt.axis('off')

plt.show()
plt.axis('off')

