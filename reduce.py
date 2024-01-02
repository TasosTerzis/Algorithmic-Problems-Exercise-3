# code to reduce dimension of images
import matplotlib.pyplot as plt
import keras as kr
import numpy as np
import argparse
import struct

# command line looks like: python3 reduce.py –d input/10k_images.dat -q input/100from60k.dat -od input/10k_images_REDUCED.dat -oq input/100from60k_REDUCED.dat
# read in command line arguments
parser = argparse.ArgumentParser()
parser.add_argument('-d', '--dataset', help='dataset file', required=True)
parser.add_argument('-q', '--queryset', help='queryset file', required=True)
parser.add_argument('-od', '--output_dataset_file',
                    help='output dataset file', required=True)
parser.add_argument('-oq', '--output_query_file',
                    help='output query file', required=True)
args = parser.parse_args()

# make a function to get the MNIST data from a file and return it as a numpy array
def read_mnist_images(file_path):
    with open(file_path, 'rb') as f:
        # Read magic number, number of images, rows, and columns
        magic_number, num_images, rows, cols = struct.unpack('>IIII', f.read(16))

        # Read image data
        image_size = rows * cols
        images = np.frombuffer(f.read(image_size * num_images), dtype=np.uint8)
        images = images.reshape(num_images, rows, cols)

    return images, magic_number, num_images, rows, cols

# Read input files
dataset, d_magic_number, d_num_images, d_rows, d_cols = read_mnist_images(args.dataset)
queryset, q_magic_number, q_num_images, q_rows, q_cols = read_mnist_images(args.queryset)

# load the encoder model
encoder = kr.models.load_model('encoder.h5')

# # preprocess the dataset and queryset
dataset = dataset.reshape(-1, 28, 28, 1)
queryset = queryset.reshape(-1, 28, 28, 1)

# #convert the dataset and queryset to float32
dataset = dataset.astype('float32')
queryset = queryset.astype('float32')

# # normalize the dataset and queryset
max_value = np.max(dataset) # 255
dataset = dataset / max_value
queryset = queryset / max_value

# use the encoder model to reduce the dimension of the dataset and queryset
output_dataset = encoder.predict(dataset)
output_queryset = encoder.predict(queryset)

# print the shape of the output dataset and queryset
print(output_dataset.shape)
print(output_queryset.shape)

#######################################################################################

# restore the dataset and queryset
dataset = dataset * max_value
queryset = queryset * max_value

# convert the dataset and queryset to uint8
dataset = dataset.astype('uint8')
queryset = queryset.astype('uint8')

########################################################################################
# TESTING
########################################################################################

# # preprocess the dataset and queryset so that they can be decoded
# dataset = dataset.reshape(-1, 28, 28, 1)
# queryset = queryset.reshape(-1, 28, 28, 1)

# # convert the dataset and queryset to float32
# dataset = dataset.astype('float32')
# queryset = queryset.astype('float32')

# # normalize the dataset and queryset
# max_value = np.max(dataset) # 255
# dataset = dataset / max_value
# queryset = queryset / max_value

# # use the decoder model to restore the dataset and queryset
# dataset = decoder.predict(output_dataset)
# queryset = decoder.predict(output_queryset)

# # restore the dataset and queryset
# dataset = dataset * max_value
# queryset = queryset * max_value

# # convert the dataset and queryset to uint8
# dataset = dataset.astype('uint8')
# queryset = queryset.astype('uint8')


# # Plot or display the images
# plt.figure(figsize=(10, 4))

# # Display a few reconstructed images from the first dataset
# for i in range(5):
#     plt.subplot(2, 5, i + 1)
#     plt.imshow(dataset[i], cmap='gray')
#     plt.axis('off')

# # Display a few reconstructed images from the second dataset
# for i in range(5):
#     plt.subplot(2, 5, 5 + i + 1)
#     plt.imshow(queryset[i], cmap='gray')
#     plt.axis('off')

# plt.tight_layout()
# plt.show()

########################################################################################



# save the output dataset and queryset to output files
with open(args.output_dataset_file, 'wb') as f:
    f.write(struct.pack('>IIII', d_magic_number, d_num_images, d_rows, 1))
    f.write(output_dataset.tobytes())

with open(args.output_query_file, 'wb') as f:
    f.write(struct.pack('>IIII', q_magic_number, q_num_images, q_rows, 1))
    f.write(output_queryset.tobytes())


    