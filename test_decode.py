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

# Load the decoder model
decoder = kr.models.load_model('decoder.h5')  # Replace 'decoder.h5' with your decoder model filename

# # Reshape the reduced datasets for processing
# reduced_dataset = reduced_dataset.reshape(-1, reduced_dataset.shape[1], reduced_dataset.shape[2], 1)
# reduced_queryset = reduced_queryset.reshape(-1, reduced_queryset.shape[1], reduced_queryset.shape[2], 1)

# Convert the reduced datasets to float32 and normalize
max_value = np.max(reduced_dataset)
reduced_dataset = reduced_dataset.astype('float32') / max_value
reduced_queryset = reduced_queryset.astype('float32') / max_value

# Use the decoder model to restore the dataset and queryset
restored_dataset = decoder.predict(reduced_dataset)
restored_queryset = decoder.predict(reduced_queryset)

# Restore the datasets to their original scale
restored_dataset = (restored_dataset * max_value).astype('uint8')
restored_queryset = (restored_queryset * max_value).astype('uint8')

# Display the reconstructed images
plt.figure(figsize=(10, 4))

# Display a few reconstructed images from the restored dataset
for i in range(5):
    plt.subplot(2, 5, i + 1)
    plt.imshow(restored_dataset[i], cmap='gray')
    plt.axis('off')

# Display a few reconstructed images from the restored queryset
for i in range(5):
    plt.subplot(2, 5, 5 + i + 1)
    plt.imshow(restored_queryset[i], cmap='gray')
    plt.axis('off')

plt.tight_layout()
plt.show()


